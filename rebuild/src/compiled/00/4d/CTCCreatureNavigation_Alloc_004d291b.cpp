#pragma optimize("s",on)
// CTCCreatureNavigation::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureNavigation for `thing` (operator new(0x60) 0x00BFEA1A, null-checked, then
//   the constructor 0x006CC170), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureNavigation@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D291B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x60 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureNavigation::CTCCreatureNavigation ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureNavigation@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureNavigation@@QAE@AAVCThing@@@Z (0x006CC170, landed). (Landed earlier as a generated free function CTCCreatureNavigation_Alloc(CThing*) returning CTCCreatureNavigation*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureNavigation : public CTCBase {
public:
    CTCCreatureNavigation(CThing& thing);                        // 0x006CC170
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x60 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureNavigation) = 0x60
};

CTCBase* __fastcall CTCCreatureNavigation::Alloc(CThing& thing)
{
    return new CTCCreatureNavigation(thing);
}
