#include <iostream>
#include "Friend.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/PutFormat.hpp"
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;
extern TcpSocket Socketfd;
extern Account Curuser;
extern PutFormat put;
extern MessageQueue msgQueue;

int result1(Protocol protocol)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(protocol, Curuser.getname(), "", name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        return 1;
    else if(!temp.compare("P"))
        return -1;
    else
        return 0;
}

vector<Friend> Friend_Srv_GetAllstate()
{
    Message msg(Friend_GetAll, Curuser.getname(), "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    vector<Friend> friend_list;
    json data = json::parse(temp);
    vector<string> friends = data.get<vector<string>>();
    for(auto& str:friends){
        Friend friend_obj(str);
        friend_list.push_back(friend_obj);
    }
    return friend_list;
}

vector<string> Friend_Srv_history(string name)
{
    Message msg(Packet_history, Curuser.getname(),"" , name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("F")){
        cout << "\t\t请输入正确的名称" << endl;
        put.stdexit();
    }

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

vector<string> Friend_Srv_GetApply()
{
    Message msg(Packet_GetSetAll, Curuser.getname()+"Q", "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

void Friend_Srv_Apply()
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Packet_Apply, Curuser.getname(), name, "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t好友验证已经发送成功" << endl;
    else if(!temp.compare("P"))
        cout << "\t\t请输入正确的账户名称" << endl;
    else if(!temp.compare("A"))
        cout << "\t\t你们已经是好友了" << endl;
    else if (!temp.compare("N"))
        cout << "\t\t请不要拿自己的名字开玩笑" << endl;
    else if(!temp.compare("E"))
        cout << "\t\t正在等待对方的同意" << endl;
    else
        cout << "\t\t好友申请发送失败" << endl;
    put.stdexit();
}

void Friend_Srv_Delete()
{
    int type = result1(Friend_Delete);
    if(type==1)
       cout << "\t\t好友删除成功" << endl;
    else if(type==-1)
       cout << "\t\t请不要拿自己的名字开玩笑" << endl;
    else
       cout << "\t\t查无此人" << endl;
    put.stdexit();
}

void Friend_Srv_Black()
{
    int type = result1(Friend_Black);
    if(type==1)
       cout << "\t\t成功加入黑名单" << endl;
    else if(type==-1)
       cout << "\t\t请不要拿自己的名字开玩笑" << endl;
    else
       cout << "\t\t加入黑名单失败" << endl;
    put.stdexit();
}

void Friend_Srv_NoBlack()
{
    int type = result1(Friend_NoBlack);
    if(type==1)
       cout << "\t\t成功解除拉黑" << endl;
    else if(type==-1)
       cout << "\t\t请不要拿自己的名字开玩笑" << endl;
    else
       cout << "\t\t解除拉黑失败" << endl;
    put.stdexit();
}
