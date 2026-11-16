#pragma optimize("s",on)
// CTCJackShield::Alloc(CThing& thing) -- the registry factory: a new CTCJackShield for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x00731610), returned as its CTCBase
// Leaf: ?Alloc@CTCJackShield@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3350, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCJackShield::CTCJackShield ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCJackShield@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCJackShield@@QAE@AAVCThing@@@Z (0x00731610, landed). (Landed earlier as a generated free function CTCJackShield_Alloc(CThing*) returning CTCJackShield*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCJackShield : public CTCBase {
public:
    CTCJackShield(CThing& thing);                        // 0x00731610
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCJackShield) = 0x14
};

CTCBase* __fastcall CTCJackShield::Alloc(CThing& thing)
{
    return new CTCJackShield(thing);
}
