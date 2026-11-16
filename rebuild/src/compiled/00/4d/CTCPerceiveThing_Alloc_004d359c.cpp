#pragma optimize("s",on)
// CTCPerceiveThing::Alloc(CThing& thing) -- the registry factory: a new CTCPerceiveThing for `thing` (operator new(0x58) 0x00BFEA1A, null-checked, then
//   the constructor 0x007445E0), returned as its CTCBase
// Leaf: ?Alloc@CTCPerceiveThing@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D359C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x58 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPerceiveThing::CTCPerceiveThing ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCPerceiveThing@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCPerceiveThing@@QAE@AAVCThing@@@Z (0x007445E0, landed). (Landed earlier as a generated free function CTCPerceiveThing_Alloc(CThing*) returning CTCPerceiveThing*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPerceiveThing : public CTCBase {
public:
    CTCPerceiveThing(CThing& thing);                        // 0x007445E0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x58 - 0xC];              // the class's own members (not modelled); sizeof(CTCPerceiveThing) = 0x58
};

CTCBase* __fastcall CTCPerceiveThing::Alloc(CThing& thing)
{
    return new CTCPerceiveThing(thing);
}
