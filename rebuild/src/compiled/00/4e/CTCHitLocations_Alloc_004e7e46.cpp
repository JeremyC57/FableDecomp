#pragma optimize("s",on)
// CTCHitLocations::Alloc(CThing& thing) -- the registry factory: a new CTCHitLocations for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E75D9), returned as its CTCBase
// Leaf: ?Alloc@CTCHitLocations@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7E46, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHitLocations::CTCHitLocations ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCHitLocations@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHitLocations@@QAE@AAVCThing@@@Z (0x004E75D9, landed). (Landed earlier as a generated free function CTCHitLocations_Alloc(CThing*) returning CTCHitLocations*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHitLocations : public CTCBase {
public:
    CTCHitLocations(CThing& thing);                        // 0x004E75D9
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCHitLocations) = 0x34
};

CTCBase* __fastcall CTCHitLocations::Alloc(CThing& thing)
{
    return new CTCHitLocations(thing);
}
