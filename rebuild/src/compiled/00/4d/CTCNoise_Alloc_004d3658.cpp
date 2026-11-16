#pragma optimize("s",on)
// CTCNoise::Alloc(CThing& thing) -- the registry factory: a new CTCNoise for `thing` (operator new(0x50) 0x00BFEA1A, null-checked, then
//   the constructor 0x00749810), returned as its CTCBase
// Leaf: ?Alloc@CTCNoise@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3658, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCNoise::CTCNoise ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCNoise@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCNoise@@QAE@AAVCThing@@@Z (0x00749810, landed). (Landed earlier as a generated free function CTCNoise_Alloc(CThing*) returning CTCNoise*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCNoise : public CTCBase {
public:
    CTCNoise(CThing& thing);                                // 0x00749810
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x50 - 0xC];              // the class's own members (not modelled); sizeof(CTCNoise) = 0x50
};

CTCBase* __fastcall CTCNoise::Alloc(CThing& thing)
{
    return new CTCNoise(thing);
}
