#pragma optimize("s",on)
// CTCHeroGuide::Alloc(CThing& thing) -- the registry factory: a new CTCHeroGuide for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x00780070), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroGuide@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4670, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroGuide::CTCHeroGuide ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHeroGuide@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroGuide@@QAE@AAVCThing@@@Z (0x00780070, landed). (Landed earlier as a generated free function CTCHeroGuide_Alloc(CThing*) returning CTCHeroGuide*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroGuide : public CTCBase {
public:
    CTCHeroGuide(CThing& thing);                        // 0x00780070
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroGuide) = 0x10
};

CTCBase* __fastcall CTCHeroGuide::Alloc(CThing& thing)
{
    return new CTCHeroGuide(thing);
}
