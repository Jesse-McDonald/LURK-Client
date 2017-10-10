
#include "LURK.h"
#include <exception>
#include <stdexcept>
LURK::LURK(){
    this->nulPak.type=0;
    cPak=&nulPak;
}

int LURK::connect(std::string ip,std::string port){//probiably finished
    sad.sin_port=htons(atoi(port.c_str()));
    sad.sin_family=AF_INET;

    skt=socket(AF_INET,SOCK_STREAM,0);

    entry = gethostbyname(ip.c_str());
    if(!entry) {
        throw std::runtime_error("Get host by name -> null");
    }
    addr_list = (struct in_addr**)entry->h_addr_list;
    c_addr = addr_list[0];
    sad.sin_addr=*c_addr;
    return ::connect(skt, (struct sockaddr*)&sad, sizeof(struct sockaddr_in));


}
LURK& LURK::LURK::disconnect(){//finished
    write(skt,(const void*)12,1);
    close(skt);
    return *this;
}
LURK& LURK::receive(){//change to return LURK* and set class level pkg *currentPack
    unsigned char type;
    read(skt,&type,1);
    pkg* ret;
    switch (type){
    case 1:{//done
        msgPkg *temp=new msgPkg();
        temp->type=type;
        read(skt,&(temp->len),2);
        read(skt,&(temp->recName),32);
        read(skt,&(temp->sender),32);
        read(skt,&(temp->msg),temp->len);

        ret=temp;
        }break;
    case 7:{//done
        errorPkg *temp=new errorPkg();
        temp->type=type;
        read(skt,&(temp->code),1);
        unsigned short len;
        read(skt,&len,2);
        read(skt,&(temp->msg),len);

        ret=temp;
        }break;
    case 8://done
        ret=new pkg();
        ret->type=type;
        break;
    case 9:{//done
        roomPkg *temp=new roomPkg();
        temp->type=type;
        read(skt,&(temp->number),2);
        read(skt,&(temp->name),32);
        unsigned short len;
        read(skt,&len,2);
        read(skt,&(temp->description),len);

        ret=temp;
        }break;
    case 10:{//done
        charPkg *temp=new charPkg();
        temp->type=type;
        read(skt,&(temp->name),32);
        read(skt,&(temp->atk),2);
        read(skt,&(temp->def),2);
        read(skt,&(temp->reg),2);
        read(skt,&(temp->health),2);
        read(skt,&(temp->gold),2);
        read(skt,&(temp->room),2);
        unsigned short len;
        read(skt,&len,2);
        read(skt,&(temp->description),len);

        ret=temp;
        }break;
    case 11:{//done
        gamePkg *temp=new gamePkg();
        temp->type=type;
        read(skt,&(temp->init),2);
        read(skt,&(temp->statMax),2);
        unsigned short len;
        read(skt,&len,2);
        read(skt,&(temp->description),len);

        ret=temp;
        }break;
    default:{//done
        ret=new pkg();
        ret->type=type;
        }
    }
    this->cPak=ret;
    return *this;
}
LURK& LURK::startReader(){
    pthread_create(&(this->listen),NULL, LURK::reader,(void*)this);
}
LURK& LURK::stop(){
    pthread_cancel(listen);
}
void* LURK::reader(void* param){
    LURK* self=(LURK*) param;
    while(true){
       self->receive();
    }
    return param;
}
bool LURK::msg(std::string message){
    bool ret;

    return ret;
}
LURK& LURK::chngrm(int num){

    return *this;
}
LURK& LURK::fight(){

    return *this;
}
LURK& LURK::pvp(std::string target){

    return *this;
}
LURK& LURK::loot(std::string target){

    return *this;
}
LURK& LURK::start(){

    return *this;
}
bool LURK::character(char name[32], __uint16_t atk, __uint16_t def, __uint16_t regen, bool joinB, std::string dec){

    charPkg pak={.type=1,.name=name,.flags=joinB<<7,.atk=atk,.def=def,.reg=regen,.health=0,.gold=0,.description=dec};
    return character(pak);
}
bool LURK::character(charPkg& inPak){
    bool ret;

    return ret;
}
LURK& LURK::leave(){

    return *this;
}
