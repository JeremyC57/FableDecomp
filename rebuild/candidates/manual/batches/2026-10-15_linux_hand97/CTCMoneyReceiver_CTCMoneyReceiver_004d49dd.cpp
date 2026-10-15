// CTCMoneyReceiver::CTCMoneyReceiver(CThing*) -- CTCBase(owner), the class vtable, AmountOfMoneyWanted from the default getter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D49DD, length 32 bytes (built by Create_CTCMoneyReceiver @ 0x004D87DF).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; the default-amount getter @ 0x007898F0 (`mov eax,32h ; ret`, a
// static with no `this`: nothing is loaded into ecx before the call).
// Shape: the size-optimised constructor module (`push dword ptr [esp+8]` passes the argument straight through);
// the vtable store follows the base constructor, the member is assigned from the call result.
#pragma optimize("s", on)
struct CThing;

struct CTCBase_004d49dd {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];                        // +0x08 ToBeRemoved, IsInGlobalUpdateTCs
    CTCBase_004d49dd(CThing* owner);                // 0x00686800
};

struct CTCMoneyReceiver : CTCBase_004d49dd {        // 0x10 bytes
    long AmountOfMoneyWanted;                       // +0x0c
    static long GetDefaultAmountOfMoneyWanted();    // 0x007898F0 (returns 50)
    CTCMoneyReceiver(CThing* owner);
    virtual void v00();
};

CTCMoneyReceiver::CTCMoneyReceiver(CThing* owner)
    : CTCBase_004d49dd(owner)
{
    AmountOfMoneyWanted = GetDefaultAmountOfMoneyWanted();
}
