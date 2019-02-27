#include <iostream>
#include "head.h"

using namespace std;

void mainMenu(short MM[], slot *block);
int *splitAddress(int address);
void displayCache(short MM[], slot *block);
void writeBack(short MM[], slot *block);
void displayMM(short MM[]);

void readByte(short MM[], slot *block){
    cout<<"Please input the address to read:"<<endl;
    int address,*newAddress,j;
    bool ifHit = false;
    int startAddress;
    cin>>hex>>address;
    newAddress = splitAddress(address);
    //Judge if address in the cache
    for(int i = 0; i < 0x10; i++){
        if(block[i].slotNum == newAddress[1]){
            if(block[i].tag == newAddress[0] && block[i].v == true){
                ifHit = true;
            }
            else{
                ifHit = false;
            }
            j = i;//j is the slot number we are looking for
            break;
        }
    }
    //Get the exact block offset we are looking for
    block[j].blockOff = newAddress[2];


    if(ifHit == true){//Cache hit, use the offset as index to display the data
        cout<<"At that byte there is the value "<<hex<<block[j].data[block[j].blockOff]<<"(Cache Hit)"<<endl;
    }else{//Cache miss
        for(int i = 0; i < 0x10; i++){//copy the entire block into cache
            startAddress = address & 0xFFFFFFF0;
            block[j].data[i] = MM[startAddress + i];
        }
        if(block[j].dirty == true){
            //update MM
            MM[block[j].preAdd] = block[j].preData;
            block[j].dirty = false;
            //block[j].preAdd = 0;
            //block[j].preData = 0;
        }//update tag and valid bit
        block[j].tag = newAddress[0];
        block[j].v = true;
        cout<<"At that byte there is the value "<<hex<<block[j].data[block[j].blockOff]<<"(Cache Miss)"<<endl;
    }
    mainMenu(MM, block);
}

void writeBack(short MM[], slot *block){
    cout<<"What address would you like to write to?"<<endl;
    int writeAdd, writeData, *newAddress,j,startAddress;
    bool ifHit = false;
    cin>>hex>>writeAdd;
    cout<<"What data would you like to write at that address?"<<endl;
    cin>>hex>>writeData;
    newAddress = splitAddress(writeAdd);

    for(int i = 0; i < 0x10; i++){
        if(block[i].slotNum == newAddress[1]){
            if(block[i].tag == newAddress[0] && block[i].v == true){
                ifHit = true;
            }
            else{
                ifHit = false;
            }
            j = i;
            break;
        }
    }
    //Get the offset
    block[j].blockOff = newAddress[2];

    if(ifHit == true){
        //write data into cache
        block[j].data[block[j].blockOff] = writeData;
        //store the updated data
        block[j].preAdd = writeAdd;
        block[j].preData = writeData;
        cout<<"Value "<<hex<<writeData<<" has been written to address "<<writeAdd<<"(Cache Hit)"<<endl;
    }else{//copy the entire block into cache
        for(int i = 0; i < 0x10; i++){
            startAddress = writeAdd & 0xFFFFFFF0;
            block[j].data[i] = MM[startAddress + i];
        }
        //Set the new data into cache
        block[j].data[block[j].blockOff] = writeData;
        if(block[j].dirty == true){
            //update MM
            MM[block[j].preAdd] = block[j].preData;
        }
        block[j].preAdd = writeAdd;
        block[j].preData = writeData;
        block[j].tag = newAddress[0];
        block[j].v = true;
        cout<<"Value "<<hex<<writeData<<" has been written to address "<<writeAdd<<"(Cache Miss)"<<endl;
    }
    block[j].dirty = true;
    mainMenu(MM, block);
}

void displayCache(short MM[], slot *block){
    cout<<"Slot\t"<<"Valid\t"<<"Tag\t"<<"Dirty\t\t"<<"Data"<<endl;
    for(int i = 0; i < 0x10; i++){
        cout<<hex<<block[i].slotNum<<"\t"<<block[i].v<<"\t"<<block[i].tag<<"\t"<<block[i].dirty<<"\t\t";
        for(int j = 0; j < 0x10; j++){
            cout<<block[i].data[j]<<"  ";
        }
        cout<<endl;
    }
    mainMenu(MM, block);
}

void mainMenu(short MM[],slot *block){
    cout<<"Please choose from blow:"<<endl;
    cout<<"R)ead byte   W)rite byte   D)isplay cache   M)ain Memory  0)Exit"<<endl;
    char choice;
    cin>>choice;
    switch(choice){
        case 'r':
        case 'R':
            readByte(MM, block);
            break;
        case 'w':
        case 'W':
            writeBack(MM, block);
            break;
        case 'd':
        case 'D':
            displayCache(MM, block);
            break;
        case 'm':
        case 'M':
            displayMM(MM);
            break;
        case 0:
            exit(0);
            break;
        default:
            cout<<"Input invalid!"<<endl;
            mainMenu(MM, block);
            break;
    }
}

//Function to split address
int *splitAddress(int address){
    int *temp = new int[3];
    //Bitwise AND
    int blockOff,slotNum,tag;
    blockOff = address & 0x0000000F;
    slotNum = (address & 0x000000F0) >> 4;
    tag = address >> 8;
    temp[0] = tag;
    temp[1] = slotNum;
    temp[2] = blockOff;
    return temp;
}

//Function to print main memory
void displayMM(short MM[]){
    for(int i = 0; i<0x7FF; i++){
        cout<<hex<<"MM[ "<<i<<" ] = "<<MM[i]<<endl;
    }
}
