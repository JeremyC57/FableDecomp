#pragma optimize("s",on)
// CTCHero::Alloc(CThing& thing) -- the registry factory: a new CTCHero for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x006C9FE0), returned as its CTCBase
// Leaf: ?Alloc@CTCHero@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3468, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHero::CTCHero ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHero@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHero@@QAE@AAVCThing@@@Z (0x006C9FE0, landed). (Landed earlier as a generated free function CTCHero_Alloc(CThing*) returning CTCHero*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHero : public CTCBase {
public:
    CTCHero(CThing& thing);                        // 0x006C9FE0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCHero) = 0x38
};

CTCBase* __fastcall CTCHero::Alloc(CThing& thing)
{
    return new CTCHero(thing);
}
