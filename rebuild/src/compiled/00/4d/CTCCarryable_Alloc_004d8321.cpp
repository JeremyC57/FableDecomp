#pragma optimize("s",on)
// CTCCarryable::Alloc(CThing& thing) -- the registry factory: a new CTCCarryable for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D3D0E), returned as its CTCBase
// Leaf: ?Alloc@CTCCarryable@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8321, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarryable::CTCCarryable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCarryable@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarryable@@QAE@AAVCThing@@@Z (0x004D3D0E, landed). (Landed earlier as a generated free function CTCCarryable_Alloc(CThing*) returning CTCCarryable*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarryable : public CTCBase {
public:
    CTCCarryable(CThing& thing);                        // 0x004D3D0E
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarryable) = 0x24
};

CTCBase* __fastcall CTCCarryable::Alloc(CThing& thing)
{
    return new CTCCarryable(thing);
}
