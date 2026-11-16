#pragma optimize("s",on)
// CTCCreatureFlyOnSpline::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureFlyOnSpline for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007DCF90), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureFlyOnSpline@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5B71, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureFlyOnSpline@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureFlyOnSpline@@QAE@AAVCThing@@@Z (0x007DCF90, landed). (Landed earlier as a generated free function CTCCreatureFlyOnSpline_Alloc(CThing*) returning CTCCreatureFlyOnSpline*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureFlyOnSpline : public CTCBase {
public:
    CTCCreatureFlyOnSpline(CThing& thing);                        // 0x007DCF90
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureFlyOnSpline) = 0x2c
};

CTCBase* __fastcall CTCCreatureFlyOnSpline::Alloc(CThing& thing)
{
    return new CTCCreatureFlyOnSpline(thing);
}
