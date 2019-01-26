#include"client.h"
int server::init_ser()
{
	this->ser_sock=socket(AF_INET,SOCK_STREAM,0);
    this->ser_addr.sin_family=AF_INET;
    this->ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    this->ser_addr.sin_port=htons(SERVER_PORT);
    
    int err=bind(this->ser_sock,(struct sockaddr *)&this->ser_addr,sizeof(this->ser_addr));
    
    //epoll
    this->e_fd=epoll_create(MAX_USER);


    if(err!=0)
    {
        perror("bind:");
        return -1;
    }

    if(listen(this->ser_sock,MAX_USER)==-1)
    {
        perror("listen:");
        return -1;
    }
    this->add_fd2epoll(this->e_fd,this->ser_sock,1);
}
//maintain the accept fd vector.
//use epoll response message of client
void server::server_run()
{
    struct epoll_event sig_ev[MAX_USER+1];
    while(1)
    {
        int sig_n=epoll_wait(this->e_fd,sig_ev,MAX_USER,-1);
        for(int i=0;i<sig_n;i++)
        {
            int fdx=sig_ev[i].data.fd;
            if(fdx==this->ser_sock)
            {
                struct sockaddr_in client;
                socklen_t len=sizeof(client);
                int conn=accept(this->ser_sock,(struct sockaddr*)&client,&len);
                std::cout<<conn<<std::endl;
                this->conn_fd.push_back(conn);
                this->add_fd2epoll(this->e_fd,conn,1);
                char wel[]="welcome!";
                for(auto i=conn_fd.begin();i!=conn_fd.end();i++)
                {
                    send(*i,wel,9,0);
                }
            }
            else
            {
                char recv_ch[MESSAGE_LEN];
                bzero(&recv_ch,sizeof(recv_ch));
                std::cout<<"response:"<<fdx<<std::endl;
                int x=recv(sig_ev[i].data.fd,recv_ch,sizeof(recv_ch),0);
                if(x==0)
                {
                    std::cout<<"i will close:"<<sig_ev[i].data.fd<<std::endl;
                    close(sig_ev[i].data.fd);
                    this->conn_fd.remove(sig_ev[i].data.fd);
                }
                else if(x==-1)
                {
                    perror("recv:");
                }
                else
                {
                    
                    for(auto i=conn_fd.begin();i!=conn_fd.end();i++)
                    {
                        send(*i,recv_ch,x,0);
                        std::cout<<"i am send all user to:"<<recv_ch<<std::endl;
                    }
                    
                }
            }
        }
    }
    
}

int server::add_fd2epoll(int e_fd,int accept_fd,int en_et)
{
    struct epoll_event ser_ev;
    ser_ev.data.fd=accept_fd;
    ser_ev.events=EPOLLIN;
    if(en_et)
    {
        ser_ev.events=EPOLLIN|EPOLLET;
    }
    epoll_ctl(e_fd,EPOLL_CTL_ADD,accept_fd,&ser_ev);
    fcntl(accept_fd,F_SETFL,fcntl(accept_fd,F_GETFD,0)|O_NONBLOCK);
    return 0;
}

template<typename T,int N>
int server::send_allch(T(&ch)[N])
{
    for(std::list<int>::iterator i=this->conn_fd.begin();i!=this->conn_fd.end();i++)
    {
        std::cout<<send(*i,ch,N,0);
    }
    return 0;
}