#pragma optimize("s",on)
// CTCJackPillar::Alloc(CThing& thing) -- the registry factory: a new CTCJackPillar for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x007315F0), returned as its CTCBase
// Leaf: ?Alloc@CTCJackPillar@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3380, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCJackPillar::CTCJackPillar ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCJackPillar@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCJackPillar@@QAE@AAVCThing@@@Z (0x007315F0, landed). (Landed earlier as a generated free function CTCJackPillar_Alloc(CThing*) returning CTCJackPillar*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCJackPillar : public CTCBase {
public:
    CTCJackPillar(CThing& thing);                        // 0x007315F0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCJackPillar) = 0xc
};

CTCBase* __fastcall CTCJackPillar::Alloc(CThing& thing)
{
    return new CTCJackPillar(thing);
}
