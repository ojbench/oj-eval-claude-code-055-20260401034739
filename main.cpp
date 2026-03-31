#include <iostream>
#include <algorithm>
#include "src.hpp"
using namespace std;

void obj_copy(object* ori, object*& cop){
    string type=ori->type();
    if(type=="no type")
        cop=new mail();
    else if(type=="air")
        cop=new air_mail();
    else if(type=="train")
        cop=new train_mail();
    else if(type=="car")
        cop=new car_mail();
    cop->copy(ori);
}

const int readinSize=100000;
int main(){
    ios::sync_with_stdio(false);
    int mailNum,readSize;
    cin>>mailNum;
    readSize = mailNum;
    object* list[readinSize]={nullptr};
    for(int i=0;i<readSize;i++){
        string type;
        cin>>type;
        string _contain_,_postmark_;
        date send_d,arrive_d;
        cin>>_contain_>>_postmark_>>send_d>>arrive_d;
        if(type=="air"){
            date take_off,land;
            string airline;
            cin>>take_off>>land>>airline;
            list[i]=new air_mail(_contain_,_postmark_,send_d,arrive_d,take_off,land,airline);
        }else if(type=="train"){
            int station_num;
            cin>>station_num;
            string* sname;
            date* stime;
            sname=new string[station_num];
            stime=new date[station_num];
            for(int j=0;j<station_num;j++)
                cin>>sname[j]>>stime[j];
            list[i]=new train_mail(_contain_,_postmark_,send_d,arrive_d,sname,stime,station_num);
            delete []sname;
            delete []stime;
        }else if(type=="car"){
            int mile;
            string driver;
            cin>>mile>>driver;
            list[i]=new car_mail(_contain_,_postmark_,send_d,arrive_d,mile,driver);
        }else if(type=="no"){
            list[i]=new mail(_contain_,_postmark_,send_d,arrive_d);
        }
    }
    int queryOp;
    int id;
    while(cin>>queryOp){
        cin>>id;
        if(queryOp==0){
            cout<<list[id]->type()<<endl;
        }else if(queryOp==1){
            list[id]->print();
        }else if(queryOp==2){
            int y,m,d;
            cin>>y>>m>>d;
            cout<<list[id]->send_status(y,m,d)<<endl;
        }else if(queryOp==3){
            int other;
            cin>>other;
            obj_swap(list[id],list[other]);
        }else if(queryOp==4){
            object* tmp;
            obj_copy(list[id],tmp);
            delete list[id];
            obj_copy(tmp,list[id]);
            delete tmp;
        }
    }
    for(int i=0;i<readSize;i++){
        if(list[i]!=nullptr){
            delete list[i];
        }
    }
    return 0;
}
