// CTCBuyableHouse::HasMultipleDressLevels  @ 0x006bf7f0
// Leaf: ?CTCBuyableHouse_HasMultipleDressLevels@@YI_NPAVCTCBuyableHouse@@@Z

#include "engine/CTCBuyableHouse.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefPointeeBase { public:
    char pad0[0x28];
    int* first;   // +0x28
    int* last;    // +0x2c
};


bool __fastcall CTCBuyableHouse_HasMultipleDressLevels(CTCBuyableHouse* house)
{
    CDefPointeeBase* d = house->PDef_Object;
    if (d != 0)
    {
        unsigned int count = (unsigned int)(d->last - d->first);
        if (count > 1)
            return true;
    }
    return false;
}