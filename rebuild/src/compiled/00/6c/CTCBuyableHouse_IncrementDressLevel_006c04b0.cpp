#include "engine/CTCBuyableHouse.h"

struct CTCBuyableHouse_Methods : CTCBuyableHouse {
    void SetDressLevel(int lvl);
};

extern int GetNewThing();

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* house)
{
    ((CTCBuyableHouse_Methods*)house)->SetDressLevel(house->CurrentDressLevel + 1);
    house->FrameToNextCheckIfWeNeedToCreateAnything = GetNewThing();
}
