#pragma optimize("s",on)
// CTCCreatureTintEffect::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureTintEffect for `thing` (operator new(0x40) 0x00BFEA1A, null-checked, then
//   the constructor 0x007CF110), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureTintEffect@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5926, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x40 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureTintEffect::CTCCreatureTintEffect ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureTintEffect@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureTintEffect@@QAE@AAVCThing@@@Z (0x007CF110, landed). (Landed earlier as a generated free function CTCCreatureTintEffect_Alloc(CThing*) returning CTCCreatureTintEffect*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureTintEffect : public CTCBase {
public:
    CTCCreatureTintEffect(CThing& thing);                        // 0x007CF110
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x40 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureTintEffect) = 0x40
};

CTCBase* __fastcall CTCCreatureTintEffect::Alloc(CThing& thing)
{
    return new CTCCreatureTintEffect(thing);
}
