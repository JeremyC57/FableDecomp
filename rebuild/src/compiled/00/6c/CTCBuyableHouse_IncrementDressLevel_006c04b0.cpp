#include "engine/CTCBuyableHouse.h"

struct CTCBuyableHouse_Methods : CTCBuyableHouse {
    void SetDressLevel(int lvl);
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* house)
{
    ((CTCBuyableHouse_Methods*)house)->SetDressLevel(house->CurrentDressLevel + 1);
    house->FrameToNextCheckIfWeNeedToCreateAnything = CWorld::GetFrame();
}
