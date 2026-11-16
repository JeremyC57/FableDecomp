#pragma optimize("s",on)
// CTCCreatureFlyInCircles::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureFlyInCircles for `thing` (operator new(0x78) 0x00BFEA1A, null-checked, then
//   the constructor 0x007DDF90), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureFlyInCircles@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5BA1, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x78 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureFlyInCircles::CTCCreatureFlyInCircles ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureFlyInCircles@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureFlyInCircles@@QAE@AAVCThing@@@Z (0x007DDF90, landed). (Landed earlier as a generated free function CTCCreatureFlyInCircles_Alloc(CThing*) returning CTCCreatureFlyInCircles*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureFlyInCircles : public CTCBase {
public:
    CTCCreatureFlyInCircles(CThing& thing);                        // 0x007DDF90
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x78 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureFlyInCircles) = 0x78
};

CTCBase* __fastcall CTCCreatureFlyInCircles::Alloc(CThing& thing)
{
    return new CTCCreatureFlyInCircles(thing);
}
