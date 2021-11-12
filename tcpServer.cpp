#include "tcpServer.h"

void TcpServer::startServer(int port)
{
    this->port = port;
    std::thread t([&]{
        int opt = 1;
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  

        sock = socket(AF_INET, SOCK_STREAM, 0); //protocol使用0则根据前两个参数使用默认的协议。

        if (INVALID_SOCKET == sock) {
            ERR("create socket failed!\n");
            return;
        }
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)); //允许套接口和一个已在使用中的地址捆绑（参见bind()）
        if (bind(sock, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1) {
            ERR("bind socket fail!");
            Close(sock);
            return;
        }

        listen(sock, MAX_LISTEN_NUM);

        fd_set set;  
        int maxfd = sock; //初始化
        char dataBuf[MAX_FRAME_LEN];
        std::list<int> clientfd; 
        struct timeval tv = {3, 0};
 
        while (true) {
            memset(dataBuf, 0, sizeof(dataBuf));
            int dataLen = 0;

            FD_ZERO(&set);
            FD_SET(sock, &set);

            for (auto& fd: clientfd) {
                if (fd != 0)
                FD_SET(fd, &set);
            }
  
            int ret = select(maxfd+1, &set, NULL, NULL,&tv); //超时
            if (ret == -1)
                break;
            else if (ret == 0)
                continue;

            if (FD_ISSET(sock, &set)) {  //客户端连接
            
                struct sockaddr_in c_address;  
                int clen = sizeof(c_address);

                int newfd = accept(sock, (struct sockaddr*) &c_address, (socklen_t*)&clen); 
#ifdef DEBUG
                INFO("new client [%s : %d]", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));
#endif                  
                clientfd.emplace_back(newfd);
                if (newfd > maxfd)
                    maxfd = newfd;
            }

            auto pos = clientfd.begin();
            //这个循环是非常必要的，因为你并不知道是哪个连接发过来的数据，所以只有一个一个去找。
            for (auto& it : clientfd) {
                if (FD_ISSET(it, &set)) {//客户端socket有数据请求时
                    dataLen = recv(it, dataBuf, sizeof(dataBuf), 0);
#ifdef DEBUG
                    INFO("server recv buffer len = %d", dataLen);
#endif   
                    if (dataLen <= 0) {
                        Close(it);
                        FD_CLR(it, &set);
                        pos = find(clientfd.begin(), clientfd.end(), it);  //记录下出错的客户端iterator
                    } else { 
                        en_queue(dataBuf, dataLen);
                    }						      
                }	
            }
            if (pos != clientfd.end())  
                clientfd.erase(pos);
	
        }    
    });
    t.detach();
}


void TcpServer::timeOut(int time)
{	
    std::thread timer([&]{
        while (1) {
            if (Frame::online) {  //联机
                if (rcvHeartNum + 3 > sndHeartNum) {  //接受包和发送包误差不超过3个包
                    sndHeartNum++;
                    heartbeatFrame.send(Type_Heart_Reply); //发送心跳查询包
                } else {
                    Frame::online = false;
                    sndHeartNum = 0;
                    rcvHeartNum = 0;
                }
            } else  //离线
                heartbeatFrame.send(Type_Heart_Reply);
            std::this_thread::sleep_for(std::chrono::seconds(time));  //精度秒级
        }
    });
    timer.detach();
}

void TcpServer::communication() 
{	
    std::thread com([&]{
        while (1)
        {
            int len = 0;
            char recvbuf[MAX_FRAME_LEN];
            Frame frame;
            FrameHead &head = frame.head;
            memset(recvbuf, 0, sizeof(recvbuf));
            len = de_queue(recvbuf);

            if (len > 0 && frame.parse((U8*)recvbuf, len))		//解析报文
            {
                if (!Frame::online)
                {
                    if (head.frameType == Type_Heart_Search) {
                        Frame::online = true;
                        rcvHeartNum++;
                    } // 心跳报文       
                }
                else{
                    if (head.frameType == Type_Query) //查询报文
                    {
                        if(readData(head.dataNum, frame.data)) //读取数据
                            frame.send(Type_Query_Reply, dataNum, eleLen, rightData); //回复报文(正常)
                        else
                            frame.send(Type_Query_Error_Reply, dataNum, eleLen, errorData); //回复报文(错误)
                        clear();
                    }
                    else if (head.frameType == Type_Set)	//设置类型
                    {
                        if(setData(head.dataNum,frame.data)) //设置数据
                            frame.send(Type_Set_Reply, dataNum, eleLen, rightData);  
                        else
                            frame.send(Type_Set_Error_Reply, dataNum, eleLen, errorData);
                        clear();
                    }	
                }
            }
        }
    });
    com.detach();
}