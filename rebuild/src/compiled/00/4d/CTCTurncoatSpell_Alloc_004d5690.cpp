#pragma optimize("s",on)
// CTCTurncoatSpell::Alloc(CThing& thing) -- the registry factory: a new CTCTurncoatSpell for `thing` (operator new(0x98) 0x00BFEA1A, null-checked, then
//   the constructor 0x007C6760), returned as its CTCBase
// Leaf: ?Alloc@CTCTurncoatSpell@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5690, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x98 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTurncoatSpell::CTCTurncoatSpell ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTurncoatSpell@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTurncoatSpell@@QAE@AAVCThing@@@Z (0x007C6760, landed). (Landed earlier as a generated free function CTCTurncoatSpell_Alloc(CThing*) returning CTCTurncoatSpell*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTurncoatSpell : public CTCBase {
public:
    CTCTurncoatSpell(CThing& thing);                        // 0x007C6760
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x98 - 0xC];              // the class's own members (not modelled); sizeof(CTCTurncoatSpell) = 0x98
};

CTCBase* __fastcall CTCTurncoatSpell::Alloc(CThing& thing)
{
    return new CTCTurncoatSpell(thing);
}
