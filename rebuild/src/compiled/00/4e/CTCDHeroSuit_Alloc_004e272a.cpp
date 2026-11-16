#pragma optimize("s",on)
// CTCDHeroSuit::Alloc(CThing& thing) -- the registry factory: a new CTCDHeroSuit for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E079C), returned as its CTCBase
// Leaf: ?Alloc@CTCDHeroSuit@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E272A, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDHeroSuit::CTCDHeroSuit ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDHeroSuit@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDHeroSuit@@QAE@AAVCThing@@@Z (0x004E079C, landed). (Landed earlier as a generated free function CTCDHeroSuit_Alloc(CThing*) returning CTCDHeroSuit*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDHeroSuit : public CTCBase {
public:
    CTCDHeroSuit(CThing& thing);                        // 0x004E079C
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCDHeroSuit) = 0x1c
};

CTCBase* __fastcall CTCDHeroSuit::Alloc(CThing& thing)
{
    return new CTCDHeroSuit(thing);
}
