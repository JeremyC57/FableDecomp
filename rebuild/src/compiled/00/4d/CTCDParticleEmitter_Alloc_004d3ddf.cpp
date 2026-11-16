#pragma optimize("s",on)
// CTCDParticleEmitter::Alloc(CThing& thing) -- the registry factory: a new CTCDParticleEmitter for `thing` (operator new(0x48) 0x00BFEA1A, null-checked, then
//   the constructor 0x006DFF30), returned as its CTCBase
// Leaf: ?Alloc@CTCDParticleEmitter@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3DDF, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDParticleEmitter::CTCDParticleEmitter ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDParticleEmitter@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDParticleEmitter@@QAE@AAVCThing@@@Z (0x006DFF30, landed). (Landed earlier as a generated free function CTCDParticleEmitter_Alloc(CThing*) returning CTCDParticleEmitter*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDParticleEmitter : public CTCBase {
public:
    CTCDParticleEmitter(CThing& thing);                        // 0x006DFF30
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x48 - 0xC];              // the class's own members (not modelled); sizeof(CTCDParticleEmitter) = 0x48
};

CTCBase* __fastcall CTCDParticleEmitter::Alloc(CThing& thing)
{
    return new CTCDParticleEmitter(thing);
}
