#pragma optimize("s",on)
// NGraphicAppearance::CTCGraphicAppearance::Alloc(CThing& thing) -- the registry factory: a new CTCGraphicAppearance for `thing` (operator new(0x5c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004C00E0), returned as its CTCBase
// Leaf: ?Alloc@CTCGraphicAppearance@NGraphicAppearance@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D44E8, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x5c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGraphicAppearance::CTCGraphicAppearance ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCGraphicAppearance@NGraphicAppearance@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGraphicAppearance@NGraphicAppearance@@QAE@AAVCThing@@@Z (0x004C00E0, landed). (Landed earlier as a generated free function CTCGraphicAppearance_Alloc(CThing*) returning CTCGraphicAppearance*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase {
public:
    CTCGraphicAppearance(CThing& thing);                        // 0x004C00E0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x5c - 0xC];              // the class's own members (not modelled); sizeof(CTCGraphicAppearance) = 0x5c
};

CTCBase* __fastcall CTCGraphicAppearance::Alloc(CThing& thing)
{
    return new CTCGraphicAppearance(thing);
}
}
