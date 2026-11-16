#pragma optimize("s",on)
// CTCParticleAttacher::Alloc(CThing& thing) -- the registry factory: a new CTCParticleAttacher for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DE7FD), returned as its CTCBase
// Leaf: ?Alloc@CTCParticleAttacher@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0BBD, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCParticleAttacher::CTCParticleAttacher ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCParticleAttacher@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCParticleAttacher@@QAE@AAVCThing@@@Z (0x004DE7FD, landed). (Landed earlier as a generated free function CTCParticleAttacher_Alloc(CThing*) returning CTCParticleAttacher*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCParticleAttacher : public CTCBase {
public:
    CTCParticleAttacher(CThing& thing);                        // 0x004DE7FD
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCParticleAttacher) = 0x1c
};

CTCBase* __fastcall CTCParticleAttacher::Alloc(CThing& thing)
{
    return new CTCParticleAttacher(thing);
}
