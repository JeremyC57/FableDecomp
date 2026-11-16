#pragma optimize("s",on)
// CTCHeroSpecialMovement::Alloc(CThing& thing) -- the registry factory: a new CTCHeroSpecialMovement for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E08E0), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroSpecialMovement@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5D23, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroSpecialMovement::CTCHeroSpecialMovement ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHeroSpecialMovement@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroSpecialMovement@@QAE@AAVCThing@@@Z (0x007E08E0, landed). (Landed earlier as a generated free function CTCHeroSpecialMovement_Alloc(CThing*) returning CTCHeroSpecialMovement*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroSpecialMovement : public CTCBase {
public:
    CTCHeroSpecialMovement(CThing& thing);                  // 0x007E08E0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroSpecialMovement) = 0x1c
};

CTCBase* __fastcall CTCHeroSpecialMovement::Alloc(CThing& thing)
{
    return new CTCHeroSpecialMovement(thing);
}
