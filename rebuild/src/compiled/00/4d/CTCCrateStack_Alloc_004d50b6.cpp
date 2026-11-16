#pragma optimize("s",on)
// CTCCrateStack::Alloc(CThing& thing) -- the registry factory: a new CTCCrateStack for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B6610), returned as its CTCBase
// Leaf: ?Alloc@CTCCrateStack@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D50B6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCrateStack::CTCCrateStack ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCrateStack@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCrateStack@@QAE@AAVCThing@@@Z (0x007B6610, landed). (Landed earlier as a generated free function CTCCrateStack_Alloc(CThing*) returning CTCCrateStack*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCrateStack : public CTCBase {
public:
    CTCCrateStack(CThing& thing);                        // 0x007B6610
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCCrateStack) = 0x20
};

CTCBase* __fastcall CTCCrateStack::Alloc(CThing& thing)
{
    return new CTCCrateStack(thing);
}
