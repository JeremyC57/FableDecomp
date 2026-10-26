// CCreatureAction_SummonerChargeSuccess::Initialise() -- set the combat animation "CHARGE_SUCCESS", then the base Initialise
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009703D0, length 51 bytes (+ int3 padding), vtable slot 12 of 0x0129733C
// (RTTI `?A0xe09fd86b::CCreatureAction_SummonerChargeSuccess`; FableWin.pdb ?Initialise@CCreatureAction_SummonerChargeSuccess@?A0x0ef6cb01@@UAEXXZ).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01297480 "CHARGE_SUCCESS"),
// CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0 (FableWin.pdb ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z;
// manifest "SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520 (last).
// Shape: the scoped name string lives in the `push ecx` frame word; `this` is held in esi across the three calls.
#include "fable_string.h"

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
    unsigned char _body[0x114 - 4];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_SummonerChargeSuccess : CCombatActionBase {   // vtable 0x0129733C, sizeof 0x114
    virtual void Initialise();
};

void CCreatureAction_SummonerChargeSuccess::Initialise()
{
    {
        CCharString name("CHARGE_SUCCESS", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}