
#ifndef LURK_H
#define LURK_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdint>
#include <pthread.h>
#include <string>
class LURK
{
private:
    pthread_t listen;
    struct sockaddr_in sad;
    struct hostent* entry;
    struct in_addr **addr_list;
    struct in_addr* c_addr;
    int skt;
public:
    
    LURK();
    bool connected=false;
    struct pkg{
         __uint8_t type;
    };

    struct charPkg:public pkg{

      char name[32]={0};
      __uint8_t flags;
      __uint16_t atk;
      __uint16_t def;
      __uint16_t reg;
      signed short health;
      __uint16_t gold;
      __uint16_t room;
      std::string description;
      inline charPkg(__int16_t t,char n[32],__uint8_t f, __uint16_t a, __uint16_t d, __uint16_t r, signed short h, __uint16_t g, std::string desc){type=t;for(int i=0;i<32;i++)name[i]=n[i];flags=f;atk=a;def=d;reg=r;health=h;gold=g;description=desc;}
      inline charPkg(){}
    };
    struct msgPkg:public pkg{
        short len;
        char recName[32];
        char sender[32];
        std::string msg;
    };
    struct errorPkg:public pkg{
        __uint8_t code;
        std::string msg;

    };
    struct roomPkg:public pkg{
        __uint16_t number;
        char name[32];
        std::string description;
    };
    struct gamePkg:public pkg{
        __uint16_t init;
        __uint16_t statMax;
        std::string description;
    };

    pkg* cPak;
    pkg nulPak;
    int connect(std::string ip, std::string port);
    LURK& disconnect();
    LURK& receive();
    bool msg(std::string message);
    LURK& chngrm(int num);
    LURK& fight();
    LURK& pvp(std::string target);
    LURK& loot(std::string target);
    LURK& start();
    bool character(char name[32], __uint16_t atk, __uint16_t def, __uint16_t regen, bool joinB, std::string dec);
    bool character(charPkg& inPak);
    LURK& leave();
    LURK& startReader();
    LURK& stop();
    static void* reader(void* param);




};
extern LURK lurk;
#endif // LURK_H
