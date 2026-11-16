#pragma optimize("s",on)
// CTCCarrying::Alloc(CThing& thing) -- the registry factory: a new CTCCarrying for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E748A), returned as its CTCBase
// Leaf: ?Alloc@CTCCarrying@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7DFC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarrying::CTCCarrying ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCarrying@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarrying@@QAE@AAVCThing@@@Z (0x004E748A, landed). (Landed earlier as a generated free function CTCCarrying_Alloc(CThing*) returning CTCCarrying*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarrying : public CTCBase {
public:
    CTCCarrying(CThing& thing);                        // 0x004E748A
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarrying) = 0x28
};

CTCBase* __fastcall CTCCarrying::Alloc(CThing& thing)
{
    return new CTCCarrying(thing);
}
