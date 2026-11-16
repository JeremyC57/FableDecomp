#pragma optimize("s",on)
// CTCFishingSpot::Alloc(CThing& thing) -- the registry factory: a new CTCFishingSpot for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B0150), returned as its CTCBase
// Leaf: ?Alloc@CTCFishingSpot@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4F21, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFishingSpot::CTCFishingSpot ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCFishingSpot@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFishingSpot@@QAE@AAVCThing@@@Z (0x007B0150, landed). (Landed earlier as a generated free function CTCFishingSpot_Alloc(CThing*) returning CTCFishingSpot*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFishingSpot : public CTCBase {
public:
    CTCFishingSpot(CThing& thing);                        // 0x007B0150
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCFishingSpot) = 0x28
};

CTCBase* __fastcall CTCFishingSpot::Alloc(CThing& thing)
{
    return new CTCFishingSpot(thing);
}
