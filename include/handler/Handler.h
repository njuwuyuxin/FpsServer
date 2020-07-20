#ifndef HANDLER_H
#define HANDLER_H
#include <unordered_map>
#include "../request/Request.h"

typedef void (Handler::*CMF)(const Request& req);

class Handler{
public:
    Handler();
    void handleRequest(const Request& req);
    void init();
private:
    unordered_map<string,CMF> handler_map;
    void handleCreateRoom(const Request& req);
};

#endif