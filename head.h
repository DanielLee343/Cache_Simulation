#ifndef PROJECT2_H
#define PROJECT2_H

struct slot
{
    bool v;
    bool dirty;
    int tag;
    int slotNum;
    int blockOff;
    int data[16];
    int preAdd;
    int preData;
};

#endif
