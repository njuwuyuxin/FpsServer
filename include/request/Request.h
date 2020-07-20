#ifndef REQUEST_H
#define REQUEST_H
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "../log/Log.h"
using namespace std;

class Request{
public:
    Request(string raw_data);
    const unordered_map<string,string>& getHead();
    const unordered_map<string,string>& getBody();
    string getHeadItem(string key) const;
    string getBodyItem(string key) const;
private:
    unordered_map<string,string> head;
    unordered_map<string,string> body;
};

#endif