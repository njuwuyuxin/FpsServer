#include "room/Room.h"

Room::Room(string host){
    uuid_t uuid;
    uuid_generate(uuid);
    char str[50];
    uuid_unparse(uuid,str);
    room_id.assign(str);
    Log::log("Create Room Success",INFO);
    Log::log("uuid = "+room_id,INFO);

    room_host = host;
    room_member.push_back(room_host);
    current_member = 1;
}
bool Room::add_member(string member){
    if(current_member==max_member)
        return false;
    room_member.push_back(member);
    current_member++;
    return true;
}
string Room::get_room_id(){
    return room_id;
}