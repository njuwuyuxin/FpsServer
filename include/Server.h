#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/epoll.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <regex>
#include <libconfig.h++>

#include "request/Request.h"
#include "log/Log.h"
#include "thread/ThreadPool.h"
#include "handler/Handler.h"
#include "room/Room.h"

using namespace std;
using namespace libconfig;

const int MAX_EVENT_NUMBER = 10000;

class Server{
    friend class Handler;
public:
    Server();
    ~Server();
    inline int get_sock_id(){ return server_sock; };
    inline u_short get_port(){ return port; };
    void start_listen();
    static void accept_request(int client_sock,Server* t);

private:
    int server_sock;
    u_short port;
    ThreadPool thread_pool;                             //工作线程池
    int work_thread_count;                              //工作线程数
    int epollfd;                                        //epoll文件描述符
    unsigned int request_queue_length;                  //请求队列长度，即最大可同时处理请求
    
    Handler request_handler;                            //请求处理句柄
    /*Room相关*/
    unordered_map<string,Room*> rooms;                  //每个房间对应一个uuid
    
    /*Log模块相关参数*/
    string log_path;                                    //log输出目录
    bool info_on;
    bool debug_on;
    bool warn_on;
    bool error_on;

    void load_config(string path);                      //加载配置文件
    void startup();
    void add_epoll_fd(int event_fd);                    //向内核事件表注册事件
};

#endif