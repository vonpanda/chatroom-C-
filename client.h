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
	int client_con;//socket fd
	std::string name;//user name
	int fd[2];//pipe fd
	struct sockaddr_in server_addr;//server addrs 
	int e_fd;
	struct epoll_event read_pipe;
	struct epoll_event recv_ser;

public:
	int connects();//use connect server
	int init();//use initial something
	int fork_both();//start chat.
	int close_all();
};
