#pragma optimize("s",on)
// CTCLightningOrb::Alloc(CThing& thing) -- the registry factory: a new CTCLightningOrb for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB4D7), returned as its CTCBase
// Leaf: ?Alloc@CTCLightningOrb@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCEA4, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCLightningOrb::CTCLightningOrb ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCLightningOrb@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCLightningOrb@@QAE@AAVCThing@@@Z (0x004DB4D7, landed). (Landed earlier as a generated free function CTCLightningOrb_Alloc(CThing*) returning CTCLightningOrb*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCLightningOrb : public CTCBase {
public:
    CTCLightningOrb(CThing& thing);                        // 0x004DB4D7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCLightningOrb) = 0x34
};

CTCBase* __fastcall CTCLightningOrb::Alloc(CThing& thing)
{
    return new CTCLightningOrb(thing);
}
