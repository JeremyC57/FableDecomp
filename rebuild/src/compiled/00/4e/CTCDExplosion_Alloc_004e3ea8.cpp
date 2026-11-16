#pragma optimize("s",on)
// CTCDExplosion::Alloc(CThing& thing) -- the registry factory: a new CTCDExplosion for `thing` (operator new(0x78) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E30E7), returned as its CTCBase
// Leaf: ?Alloc@CTCDExplosion@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3EA8, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x78 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDExplosion::CTCDExplosion ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDExplosion@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDExplosion@@QAE@AAVCThing@@@Z (0x004E30E7, landed). (Landed earlier as a generated free function CTCDExplosion_Alloc(CThing*) returning CTCDExplosion*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDExplosion : public CTCBase {
public:
    CTCDExplosion(CThing& thing);                        // 0x004E30E7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x78 - 0xC];              // the class's own members (not modelled); sizeof(CTCDExplosion) = 0x78
};

CTCBase* __fastcall CTCDExplosion::Alloc(CThing& thing)
{
    return new CTCDExplosion(thing);
}
