#pragma optimize("s",on)
// CTCRockTrollShield::Alloc(CThing& thing) -- the registry factory: a new CTCRockTrollShield for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x007AEE10), returned as its CTCBase
// Leaf: ?Alloc@CTCRockTrollShield@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4EAC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCRockTrollShield::CTCRockTrollShield ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCRockTrollShield@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCRockTrollShield@@QAE@AAVCThing@@@Z (0x007AEE10, landed). (Landed earlier as a generated free function CTCRockTrollShield_Alloc(CThing*) returning CTCRockTrollShield*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCRockTrollShield : public CTCBase {
public:
    CTCRockTrollShield(CThing& thing);                        // 0x007AEE10
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCRockTrollShield) = 0x14
};

CTCBase* __fastcall CTCRockTrollShield::Alloc(CThing& thing)
{
    return new CTCRockTrollShield(thing);
}
