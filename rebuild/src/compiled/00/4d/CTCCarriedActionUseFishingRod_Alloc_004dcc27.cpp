#pragma optimize("s",on)
// CTCCarriedActionUseFishingRod::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseFishingRod for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB265), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseFishingRod@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCC27, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseFishingRod::CTCCarriedActionUseFishingRod ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseFishingRod@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseFishingRod@@QAE@AAVCThing@@@Z (0x004DB265, landed). (Landed earlier as a generated free function CTCCarriedActionUseFishingRod_Alloc(CThing*) returning CTCCarriedActionUseFishingRod*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseFishingRod : public CTCBase {
public:
    CTCCarriedActionUseFishingRod(CThing& thing);                        // 0x004DB265
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseFishingRod) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseFishingRod::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseFishingRod(thing);
}
