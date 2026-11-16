#pragma optimize("s",on)
// CTCCombat::Alloc(CThing& thing) -- the registry factory: a new CTCCombat for `thing` (operator new(0x98) 0x00BFEA1A, null-checked, then
//   the constructor 0x00735540), returned as its CTCBase
// Leaf: ?Alloc@CTCCombat@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3513, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x98 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCombat::CTCCombat ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCombat@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCombat@@QAE@AAVCThing@@@Z (0x00735540, landed). (Landed earlier as a generated free function CTCCombat_Alloc(CThing*) returning CTCCombat*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCombat : public CTCBase {
public:
    CTCCombat(CThing& thing);                        // 0x00735540
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x98 - 0xC];              // the class's own members (not modelled); sizeof(CTCCombat) = 0x98
};

CTCBase* __fastcall CTCCombat::Alloc(CThing& thing)
{
    return new CTCCombat(thing);
}
