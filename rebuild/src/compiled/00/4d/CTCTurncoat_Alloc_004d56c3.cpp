#pragma optimize("s",on)
// CTCTurncoat::Alloc(CThing& thing) -- the registry factory: a new CTCTurncoat for `thing` (operator new(0x4c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007C78C0), returned as its CTCBase
// Leaf: ?Alloc@CTCTurncoat@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D56C3, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x4c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTurncoat::CTCTurncoat ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTurncoat@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCTurncoat@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x007C78C0, landed in the same unit.
// Naming (NF456, r454b unit with 0x007C78C0): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCTurncoat_Alloc(CThing*) returning CTCTurncoat*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTurncoat : public CTCBase {
public:
    CTCTurncoat(CThing& thing);                             // 0x007C78C0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x4c - 0xC];              // the class's own members (not modelled); sizeof(CTCTurncoat) = 0x4c
};

CTCBase* __fastcall CTCTurncoat::Alloc(CThing& thing)
{
    return new CTCTurncoat(thing);
}
