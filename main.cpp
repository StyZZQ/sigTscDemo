#include "gb2017.h"
#include "tcpClient.h"
#include "tcpServer.h"
#include "operate.h"
#include "singleton.h"

int main()
{
    TcpClient &client = Singleton<TcpClient>::getInstance();
	TcpServer &server = Singleton<TcpServer>::getInstance();
    Operate &op = Singleton<Operate>::getInstance();
    All all = op.all;
    client.setCenterAddr(all.basic.info.centerIPV4.IP, all.basic.info.centerIPV4.port);
    server.startServer(20000);
    server.communication();
    return 0;

}