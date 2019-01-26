#ifndef CHATROOM_SERVER
#define CHATROOM_SERVER 
#endif
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include "commom.h"
#include <list>
#include<thread>
#include<iostream>
#include<sys/epoll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/stat.h>

class server{
private:
    //服务端用于accept连接的套接字描述符
	int ser_sock;
    //服务端addrs
    struct sockaddr_in ser_addr;
    //epoll套接字
    int e_fd;
    //epoll event，用于注册事件
    struct epoll_event add_ev;
public:
    //初始化addrs、epoll
	int init_ser();
    /*
        思路：将ser_sock放入epoll，当触发时间，判断套接字描述符，若为ser_sock，则将新accept的套接字描述符放入list，同时注册epoll事件。
        当epoll_wait触发事件，且触发的描述符为非ser_sock，则获取数据，当socket close，则在list删除该描述符，如若不然，则获取数据，发送给所有用户（即在List里的所有套接字）；
    */
	void server_run();
    //所维护的客户端描述符表
    std::list<int> conn_fd;
    //用于在epoll中注册事件
    int add_fd2epoll(int e_fd,int accept_fd,int en_et);
    template<typename T,int N>
    int send_allch(T(&ch)[N]);
};
