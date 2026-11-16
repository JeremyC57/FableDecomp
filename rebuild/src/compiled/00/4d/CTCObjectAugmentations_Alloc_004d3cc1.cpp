#pragma optimize("s",on)
// CTCObjectAugmentations::Alloc(CThing& thing) -- the registry factory: a new CTCObjectAugmentations for `thing` (operator new(0x70) 0x00BFEA1A, null-checked, then
//   the constructor 0x007673C0), returned as its CTCBase
// Leaf: ?Alloc@CTCObjectAugmentations@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3CC1, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x70 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCObjectAugmentations::CTCObjectAugmentations ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCObjectAugmentations@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCObjectAugmentations@@QAE@AAVCThing@@@Z (0x007673C0, landed). (Landed earlier as a generated free function CTCObjectAugmentations_Alloc(CThing*) returning CTCObjectAugmentations*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCObjectAugmentations : public CTCBase {
public:
    CTCObjectAugmentations(CThing& thing);                        // 0x007673C0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x70 - 0xC];              // the class's own members (not modelled); sizeof(CTCObjectAugmentations) = 0x70
};

CTCBase* __fastcall CTCObjectAugmentations::Alloc(CThing& thing)
{
    return new CTCObjectAugmentations(thing);
}
