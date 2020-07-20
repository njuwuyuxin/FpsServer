#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <uuid/uuid.h>
#include <sstream>
#include <vector>
#include "../log/Log.h"
using namespace std;

class Room{
public:
    Room(string host);              //需要用房主用户名创建房间
    bool add_member(string member);
    string get_room_id();
private:
    string room_id;                 //uuid
    vector<string> room_member;     //房间成员
    int max_member;                 //最大成员数
    int current_member;             //房间当前人数
    string room_host;               //房主
};

#endif