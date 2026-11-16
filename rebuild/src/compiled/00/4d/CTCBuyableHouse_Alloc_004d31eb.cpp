#pragma optimize("s",on)
// CTCBuyableHouse::Alloc(CThing& thing) -- the registry factory: a new CTCBuyableHouse for `thing` (operator new(0x4c) 0x00BFEA1A, null-checked, then
//   the constructor 0x006BF8A0), returned as its CTCBase
// Leaf: ?Alloc@CTCBuyableHouse@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D31EB, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x4c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBuyableHouse::CTCBuyableHouse ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBuyableHouse@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBuyableHouse@@QAE@AAVCThing@@@Z (0x006BF8A0, landed). (Landed earlier as a generated free function CTCBuyableHouse_Alloc(CThing*) returning CTCBuyableHouse*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBuyableHouse : public CTCBase {
public:
    CTCBuyableHouse(CThing& thing);                         // 0x006BF8A0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x4c - 0xC];              // the class's own members (not modelled); sizeof(CTCBuyableHouse) = 0x4c
};

CTCBase* __fastcall CTCBuyableHouse::Alloc(CThing& thing)
{
    return new CTCBuyableHouse(thing);
}
