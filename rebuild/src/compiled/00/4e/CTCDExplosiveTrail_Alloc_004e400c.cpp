#pragma optimize("s",on)
// CTCDExplosiveTrail::Alloc(CThing& thing) -- the registry factory: a new CTCDExplosiveTrail for `thing` (operator new(0x30) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E3401), returned as its CTCBase
// Leaf: ?Alloc@CTCDExplosiveTrail@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E400C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x30 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDExplosiveTrail::CTCDExplosiveTrail ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDExplosiveTrail@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDExplosiveTrail@@QAE@AAVCThing@@@Z (0x004E3401, landed). (Landed earlier as a generated free function CTCDExplosiveTrail_Alloc(CThing*) returning CTCDExplosiveTrail*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDExplosiveTrail : public CTCBase {
public:
    CTCDExplosiveTrail(CThing& thing);                        // 0x004E3401
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x30 - 0xC];              // the class's own members (not modelled); sizeof(CTCDExplosiveTrail) = 0x30
};

CTCBase* __fastcall CTCDExplosiveTrail::Alloc(CThing& thing)
{
    return new CTCDExplosiveTrail(thing);
}
