#pragma optimize("s",on)
// CTCGuard::Alloc(CThing& thing) -- the registry factory: a new CTCGuard for `thing` (operator new(0x54) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B2DB0), returned as its CTCBase
// Leaf: ?Alloc@CTCGuard@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5013, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x54 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGuard::CTCGuard ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCGuard@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGuard@@QAE@AAVCThing@@@Z (0x007B2DB0, landed). (Landed earlier as a generated free function CTCGuard_Alloc(CThing*) returning CTCGuard*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCGuard : public CTCBase {
public:
    CTCGuard(CThing& thing);                        // 0x007B2DB0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x54 - 0xC];              // the class's own members (not modelled); sizeof(CTCGuard) = 0x54
};

CTCBase* __fastcall CTCGuard::Alloc(CThing& thing)
{
    return new CTCGuard(thing);
}
