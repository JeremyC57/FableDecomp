#pragma optimize("s",on)
// CTCSummonableCreature::Alloc(CThing& thing) -- the registry factory: a new CTCSummonableCreature for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x0079C240), returned as its CTCBase
// Leaf: ?Alloc@CTCSummonableCreature@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4C0E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSummonableCreature::CTCSummonableCreature ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCSummonableCreature@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCSummonableCreature@@QAE@AAVCThing@@@Z (0x0079C240, landed). (Landed earlier as a generated free function CTCSummonableCreature_Alloc(CThing*) returning CTCSummonableCreature*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSummonableCreature : public CTCBase {
public:
    CTCSummonableCreature(CThing& thing);                        // 0x0079C240
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCSummonableCreature) = 0x1c
};

CTCBase* __fastcall CTCSummonableCreature::Alloc(CThing& thing)
{
    return new CTCSummonableCreature(thing);
}
