#pragma optimize("s",on)
// CTCFishingRod::Alloc(CThing& thing) -- the registry factory: a new CTCFishingRod for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E2450), returned as its CTCBase
// Leaf: ?Alloc@CTCFishingRod@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5F65, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFishingRod::CTCFishingRod ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCFishingRod@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFishingRod@@QAE@AAVCThing@@@Z (0x007E2450, landed). (Landed earlier as a generated free function CTCFishingRod_Alloc(CThing*) returning CTCFishingRod*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFishingRod : public CTCBase {
public:
    CTCFishingRod(CThing& thing);                        // 0x007E2450
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCFishingRod) = 0x10
};

CTCBase* __fastcall CTCFishingRod::Alloc(CThing& thing)
{
    return new CTCFishingRod(thing);
}
