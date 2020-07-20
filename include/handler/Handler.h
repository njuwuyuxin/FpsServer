#ifndef HANDLER_H
#define HANDLER_H
#include <sys/socket.h>
#include <unordered_map>
#include "../request/Request.h"

class Handler;
typedef void (Handler::*CMF)(const Request& req,int sock);
class Server;


class Handler{
public:
    Handler();
    void handleRequest(const Request& req,int sock);
    void init();
    void bind(Server* s);
private:
    Server* server;         //绑定的server实例
    unordered_map<string,CMF> handler_map;
    void handleCreateRoom(const Request& req,int sock);
};

#endif