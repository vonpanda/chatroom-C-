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
	int ser_sock;
    struct sockaddr_in ser_addr;
    int e_fd;
    struct epoll_event add_ev;
public:
	int init_ser();
	void server_run();
    std::list<int> conn_fd;
    int add_fd2epoll(int e_fd,int accept_fd,int en_et);
    template<typename T,int N>
    int send_allch(T(&ch)[N]);
};
