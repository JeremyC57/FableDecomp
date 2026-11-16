#pragma optimize("s",on)
// CTCNymph::Alloc(CThing& thing) -- the registry factory: a new CTCNymph for `thing` (operator new(0x70) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E3325), returned as its CTCBase
// Leaf: ?Alloc@CTCNymph@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3FEF, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x70 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCNymph::CTCNymph ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCNymph@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCNymph@@QAE@AAVCThing@@@Z (0x004E3325, landed). (Landed earlier as a generated free function CTCNymph_Alloc(CThing*) returning CTCNymph*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCNymph : public CTCBase {
public:
    CTCNymph(CThing& thing);                        // 0x004E3325
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x70 - 0xC];              // the class's own members (not modelled); sizeof(CTCNymph) = 0x70
};

CTCBase* __fastcall CTCNymph::Alloc(CThing& thing)
{
    return new CTCNymph(thing);
}
