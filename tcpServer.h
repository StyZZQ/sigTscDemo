#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include "inc.h"
#include "operate.h"
#include "frame.h"
#include "gb2017.h"



#ifdef __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#define MAX_LISTEN_NUM 	5
#define MSG_MAX_SIZE 	65535

class TcpServer : public Operate
{
private:
    int port;
    int sock;
	Frame heartbeatFrame; 	//心跳帧
	U32 rcvHeartNum;		//接受心跳数
	U32 sndHeartNum;		//发送心跳数
	std::mutex mx;			//互斥锁
	int msgLen;				//消息长度
	volatile int head; 		//读取的位置
	volatile int tail;		//写入的位置
	std::condition_variable not_full; // 条件变量, 指缓冲区不为满.
    std::condition_variable not_empty; // 条件变量, 指缓冲区不为空.
	char msg_buf[MSG_MAX_SIZE];
public:
    TcpServer(/* args */)
    {
		sock = -1;
		port = 0;
		head = 0;
		tail = 0;
		rcvHeartNum = 0;
		sndHeartNum = 0;
		memset(msg_buf, 0, MSG_MAX_SIZE);
#ifdef __WIN32__       
        WSADATA ws;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
            ERR("Init socket failed!\n");
#endif        
    }

    ~TcpServer()
    {
        Close(sock);
#ifdef __WIN32__
        WSACleanup();
#endif        
    }

	int en_queue(char *p_msg, int len) //写入buff
	{
		if (nullptr == p_msg) {
			perror("p_msg == NULL!");
			return -1;
		}
		
		msgLen = len;
		std::unique_lock<std::mutex> lock(mx);
		not_full.wait(lock, [this]{return (tail + 1) % MSG_MAX_SIZE != head;});//用lambda表达式替代while循环，队列未满唤醒
		
		memcpy(&(msg_buf[tail]), p_msg, len); //拷贝
		tail++;	//写入位置后移

		if (tail >= MSG_MAX_SIZE)		//超出数组大小,重置写入位置为0
			tail = 0;

		not_empty.notify_all();	//通知不为空
		lock.unlock();	//解锁
		return 0;
	}
	
	int de_queue(char *p_msg) //从buf取数据
	{
		std::unique_lock<std::mutex> lock(mx);

		not_empty.wait(lock, [this]{return head != tail;}); //队列不为空唤醒，为空等待

		memcpy(p_msg, &(msg_buf[head]), msgLen);
		head++;

		if (head >= MSG_MAX_SIZE)  //达到数组最大位置
			head = 0;

		not_full.notify_all();	//通知不为满
		lock.unlock();	
		return msgLen;
	}

    void startServer(int port);
	void timeOut(int time);
	void communication();
	
	void Close(int s)  //包裹函数
    {
	    if (s == -1)
			return;
#ifdef __linux__
		close(s);
#else	//__WIN32__
	    closesocket(s);
#endif
		s = -1;
    }
};









