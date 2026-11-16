#pragma optimize("s",on)
// CTCMoneyReceiver::Alloc(CThing& thing) -- the registry factory: a new CTCMoneyReceiver for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D49DD), returned as its CTCBase
// Leaf: ?Alloc@CTCMoneyReceiver@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D87DF, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCMoneyReceiver::CTCMoneyReceiver ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCMoneyReceiver@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCMoneyReceiver@@QAE@AAVCThing@@@Z (0x004D49DD, landed). (Landed earlier as a generated free function CTCMoneyReceiver_Alloc(CThing*) returning CTCMoneyReceiver*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCMoneyReceiver : public CTCBase {
public:
    CTCMoneyReceiver(CThing& thing);                        // 0x004D49DD
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCMoneyReceiver) = 0x10
};

CTCBase* __fastcall CTCMoneyReceiver::Alloc(CThing& thing)
{
    return new CTCMoneyReceiver(thing);
}
