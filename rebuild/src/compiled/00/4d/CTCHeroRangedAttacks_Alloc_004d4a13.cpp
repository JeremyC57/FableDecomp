#pragma optimize("s",on)
// CTCHeroRangedAttacks::Alloc(CThing& thing) -- the registry factory: a new CTCHeroRangedAttacks for `thing` (operator new(0x4c) 0x00BFEA1A, null-checked, then
//   the constructor 0x00789BB0), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroRangedAttacks@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4A13, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x4c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroRangedAttacks::CTCHeroRangedAttacks ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCHeroRangedAttacks@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroRangedAttacks@@QAE@AAVCThing@@@Z (0x00789BB0, landed). (Landed earlier as a generated free function CTCHeroRangedAttacks_Alloc(CThing*) returning CTCHeroRangedAttacks*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroRangedAttacks : public CTCBase {
public:
    CTCHeroRangedAttacks(CThing& thing);                        // 0x00789BB0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x4c - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroRangedAttacks) = 0x4c
};

CTCBase* __fastcall CTCHeroRangedAttacks::Alloc(CThing& thing)
{
    return new CTCHeroRangedAttacks(thing);
}
