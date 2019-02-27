#include <iostream>
#include "head.h"
using namespace std;

void readByte(short MM[], slot *block);
void mainMenu(short MM[], slot *block);
void displayCache(short MM[], slot *block);
void writeBack(short MM[], slot *block);
void displayMM(short MM[]);

int main()
{
    short MM[2048];
    slot block[16];
    int j = 0;

    //Initializing MM
    for(int i = 0;i <= 0x7FF; i++){
        MM[i] = j;
        j++;
        if(j > 255){
            j = 0;
        }
    }
    //Initializing cache
    for(int i = 0; i < 0x10; i++){
        block[i].v = false;
        block[i].tag = 0;
        block[i].slotNum = i;
        block[i].dirty = false;
        block[i].preAdd = 0;
        block[i].preData = 0;
        for(int j = 0; j < 0x10; j++){
            block[i].data[j] = 0;
        }
    }
    mainMenu(MM, block);
    return 0;
}
