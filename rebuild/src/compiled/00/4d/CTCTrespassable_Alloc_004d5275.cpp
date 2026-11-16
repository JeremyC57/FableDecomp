#pragma optimize("s",on)
// CTCTrespassable::Alloc(CThing& thing) -- the registry factory: a new CTCTrespassable for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x007BAD20), returned as its CTCBase
// Leaf: ?Alloc@CTCTrespassable@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5275, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTrespassable::CTCTrespassable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTrespassable@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTrespassable@@QAE@AAVCThing@@@Z (0x007BAD20, landed). (Landed earlier as a generated free function CTCTrespassable_Alloc(CThing*) returning CTCTrespassable*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTrespassable : public CTCBase {
public:
    CTCTrespassable(CThing& thing);                        // 0x007BAD20
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCTrespassable) = 0x10
};

CTCBase* __fastcall CTCTrespassable::Alloc(CThing& thing)
{
    return new CTCTrespassable(thing);
}
