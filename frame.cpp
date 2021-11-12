#include "frame.h"
std::atomic_bool Frame::online;

/*
    1.协议约定开始字节为0x7E，结束字节为0x7D，转义字符为0x5C，在报文数据中，遇到开始字
    节、结束字节、转义字符，在其前增加转义字符0x5C；
    2.报文长度，不包含头尾
    3.校验生成式：X16+x12+x2+1,不包含头尾
*/
bool Frame::parse(U8* buffer, int& size)
{
    
    int i, j, len;     //协议版本起始下标i，原始报文长度len
	U32 sum = 0;
	U16 checksum = 0;
	
	if (buffer[0] != 0x7E || buffer[size - 1] != 0x7D) { //判断头尾字节
		ERR("receive data head or tail is invalid!");
		return false;
	}
    if (buffer[1] == 0x5C) {
        if (buffer[3] == 0x5C) 
            len = (buffer[2] << 8 ) | buffer[4]; 
        else 
            len = (buffer[2] << 8 ) | buffer[3]; 
    } else {
        if (buffer[2] == 0x5C) 
            len = (buffer[1] << 8 ) | buffer[3];
        else
            len = (buffer[1] << 8 ) | buffer[2];
    }
    
    if (len == 0)       //报文长度不为0，从协议版本到校验位的字节长度
        return false;

    memset(dataBuf,0,sizeof(dataBuf));

	for (i = 1, j = 0; j < len + 2; j++) {  //拷贝还原后的报文长度~校验位,还原的数据 j < 2(报文长度) + len(ver~校验位) 
		if (buffer[i] == 0x5C && buffer[i + 1] == 0x7E) {
            dataBuf[j] = 0x7E;
            i += 2;
        }	
		else if (buffer[i] == 0x5C && buffer[i + 1] == 0x7D) {
			dataBuf[j] = 0x7D;
            i += 2;
        }
        else if (buffer[i] == 0x5C && buffer[i + 1] == 0x5C)  {
            dataBuf[j] = 0x5C;
            i += 2;
        }
        else {
            dataBuf[j] = buffer[i];
            i++;
        }		
    }


    checksum = crc16(dataBuf, len);  //crc16 涉及通信帧结构报文长度(2位)至数据值(16位)
    U16 oCrc16 = dataBuf[j - 2] | dataBuf[j - 1] << 8;  //网络字节序
    if (checksum != oCrc16) {
        ERR("crc wrong, calculate crc16: %#x, rcv crc16: %#x", checksum, oCrc16);
        return false;
    }

	memcpy((char *)&head, dataBuf + 2, sizeof(FrameHead)); //拷贝帧头部分
                                                          
	return frameHeadCheck(len);        
}

void Frame::send(int& frameType, int& num, int& eleLen, Data** sndData)
{
    fillHead(frameType, std::move(num));
    frameLen = 13 ; // 应答报文不包含数据部分，共13个字节
    for (int i = 0; i < num; i++) {
        if (sndData[i]->len != 0) {     //如果数据值长度不为0
            strData.append((char *)sndData[i], sndData[i]->len + 2);
            frameLen += 2 + sndData[i]->len ; //总长度 += 索引(1) + 长度(1) + 数据值长度
        } else {
            strData.append((char *)sndData[i], eleLen + 6);
            frameLen += 6 + eleLen; //适用于整表下发的情况，data.len长度为0，则报文长度 += 6 + 元素值长度
        }
    }
    packAndSend();
}

void Frame::send(int& frameType)
{
    fillHead(frameType,0);
    frameLen = 12 ;
    packAndSend();
}

void Frame::send(const GBTrafficInfo &traffic)
{
    if (!online)
		return;
	//填充帧头部
	fillHead(Type_Trap, 1);
	data.append((char*)&traffic, sizeof(GBTrafficInfo));	//填充帧数据
	packAndSend();		//组装帧并发送
}		
void Frame::send(const GBAlarmInfo &alarm)
{
    if (!online)
		return;
	//填充帧头部
	fillHead(Type_Trap, 1);
	data.append((char*)&alarm, sizeof(GBAlarmInfo));	//填充帧数据
	packAndSend();		//组装帧并发送    
}		        
void Frame::send(const GBFaultInfo &fault)
{
    if (!online)
		return;
	//填充帧头部
	fillHead(Type_Trap, 1);
	data.append((char *)&fault, sizeof(GBFaultInfo));	//填充帧数据
	packAndSend();		//组装帧并发送
}	      


