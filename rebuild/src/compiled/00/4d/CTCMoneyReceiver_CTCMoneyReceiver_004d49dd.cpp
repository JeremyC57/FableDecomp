// CTCMoneyReceiver::CTCMoneyReceiver(CThing&) -- CTCBase(owner), the class vtable, AmountOfMoneyWanted from the default getter
// Leaf: ??0CTCMoneyReceiver@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CTCBase_004d49dd, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D49DD, length 32 bytes (built by Create_CTCMoneyReceiver @ 0x004D87DF).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800; the default-amount getter @ 0x007898F0 (`mov eax,32h ; ret`, a
// static with no `this`: nothing is loaded into ecx before the call).
// Shape: the size-optimised constructor module (`push dword ptr [esp+8]` passes the argument straight through);
// the vtable store follows the base constructor, the member is assigned from the call result.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CTCMoneyReceiver : public CTCBase { public:        // 0x10 bytes
    long AmountOfMoneyWanted;                       // +0x0c
    static long GetDefaultAmountOfMoneyWanted();    // 0x007898F0 (returns 50)
    CTCMoneyReceiver(CThing& owner);
};

CTCMoneyReceiver::CTCMoneyReceiver(CThing& owner)
    : CTCBase(owner)
{
    AmountOfMoneyWanted = GetDefaultAmountOfMoneyWanted();
}