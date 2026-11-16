#pragma optimize("s",on)
// CTCHeroMorph::Alloc(CThing& thing) -- the registry factory: a new CTCHeroMorph for `thing` (operator new(0x64) 0x00BFEA1A, null-checked, then
//   the constructor 0x0071E180), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroMorph@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D28BB, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x64 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroMorph::CTCHeroMorph ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHeroMorph@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroMorph@@QAE@AAVCThing@@@Z (0x0071E180, landed). (Landed earlier as a generated free function CTCHeroMorph_Alloc(CThing*) returning CTCHeroMorph*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroMorph : public CTCBase {
public:
    CTCHeroMorph(CThing& thing);                        // 0x0071E180
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x64 - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroMorph) = 0x64
};

CTCBase* __fastcall CTCHeroMorph::Alloc(CThing& thing)
{
    return new CTCHeroMorph(thing);
}
