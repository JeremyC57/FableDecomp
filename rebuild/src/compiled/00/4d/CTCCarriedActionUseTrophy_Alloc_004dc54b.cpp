#pragma optimize("s",on)
// CTCCarriedActionUseTrophy::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseTrophy for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAE4D), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseTrophy@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC54B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseTrophy::CTCCarriedActionUseTrophy ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseTrophy@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseTrophy@@QAE@AAVCThing@@@Z (0x004DAE4D, landed). (Landed earlier as a generated free function CTCCarriedActionUseTrophy_Alloc(CThing*) returning CTCCarriedActionUseTrophy*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseTrophy : public CTCBase {
public:
    CTCCarriedActionUseTrophy(CThing& thing);                        // 0x004DAE4D
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseTrophy) = 0x28
};

CTCBase* __fastcall CTCCarriedActionUseTrophy::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseTrophy(thing);
}
