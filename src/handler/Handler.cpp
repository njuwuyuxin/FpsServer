#include "handler/Handler.h"
#include "Server.h"
Handler::Handler(){

}

void Handler::init(){
    handler_map.insert(pair<string,CMF>("CreateRoom",&Handler::handleCreateRoom));
}

void Handler::bind(Server* s){
    server = s;
}

void Handler::handleRequest(const Request& req,int sock){
    string function_name = req.getHeadItem("Function");
    if(function_name==""){
        Log::log("bad request",ERROR);
        return;
    }
    auto iter = handler_map.find(function_name);
    if(iter==handler_map.end()){
        Log::log("function:"+function_name+" not implemented",WARN);
        return;
    }
    CMF func = iter->second;
    (this->*func)(req,sock);
}

void Handler::handleCreateRoom(const Request& req,int sock){
    Log::log("receive CreateRoom request",DEBUG);
    string resp = "create success";
    send(sock,resp.c_str(),resp.size(),0);
    close(sock);
}