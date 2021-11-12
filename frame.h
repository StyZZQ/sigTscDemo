#pragma once
#include <atomic>
#include <string>
#include <sstream>
#include "inc.h"
#include "gb2017.h"
#include "tcpClient.h"
#include "exchange.h"


class Frame 
{
private:
    U16 frameLen;
    U8 seq;
public:
    U8 dataBuf[MAX_FRAME_LEN];
    std::string strData;
    static std::atomic_bool online;
    FrameHead head;
    TcpClient client;
    Data* data[255];
public:
    Frame(/* args */):seq(0), frameLen(0) {}
    ~Frame(){}
    bool parse(U8* buffer, int& size);
    void send(int& frameType);                   //心跳
    void send(int& frameType, int& num, int& eleLen, Data** sndData);  //回复报文
    void send(const GBTrafficInfo &traffic);			//主动上传交通数据
	void send(const GBAlarmInfo &alarm);		        //主动上传警报数据
	void send(const GBFaultInfo &fault);		        //主动上传故障数据

    
private:
    bool frameHeadCheck(int& frameLen);
    void fillHead(int& frameType, int&& num);
    void packAndSend();

    U16 crc16(U8 *data, int len)  
    {  
        U16 crc = 0x0000;//初始值 
        while (len--) {
            for (U8 i = 0x80; i != 0; i >>= 1) {  
                if (crc & 0x8000 != 0)       //判断移除位是否为0
                    crc = (crc << 1) ^ 0x1005;//左移后与多项式1005异或
                else                   //否则直接右移
                    crc <<= 1;     
                if ((*data & i) != 0)
                    crc ^= 0x1005;               
            }   
            data++;
        }
        return crc;            //返回校验值         
    }
};

