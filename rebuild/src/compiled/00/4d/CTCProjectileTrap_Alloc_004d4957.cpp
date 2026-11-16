#pragma optimize("s",on)
// CTCProjectileTrap::Alloc(CThing& thing) -- the registry factory: a new CTCProjectileTrap for `thing` (operator new(0x74) 0x00BFEA1A, null-checked, then
//   the constructor 0x00787DDC), returned as its CTCBase
// Leaf: ?Alloc@CTCProjectileTrap@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4957, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x74 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCProjectileTrap::CTCProjectileTrap ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCProjectileTrap@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCProjectileTrap@@QAE@AAVCThing@@@Z (0x00787DDC, landed). (Landed earlier as a generated free function CTCProjectileTrap_Alloc(CThing*) returning CTCProjectileTrap*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCProjectileTrap : public CTCBase {
public:
    CTCProjectileTrap(CThing& thing);                        // 0x00787DDC
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x74 - 0xC];              // the class's own members (not modelled); sizeof(CTCProjectileTrap) = 0x74
};

CTCBase* __fastcall CTCProjectileTrap::Alloc(CThing& thing)
{
    return new CTCProjectileTrap(thing);
}
