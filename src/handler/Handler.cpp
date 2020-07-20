#include "handler/Handler.h"
#include "Server.h"
Handler::Handler(){

}

void Handler::init(){
    handler_map.insert(pair<string,CMF>("CreateRoom",&Handler::handle_create_room));
}

void Handler::bind(Server* s){
    server = s;
}

void Handler::handle_request(const Request& req,int sock){
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

void Handler::handle_create_room(const Request& req,int sock){
    Log::log("receive CreateRoom request",DEBUG);
    string username = req.getBodyItem("Username");
    Room* new_room = new Room(username);
    server->rooms.insert(pair<string,Room*>(new_room->get_room_id(),new_room));
    string resp = "room_id:"+new_room->get_room_id();
    send(sock,resp.c_str(),resp.size(),0);
    close(sock);
}