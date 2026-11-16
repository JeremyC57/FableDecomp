#pragma optimize("s",on)
// CTCActionUseKickChicken::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseKickChicken for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAC36), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseKickChicken@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC283, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseKickChicken::CTCActionUseKickChicken ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCActionUseKickChicken@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseKickChicken@@QAE@AAVCThing@@@Z (0x004DAC36, landed). (Landed earlier as a generated free function CTCActionUseKickChicken_Alloc(CThing*) returning CTCActionUseKickChicken*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseKickChicken : public CTCBase {
public:
    CTCActionUseKickChicken(CThing& thing);                        // 0x004DAC36
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseKickChicken) = 0x14
};

CTCBase* __fastcall CTCActionUseKickChicken::Alloc(CThing& thing)
{
    return new CTCActionUseKickChicken(thing);
}
