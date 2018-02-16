
#ifndef LURK_H
#define LURK_H
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdint>
#include <pthread.h>
#include <string>
#include <memory>
#include <queue>
#include <bitset>

using std::vector;
class LURK
{
private:
    pthread_t listen;
    struct sockaddr_in sad;
    struct hostent* entry;
    struct in_addr **addr_list;
    struct in_addr* c_addr;
    int skt;
    bool closeThread=false;
public:
    LURK();
    bool connected=false;
    struct pkg{
        pkg(){type=0;}
         __uint8_t type;
	virtual ~pkg(){}
    };

    struct charPkg:public pkg{

      char name[32]={0};
      std::bitset<8> flags;
      __uint16_t atk;
      __uint16_t def;
      __uint16_t reg;
      signed short health;
      __uint16_t gold;
      __uint16_t room;
      std::string description;
      inline charPkg(__int16_t t,char n[32],__uint8_t f, __uint16_t a, __uint16_t d, __uint16_t r, signed short h, __uint16_t g, std::string desc){type=t;for(int i=0;i<32;i++)name[i]=n[i];flags=f;atk=a;def=d;reg=r;health=h;gold=g;description=desc;}
      inline charPkg(){}
      charPkg(const charPkg &chr){
          this->atk=chr.atk;
          this->def=chr.def;
          this->description=chr.description;
          this->flags=chr.flags;
          this->gold=chr.gold;
          this->health=chr.health;
          for (int i=0;i<32;i++){
              this->name[i]=chr.name[i];
          }
          this->reg=chr.reg;
          this->room=chr.room;
      }
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
        roomPkg(){}
        roomPkg(const roomPkg &room){
            this->description=room.description;
            this->type=room.type;
            this->number=room.number;
            for (int i=0;i<32;i++){
                this->name[i]=room.name[i];
            }
        }
        __uint16_t number;
        char name[32];
        std::string description;
    };
    struct gamePkg:public pkg{
        __uint16_t init;
        __uint16_t statMax;
        std::string description="UNDEF";
    };
    vector<roomPkg> connectedRooms;
    roomPkg currentRoom;
    vector<charPkg> players;
    vector<charPkg> monsters;
    vector<charPkg> lootables;
    charPkg currentChar;
    pkg nulPak;
    gamePkg game;
    std::shared_ptr<pkg> cPak;
    std::queue<std::shared_ptr<pkg>> pks;
    pthread_mutex_t readLock = PTHREAD_MUTEX_INITIALIZER;
    std::string serverIp;
    std::string serverPort;
    gamePkg gameData;
    int connect(std::string ip, std::string port);
    LURK& disconnect();
    LURK& receive();
    bool timeout(int type,int mills);
    LURK& lock();
    LURK& unlock();
    LURK& msg(std::string target, std::string message);
    LURK& msg(std::string message);
    LURK& chngrm(__uint16_t num);
    LURK& fight();
    LURK& pvp(std::string target);
    LURK& loot(std::string target);
    LURK& start();
    LURK& character(char name[32], __uint16_t atk, __uint16_t def, __uint16_t regen, bool joinB, std::string dec);
    LURK& character(charPkg& inPak);
    LURK& leave();
    LURK& startReader();
    LURK& stop();
    int checkPlayer(charPkg check);
    int checkMonster(charPkg check);
    static void* reader(void* param);




};
extern LURK lurk;
#endif // LURK_H
