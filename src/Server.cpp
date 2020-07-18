#include "Server.h"

Server::Server():thread_pool(this,8){
    load_config("../Serverconf.cfg");
    startup();
    thread_pool.init(work_thread_count);             //启动工作线程
    Log::init(log_path,info_on,debug_on,warn_on,info_on);
}

Server::~Server(){
}

void Server::load_config(string path){
    Log::log("Loading config",INFO);
    Config config;
    try{
        config.readFile(path.c_str());
        int i_port;
        if(!config.lookupValue("server.port",i_port)){
            Log::log("server port setting not found, use default setting",WARN);
            port = 0;
        }
        else{
            port = i_port;
        }
        if(!config.lookupValue("server.max_request",request_queue_length)){
            Log::log("server max_request setting not found, use default setting",WARN);
            request_queue_length = 5;
        }
        if(!config.lookupValue("server.work_thread_count",work_thread_count)){
            Log::log("server work_thread_count setting not found, use default setting",WARN);
            work_thread_count = 8;
        }

        /*********** Log ***********/
        if(!config.lookupValue("server.log.log_path",log_path)){
            Log::log("server.log.log_path setting not found, use default setting",WARN);
            log_path = "";
        }
        if(!config.lookupValue("server.log.info_on",info_on)){
            Log::log("server.log.info_on setting not found, use default setting",WARN);
            info_on = true;
        }
        if(!config.lookupValue("server.log.debug_on",debug_on)){
            Log::log("server.log.debug_on setting not found, use default setting",WARN);
            debug_on = true;
        }
        if(!config.lookupValue("server.log.warn_on",warn_on)){
            Log::log("server.log.warn_on setting not found, use default setting",WARN);
            warn_on = true;
        }
        if(!config.lookupValue("server.log.error_on",error_on)){
            Log::log("server.log.error_on setting not found, use default setting",WARN);
            error_on = true;
        }
    }
    catch(FileIOException io_exception){
        cout<<"Config not found, use default settings"<<endl;
        port = 0;
        request_queue_length = 5;
    }
    catch(SettingNotFoundException set_not_found){
        Log::log("settings not found",ERROR);
    }
}

void Server::startup(){
    int on = 1;
    struct sockaddr_in name;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);    //使用TCP协议
    if (server_sock == -1)
        Log::log("create socket failed",ERROR);
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  {  
        Log::log("setsockopt failed",ERROR);
    }
    if (bind(server_sock, (struct sockaddr *)&name, sizeof(name)) < 0){
        Log::log("bind failed",ERROR);
    }

    if (port == 0){  //动态分配端口
        socklen_t namelen = sizeof(name);
        if (getsockname(server_sock, (struct sockaddr *)&name, &namelen) == -1)
            Log::log("getsockname failed",ERROR);
        port = ntohs(name.sin_port);
    }
    if (listen(server_sock, request_queue_length) < 0)   //listen第二个参数为连接请求队列长度，代表最多同时接受n个连接请求
        Log::log("listen failed",ERROR);
}

void Server::start_listen(){
    stringstream ss;
    string s_port;
    ss<<port;
    ss>>s_port;
    Log::log("FpsServer running on port "+s_port,INFO);

    epollfd = epoll_create(5);
    add_epoll_fd(server_sock);      //把监听socket加入内核事件表

    epoll_event events[MAX_EVENT_NUMBER];
    while(1){
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        for (int i = 0; i < number; i++){
            int sockfd = events[i].data.fd;

            //处理新到的客户连接
            if (sockfd == server_sock){
                int client_sock = -1;
                struct sockaddr_in client_name;
                socklen_t  client_name_len = sizeof(client_name);
                client_sock = accept(server_sock,
                        (struct sockaddr *)&client_name,
                        &client_name_len);
                if (client_sock == -1){
                    Log::log("accept failed",ERROR);
                    continue;
                }
                add_epoll_fd(client_sock);      //把客户端socket加入内核事件表
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                Log::log("epoll end",DEBUG);
                //服务器端关闭连接
            }
            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN){
                thread_pool.append(sockfd);
                // thread accept_thread(accept_request,sockfd,this);
                // accept_thread.detach();
            }
            else if (events[i].events & EPOLLOUT){
                Log::log("epoll out",DEBUG);
            }
        }
    }

    close(server_sock);
}

void Server::accept_request(int client_sock, Server* t)
{
    int client = client_sock;
    char buf[1024];
    read(client_sock,(void*)buf,1024);
    string req(buf);

    send(client,req.c_str(),req.size(),0);
    close(client);
}

void Server::add_epoll_fd(int event_fd){
    epoll_event event;
    event.data.fd = event_fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, event_fd, &event);
}