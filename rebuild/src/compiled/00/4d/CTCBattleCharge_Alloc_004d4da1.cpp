#pragma optimize("s",on)
// CTCBattleCharge::Alloc(CThing& thing) -- the registry factory: a new CTCBattleCharge for `thing` (operator new(0x94) 0x00BFEA1A, null-checked, then
//   the constructor 0x007A71B0), returned as its CTCBase
// Leaf: ?Alloc@CTCBattleCharge@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4DA1, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x94 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBattleCharge::CTCBattleCharge ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCBattleCharge@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBattleCharge@@QAE@AAVCThing@@@Z (0x007A71B0, landed). (Landed earlier as a generated free function CTCBattleCharge_Alloc(CThing*) returning CTCBattleCharge*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBattleCharge : public CTCBase {
public:
    CTCBattleCharge(CThing& thing);                        // 0x007A71B0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x94 - 0xC];              // the class's own members (not modelled); sizeof(CTCBattleCharge) = 0x94
};

CTCBase* __fastcall CTCBattleCharge::Alloc(CThing& thing)
{
    return new CTCBattleCharge(thing);
}
