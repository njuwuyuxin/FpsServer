#include "request/Request.h"

//最后参数为重复标志，即分隔符连续重复出现是否视为一个
vector<string> splitString(string srcStr, string delimStr,bool repeatedCharIgnored)
{
    std::vector<std::string> resultStringVector;
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c){if(delimStr.find(c)!=std::string::npos){return true;}else{return false;}}/*pred*/, delimStr.at(0));//将出现的所有分隔符都替换成为一个相同的字符（分隔符字符串的第一个）
    size_t pos=srcStr.find(delimStr.at(0));
    std::string addedString="";
    while (pos!=std::string::npos) {
        addedString=srcStr.substr(0,pos);
        if (!addedString.empty()||!repeatedCharIgnored) {
            resultStringVector.push_back(addedString);
        }
        srcStr.erase(srcStr.begin(), srcStr.begin()+pos+1);
        pos=srcStr.find(delimStr.at(0));
    }
    addedString=srcStr;
    if (!addedString.empty()||!repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
    }
    return resultStringVector;
}

Request::Request(string raw_data){
    size_t pos = raw_data.find("\r\n");
    if(pos==string::npos){
        Log::log("bad request",ERROR);
        Log::log(raw_data,ERROR);
        return;
    }
    string head_str = raw_data.substr(0,pos);
    string body_str = raw_data.substr(pos+2);
    vector<string> head_lines = splitString(head_str,"\n",false);
    vector<string> body_lines = splitString(body_str,"\n",false);
    for(auto line:head_lines){
        size_t delima = line.find(':');
        head.insert(pair<string,string>(line.substr(0,delima),line.substr(delima+1)));
    }
    for(auto line:body_lines){
        size_t delima = line.find(':');
        body.insert(pair<string,string>(line.substr(0,delima),line.substr(delima+1)));
    }
}

const unordered_map<string,string>& Request::getHead(){
    return head;
}
const unordered_map<string,string>& Request::getBody(){
    return body;
}
string Request::getHeadItem(string key) const{
    auto iter = head.find(key);
    if(iter!=head.end())
        return iter->second;
    else
        return "";
}
string Request::getBodyItem(string key) const{
    auto iter = body.find(key);
    if(iter!=body.end())
        return iter->second;
    else
        return "";
}