bool Frame::frameHeadCheck(int& frameLen)
{
#ifdef CHECK_FRAME 
    if (head.version != 0x0100) {
        ERR("version is wrong, it's %#x", head.version);
        return false;
    }

    if (head.tscID != 0x01000000) {
        ERR("signalMachineID is wrong, it's %#x", head.tscID);
        return false;
    }
#endif

    if (head.frameType != Type_Query && head.frameType != Type_Set &&
        head.frameType != Type_Heart_Search && head.frameType != Type_Broadcast) { 
    
        ERR("frameType is wrong, it's %#x", head.frameType);
        return false;
    }

    //打包报文数据部分成data结构体
    if (head.frameType == Type_Set || head.frameType == Type_Query) { //设置报文或查询报文
        for (int i = 0; i < head.dataNum; i++) {
            int valLen = 0; //元素值长度
            for (int j = 11; j < sizeof(dataBuf); ) {
                if (dataBuf[j + 1] != 0)   //全表下发长度不为0
                    valLen = dataBuf[j + 1] - 4;  
                else if (head.dataNum == 1 && dataBuf[j + 1] == 0 )     //数据值数量为1，数据值长度为0
                    valLen = frameLen - 19;                             //报文长度-19 = 元素值长度
                data[i] = (Data*)realloc(data[i],sizeof(Data) + valLen);   
                data[i]->index = dataBuf[j];
                data[i]->len = dataBuf[++j];
                data[i]->classId = dataBuf[++j];
                data[i]->objId = dataBuf[++j];
                data[i]->attrId = dataBuf[++j];
                data[i]->eleId = dataBuf[++j];
                memcpy(data[i]->value, dataBuf + j, valLen);
                j += valLen;
            }   
        }
    } 


	return true;
}
void Frame::fillHead(int& frameType, int&& num)
{
    FrameHead &h = head;        //获取到的head直接使用，但要修改帧类型和num
    h.frameType = frameType;    
    h.dataNum = num;
    strData.clear();
    if (frameType == Type_Trap) {//主动上传包的流水号为0
        h.frameSeq = 0;
    }
    if (frameType == Type_Heart_Reply) //心跳应答，报文内容不包含数据值数量
        strData.append((char *)&h, sizeof(FrameHead) - 1);
    else
        strData.append((char *)&h, sizeof(FrameHead));
}

void Frame::packAndSend()
{
    int i, j, len;
    U8 crcBuffer[MAX_FRAME_LEN] = {0};  //报文长度~校验位
    U8 sndBuffer[MAX_FRAME_LEN] = {0};  //用作发送报文，需转义字符
    TcpClient &client = Singleton<TcpClient>::getInstance();
                    
    crcBuffer[0] = frameLen >> 8;        
    crcBuffer[1] = frameLen & 0xff;      

    for (i = 0, len = 2; i < strData.size(); i++, len++) {
        crcBuffer[len] = strData[i];
        if (len > MAX_FRAME_LEN - 2) 
            break;
    } 
   
    U16 crc = crc16(crcBuffer, len);  //crc-16 校验  
    crcBuffer[len] = crc >> 8;
    crcBuffer[++len] = crc & 0xff;

    sndBuffer[0] = 0x7e;  //开始字节
    for (i = 0, j = 1; i <= len; i++, j++) {  //检查数据中转义字符，转化并拷贝,拷贝至下标len
        if (crcBuffer[i] == 0x7e) {
            sndBuffer[j] =  0x5c;
            sndBuffer[++j] = 0x7e;
        }
        else if (crcBuffer[i] == 0x7d) {
            sndBuffer[j] =  0x5c;
            sndBuffer[++j] = 0x7d;
        }
        else if (crcBuffer[i] == 0x5c) {
            sndBuffer[j] =  0x5c;
            sndBuffer[++j] = 0x5c;
        }
        else {
            sndBuffer[j] = crcBuffer[i];
        }
        if (j > MAX_FRAME_LEN - 1)
            break;
    }
    sndBuffer[j] = 0x7d;  //结束字节

#ifdef DEBUG    
    printf("show after change snd buffer: ");
    for (i = 0; i < j + 1; i++ )
        printf("%02x ", sndBuffer[i]);
    printf("\n");
#endif   

    if (client.Open())              //连接上位机发送报文
        client.Write(sndBuffer, j + 1);     
}