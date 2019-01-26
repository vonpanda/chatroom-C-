#include"client.h"

int client::init()
{
	//initial server addrs;
	std::cout<<"could you enter your name:"<<std::endl;
	std::cin>>this->name;
	this->server_addr.sin_family=AF_INET;
	this->server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
	this->server_addr.sin_port=htons(SERVER_PORT);
	//create socket
	this->client_con=socket(AF_INET,SOCK_STREAM,0);
	//create pipe
	if(pipe(this->fd)==-1)
	{
		std::cout<<"error by pipe."<<std::endl;
		return -1;
	}


	//epoll

	this->e_fd=epoll_create(3);
	
	this->read_pipe.data.fd=this->fd[0];
	this->read_pipe.events=EPOLLIN;

	
	this->recv_ser.data.fd=this->client_con;
	this->recv_ser.events=EPOLLIN;
	epoll_ctl(this->e_fd,EPOLL_CTL_ADD,this->fd[0],&this->read_pipe);
	epoll_ctl(this->e_fd,EPOLL_CTL_ADD,this->client_con,&this->recv_ser);

	return 0;
}

int client::connects()
{
	
	if(connect(this->client_con,(struct sockaddr *)&this->server_addr,sizeof(this->server_addr))!=0)
	{
		perror("connect:");
		return -1;
	}
	return 0;
}


int client::fork_both()
{
	pid_t pid=fork();
	if(pid==-1)
	{
		perror("error by fork:");
	}
	else if(pid==0)
	{	//nonblock;
		fcntl(this->client_con,F_SETFL,fcntl(this->client_con,F_GETFD,0)|O_NONBLOCK);
		struct epoll_event ev[3];
		while(1)//read pipe; write server;recv server;
		{
			char ch[MESSAGE_LEN];
			bzero(&ev,sizeof(ev));

			int epoll_count=epoll_wait(this->e_fd,ev,3,-1);
			for(int i=0;i<epoll_count;i++)//make epoll
			{
				if((ev[i].data.fd)==this->client_con)//from server
				{
					//bzero(ch,1024);
					int recount=recv(this->client_con,ch,sizeof(ch),0);
					std::cout<<"recv circle."<<std::endl;
					if(recount==0)
					{
						std::cout<<"socket was close."<<std::endl;
						exit(0);
					}
					else if(recount == -1)
					{
						perror("error by recv:");
					}
					else
					{
						std::cout<<"recv to server:"<<ch<<std::endl;
					}
				}
				else
				{
					bzero(ch,MESSAGE_LEN);
					close(this->fd[1]);//close write;
					int rec_i=read(this->fd[0],ch,sizeof(ch));
					
					if(rec_i==0)
					{
						//close(this->fd[0]);
						exit(0);
					}
					std::cout<<send(this->client_con,ch,rec_i,0)<<std::endl;
				}
			
			}
		//child
		}	
	}
	else
	{
		while(1)
		{
			if(waitpid(pid,NULL,WNOWAIT)==pid)
			{
				this->close_all();
				exit(0);
			}
			

			std::string ch;//write pipe			bzero(ch,1024);
			std::cin>>ch;	
			ch=this->name+":"+ch;
			if(ch.size()==0)
				continue;
			
			close(this->fd[0]);//close read;
			write(this->fd[1],ch.data(),ch.size());
			//int recount=recv(this->client_con,chx,sizeof(chx),0);
			//std::cout<<"now ,i am recv to pipe:"<<chx<<std::endl;
			
		}
		//father
	}
}

int client::close_all()
{
	close(this->fd[0]);
	close(this->fd[1]);
	close(this->client_con);
	close(this->e_fd);
}