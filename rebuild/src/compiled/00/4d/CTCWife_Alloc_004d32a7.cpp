#pragma optimize("s",on)
// CTCWife::Alloc(CThing& thing) -- the registry factory: a new CTCWife for `thing` (operator new(0xec) 0x00BFEA1A, null-checked, then
//   the constructor 0x006CF040), returned as its CTCBase
// Leaf: ?Alloc@CTCWife@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D32A7, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xec ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWife::CTCWife ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCWife@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWife@@QAE@AAVCThing@@@Z (0x006CF040, landed). (Landed earlier as a generated free function CTCWife_Alloc(CThing*) returning CTCWife*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWife : public CTCBase {
public:
    CTCWife(CThing& thing);                        // 0x006CF040
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xec - 0xC];              // the class's own members (not modelled); sizeof(CTCWife) = 0xec
};

CTCBase* __fastcall CTCWife::Alloc(CThing& thing)
{
    return new CTCWife(thing);
}
