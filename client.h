#include"commom.h"
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<iostream>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include<thread>
#include<string.h>
#include<sys/wait.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>

class client{
private:
	//客户端连接到服务端的套接字描述符
	int client_con;

	//用于标记用户发言的名字
	std::string name;
	
	//用于保存管道通信的套接字数组
	int fd[2];
	
	//服务端addrs
	struct sockaddr_in server_addr;
	
	//epoll套接字
	int e_fd;

	//用于注册epoll的epoll_event结构体
	struct epoll_event read_pipe;//注册从管道读
	struct epoll_event recv_ser;//注册从服务器读

public:
	int connects();//用于连接服务器
	int init();//初始化，包括初始化名字、服务端addrs、套接字描述符、创建epoll并注册事件。
	int fork_both();//使用fork，做成两个进程。
	/*
		1.父进程不断读取用户输入并且发送给服务端，同时父进程检测子进程状态，当子进程退出，则其一并退出。
		2.子进程使用epoll_wait监听事件，同时将client_con设置非阻塞。
	*/
	int close_all();
};
