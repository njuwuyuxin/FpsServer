#include "handler/Handler.h"

void Handler::init(){
    handler_map.insert(pair<string,CMF>("CreateRoom",handleCreateRoom));
}


void Handler::handleRequest(const Request& req){
}

void Handler::handleCreateRoom(const Request& req){

}