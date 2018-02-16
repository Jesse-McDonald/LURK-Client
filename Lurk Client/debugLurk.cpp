#include "LURK.h"
#include <iostream>
LURK lurk=LURK();
int main(){
lurk.connect(localhost,5070);
while(true){
	lurk.recieve();
}

}
