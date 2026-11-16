#pragma optimize("s",on)
// CTCBurntEffect::Alloc(CThing& thing) -- the registry factory: a new CTCBurntEffect for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x007CDFE0), returned as its CTCBase
// Leaf: ?Alloc@CTCBurntEffect@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D58C6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBurntEffect::CTCBurntEffect ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBurntEffect@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBurntEffect@@QAE@AAVCThing@@@Z (0x007CDFE0, landed). (Landed earlier as a generated free function CTCBurntEffect_Alloc(CThing*) returning CTCBurntEffect*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBurntEffect : public CTCBase {
public:
    CTCBurntEffect(CThing& thing);                        // 0x007CDFE0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCBurntEffect) = 0x38
};

CTCBase* __fastcall CTCBurntEffect::Alloc(CThing& thing)
{
    return new CTCBurntEffect(thing);
}
