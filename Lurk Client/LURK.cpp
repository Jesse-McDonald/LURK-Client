#include <iostream>
#include "LURK.h"
#include <exception>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstring>

LURK::LURK(){
    pkg *ePak=new pkg;
    ePak->type=0;
    this->cPak.reset(ePak);

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
bool LURK::timeout(int type,int mills){//time out is being depercated
    //use of timeout is discoraged as it only checks the last recieved packet
    //while this is ok for some applicatins, this function often misses packets
    //if a long stream are sent at the same time
    for(int i=0;i<mills;i++){
        if(lurk.cPak&&lurk.cPak.get()->type==type){
            return true;
        }
         std::this_thread::sleep_for (std::chrono::milliseconds(1));

    }
    return false;
}
LURK& LURK::LURK::disconnect(){//finished
    write(skt,(const void*)12,1);
    close(skt);
    return *this;
}
LURK& LURK::lock(){
    pthread_mutex_lock(&(this->readLock));
    return *this;
}
LURK& LURK::unlock(){
    pthread_mutex_unlock(&(this->readLock));
    return *this;
}
LURK& LURK::receive(){//change to return LURK* and set class level pkg *currentPack

    unsigned char type;
    pkg* ret;
    try{
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        //std::cout<<"startning read loop"<<std::endl;
        read(skt,&type,1);
        //std::cout<<"type recieved: "<<(int)type<<std::endl;
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        switch (type){
        case 1:{//done
            //std::cout<<"Reading Message"<<std::endl;
            msgPkg *temp=new msgPkg();
            temp->type=type;
            unsigned short len;
            //std::cout<<"Reading len"<<std::endl;
            read(skt,&(len),2);
            //std::cout<<"len is: "<<len <<std::endl;
            //std::cout<<"Reading rec name"<<std::endl;
            read(skt,&(temp->recName),32);
            //std::cout<<"rec name is: "<<temp->recName <<std::endl;
            //std::cout<<"Reading sender"<<std::endl;
            read(skt,&(temp->sender),32);
            //std::cout<<"sender is: "<<temp->sender <<std::endl;
            //std::cout<<"Reading message"<<std::endl;
            char * descBuffer = new char[len+1];
            read(skt,descBuffer,len);
            //std::cout<<"message is: "<<descBuffer <<std::endl;
            temp->msg = std::string(descBuffer, len);
            delete[] descBuffer;
            ret=temp;
            }break;
        case 7:{//done
            //std::cout<<"Reading error"<<std::endl;
            errorPkg *temp=new errorPkg();
            temp->type=type;
            //std::cout<<"Reading code"<<std::endl;
            read(skt,&(temp->code),1);
            //std::cout<<"code is: "<<temp->code <<std::endl;
            unsigned short len;
            //std::cout<<"Reading len"<<std::endl;
            read(skt,&len,2);
            //std::cout<<"len is: "<<len <<std::endl;
            char * descBuffer = new char[len+1];
            //std::cout<<"Reading message"<<std::endl;
            read(skt,descBuffer,len);
            //std::cout<<"message is: "<<descBuffer <<std::endl;
            temp->msg = std::string(descBuffer, len);
            delete[] descBuffer;

            ret=temp;
            }break;
        case 8://done
            //std::cout<<"Reading Accept"<<std::endl;
            ret=new pkg();
            char disc;
            //std::cout<<"Reading type"<<std::endl;
            read(skt,&disc,1);//throw away extra
            //std::cout<<"type is: "<<disc<<std::endl;
            ret->type=type;
            break;
        case 9:{//done
            //std::cout<<"Reading Room"<<std::endl;
            roomPkg *temp=new roomPkg();
            temp->type=type;
            //std::cout<<"Reading number"<<std::endl;
            read(skt,&(temp->number),2);
            //std::cout<<"number is: "<<temp->number <<std::endl;
            //std::cout<<"Reading name"<<std::endl;
            read(skt,&(temp->name),32);
            //std::cout<<"name is: "<<temp->name <<std::endl;
            //std::cout<<"Reading lenght"<<std::endl;
            unsigned short len;

            read(skt,&len,2);
            //std::cout<<"len is: "<<len <<std::endl;
            char * descBuffer = new char[len+1];
            //std::cout<<"Reading desc"<<std::endl;
            read(skt,descBuffer,len);
            //std::cout<<"desc is: "<<descBuffer <<std::endl;
            temp->description = std::string(descBuffer, len);
            delete[] descBuffer;

            ret=temp;
            }break;
        case 10:{//done
            //std::cout<<"Reading character"<<std::endl;
            charPkg *temp=new charPkg();
            temp->type=type;
            //std::cout<<"Reading name"<<std::endl;
            read(skt,&(temp->name),32);
            //std::cout<<"name is: "<<temp->name<<std::endl;
            //std::cout<<"reading flags"<<std::endl;
            char b;
            read(skt,&b,1);
            temp->flags=std::bitset<8>(b);
            //std::cout<<"flags are: "<<temp->flags<<std::endl;

            //std::cout<<"Reading atk"<<std::endl;
            read(skt,&(temp->atk),2);
            //std::cout<<"atk is: "<<temp->atk<<std::endl;
            //std::cout<<"Reading def"<<std::endl;
            read(skt,&(temp->def),2);
            //std::cout<<"Def is: "<<temp->def<<std::endl;
            //std::cout<<"Reading Reg"<<std::endl;
            read(skt,&(temp->reg),2);
            //std::cout<<"Reg is: "<<temp->reg<<std::endl;
            //std::cout<<"Reading health"<<std::endl;
            read(skt,&(temp->health),2);
            //std::cout<<"health is: "<<temp->health<<std::endl;
            //std::cout<<"Reading gold"<<std::endl;
            read(skt,&(temp->gold),2);
            //std::cout<<"gold is"<<temp->gold<<std::endl;
            //std::cout<<"Reading room"<<std::endl;
            read(skt,&(temp->room),2);
            //std::cout<<"room is: "<<temp->room<<std::endl;
            //std::cout<<"Reading len"<<std::endl;
            unsigned short len;
            read(skt,&len,2);
            //std::cout<<"len is: "<<len<<std::endl;
            char * descBuffer = new char[len+1];
            //std::cout<<"Reading description"<<std::endl;
            read(skt,descBuffer,len);
            temp->description = std::string(descBuffer, len);
            //std::cout<<"Description is: "<<descBuffer<<std::endl;
            delete[] descBuffer;

            ret=temp;
            }break;
        case 11:{//done
            //std::cout<<"Reading game"<<std::endl;
            gamePkg *temp=new gamePkg();
            temp->type=type;
            //std::cout<<"Reading init"<<std::endl;
            read(skt,&(temp->init),2);
            //std::cout<<"init is: "<<temp->init <<std::endl;
            //std::cout<<"Reading max"<<std::endl;
            read(skt,&(temp->statMax),2);
            //std::cout<<"max is: "<<temp->statMax <<std::endl;
            unsigned short len;
            //std::cout<<"Reading len "<<std::endl;
            read(skt,&len,2);
            char * descBuffer = new char[len+1];
            //std::cout<<"len is: "<<len <<std::endl;
            //std::cout<<"Reading desc"<<std::endl;
            read(skt,descBuffer,len);
            //std::cout<<"desc is: "<<descBuffer <<std::endl;
            temp->description = std::string(descBuffer, len);
            delete[] descBuffer;
            ret=temp;

            }break;
        case 13:{//done
            roomPkg *temp=new roomPkg();
            //std::cout<<"Reading connection"<<std::endl;
            temp->type=type;
            //std::cout<<"Reading number"<<std::endl;
            read(skt,&(temp->number),2);
            //std::cout<<"number is: "<<temp->number <<std::endl;
            //std::cout<<"Reading name"<<std::endl;
            read(skt,&(temp->name),32);
            //std::cout<<"name is: "<<temp->name <<std::endl;
            unsigned short len;
            //std::cout<<"Reading len"<<std::endl;
            read(skt,&len,2);
            //std::cout<<"len is: "<<len <<std::endl;
            char * descBuffer = new char[len+1];
            //std::cout<<"Reading desc"<<std::endl;
            read(skt,descBuffer,len);
            //std::cout<<"desc is: "<<descBuffer <<std::endl;
            temp->description = std::string(descBuffer, len);
            delete[] descBuffer;

            ret=temp;
            }break;
        default:{//done
            ret=new pkg();
            ret->type=type;
            }
        }
    }catch(...){
        ret=new pkg();
        ret->type=12;
    };
    //pthread_mutex_lock(&(this->readLock));
    //if(this->cPak!=&(this->nulPak)){
    //    delete cPak;
    //    cPak=&nulPak;
    //}
    //pthread_mutex_unlock(&(this->readLock));
    this->lock();
    this->cPak.reset(ret);
    this->pks.push(cPak);
    this->unlock();
    return *this;
}
LURK& LURK::startReader(){
    this->closeThread=false;
    pthread_create(&(this->listen),NULL, LURK::reader,(void*)this);
    return *this;
}
LURK& LURK::stop(){
    this->closeThread=true;
    pthread_join(listen,0);
    return *this;
}
void* LURK::reader(void* param){
    LURK* self=(LURK*) param;
    while(!(self->closeThread)){
       self->receive();
    }
    return param;
}
LURK& LURK::msg(std::string target, std::string message){
    char type=1;
    write(skt,&type,1);
    __uint16_t len= message.length();
    write(skt,&len,2);
    const char *name=target.c_str();
    write(skt,name,32);
    //std::cout<<name;
    write(skt,this->currentChar.name,32);
    const char *mesg=message.c_str();
    write(skt,mesg,len);
    return *this;
}
LURK& LURK::msg(std::string message){
    this->msg("",message);
    return *this;
}
LURK& LURK::chngrm(__uint16_t num){
    char type=2;
    write(skt,&type,1);
    write(skt,&num,2);
    return *this;
}
LURK& LURK::fight(){
    char type=3;
    write(skt,&type,1);
    return *this;
}
LURK& LURK::pvp(std::string target){
    char type=4;
    write(skt,&type,1);
    const char *name=target.c_str();
    write(skt,name,32);
    return *this;
}
LURK& LURK::loot(std::string target){
    char type=5;
    write(skt,&type,1);
    const char *name=target.c_str();
    write(skt,name,32);
    return *this;
}
LURK& LURK::start(){
    char d=6;
    connected=true;
    write(skt,&d,1);
    return *this;
}

LURK& LURK::character(charPkg& inPak){
    write(skt, &(inPak.type), 1);
    write(skt, &(inPak.name), 32);
    char d=(char)inPak.flags.to_ulong();
    //void * f=(void *)&inPak.flags;
    //char *d=(char*)f;
    //write(skt,&(inPak.flags),1);
    write(skt, &d,1);

    write(skt, &(inPak.atk),2);
    write(skt, &(inPak.def),2);
    write(skt, &(inPak.reg),2);
    write(skt, &(inPak.health),2);
    write(skt, &(inPak.gold),2);
    write(skt, &(inPak.room),2);
    int len=inPak.description.length();
    write(skt, &len, 2);
    write(skt, (inPak.description.c_str()),len);
    return *this;
}
LURK& LURK::leave(){
    int d=12;
    write(skt,&d,1);
    connected=false;
    serverIp="";
    serverPort="";
    gameData.type=0;
    close(skt);
    stop();
    return *this;
}
int LURK::checkPlayer(LURK::charPkg check){
    for (int i=0;i<this->players.size();i++){
        if(!strcmp(players[i].name,check.name)){
           return i;
    }
    }
    return -1;
}
int LURK::checkMonster(LURK::charPkg check){
    for (int i=0;i<this->monsters.size();i++){
        if(!strcmp(monsters[i].name,check.name)){
           return i;
    }
    }
    return -1;
}
