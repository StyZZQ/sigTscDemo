#pragma once
#include "inc.h"
#include <chrono>
#include <cstring>

#ifdef __linux__
#include <sys/socket.h> //数据类型定义
#include <sys/types.h>  //提供socket函数及数据结构
#include <unistd.h>     //提供通用的文件、目录、程序及进程操作的函数  
#include <netinet/in.h> //定义数据结构sockaddr_in
#include <arpa/inet.h> //提供IP地址转换
#include <fcntl.h>   //提供对文件控制的函
#else
#include <winsock2.h>
#endif
class TcpClient
{
private:
    sockaddr_in centerAddr;	//上位机地址信息
    int sock;

public:
    TcpClient(/* args */):sock(-1)
    {
#ifdef __WIN32__       
        WSADATA ws;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
            ERR("Init socket failed!\n");
#endif        
    }

    void Close()
    {
	    if (sock == -1)
			return;
#ifdef __linux__
		close(sock);
#else	//__WIN32__
	    closesocket(sock);
#endif
		sock = -1;
    }
    ~TcpClient()
    {
        Close();
#ifdef __WIN32__
        WSACleanup();
#endif        
    }
    void setCenterAddr(uint32_t& ip ,uint16_t& port)
    {
        centerAddr.sin_family = PF_INET;
		centerAddr.sin_addr.s_addr = htonl(ip);
		centerAddr.sin_port = htons(port);
		memset(centerAddr.sin_zero, 0, sizeof(centerAddr.sin_zero));
    }
    bool Open()
    {
        if(sock != -1)
            return true;
  
	    sock = socket(PF_INET, SOCK_STREAM, 0); //protocol使用0则根据前两个参数使用默认的协议。

        if (INVALID_SOCKET == sock)
        {
            ERR("create socket failed!\n");
            return false;
        }


#ifdef __WIN32__
//socket非阻塞设置
        unsigned long long_para = 1;
		if (ioctlsocket(sock, FIONBIO, &long_para) == SOCKET_ERROR) 
		{
			ERR("set nonblock failed!\n");
			return false;
		}
#else
 //F_SETFL 几个标志如下
// O_APPEND：强制每次写(write)操作都添加在文件大的末尾
// O_NONBLOCK：非阻塞I/O
// O_DIRECT：最小化或去掉reading和writing的缓存影响
// O_ASYNC：当I/O可用的时候,允许SIGIO信号发送到进程组 
            int flag;
            flag = fcntl(sock, F_GETFL, 0); //获取文件flags
            if(fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0) //设置成非阻塞
            {
                ERR("set nonblock failed!\n");
			    return false;
            }
#endif
        if (connect(sock, (struct sockaddr *) (&centerAddr),
                    sizeof(struct sockaddr)) == -1)
        {
            Close();
            return false;
        }
        INFO("create socket success!\n");
	    return true;
    }
    
	int Write(const void *buf, int len)
    {   
#ifdef __linux__ //send函数最后一个参数windows下一般设置为0,linux下最好设置为MSG_NOSIGNAL 
        //linux下当连接断开，还发数据的时候，不仅send()的返回值会有反映，
        //而且还会向系统发送一个异常消息，如果不作处理，系统会出BrokePipe，程序会退出 
        //设MSG_NOSIGNA不用返回SIGPIPE错误。EPIPE错误还是要返回的。      
        auto ret = send(sock, buf, len, MSG_NOSIGNAL);
#else
        auto ret = send(sock, buf, len, 0); 
#endif
        if (len != ret) //返回实际copy字节数，拷贝失败或网络断开返回-1
        {
            Close();
            ERR("snd len != req,err:%s\n",strerror(errno));
            return 0;
        }
        return ret;
    }

	int Read(void *buf, int len)
    {   
#ifdef __linux__
        auto ret = recv(sock, buf, len, MSG_NOSIGNAL);
#else
        auto ret = recv(sock, buf, len, 0);
#endif 
        if (0 == ret)   // ret >0 :拷贝字节数，-1(SOCKET_ERROR):copy时出错，0:网络中断了
        {
            Close();
            ERR("read len = 0,err:%s\n",strerror(errno));
        }
        return ret;        
    }

    //在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限
	void SetRecvTimeOut(int sec)    
	{
#ifdef __linux__
		struct timeval timeout = {sec, 0};
#else	//__WIN32__
		int timeout = sec * 1000;
#endif
		//设置接收超时
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	}
	
	void SetSendTimeOut(int sec)
	{
#ifdef __linux__
		struct timeval timeout = {sec, 0};
#else	//__WIN32__
		int timeout = sec * 1000;
#endif
		//设置发送超时
		setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
	}
    


};




