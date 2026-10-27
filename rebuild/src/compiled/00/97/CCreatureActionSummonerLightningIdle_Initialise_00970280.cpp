// CCreatureAction_SummonerLightningIdle::Initialise() -- set the combat animation LIGHTNING_FOLLOWUP_IDLE, then the base CCombatActionBase::Initialise()
// Leaf: ?Initialise@CCreatureAction_SummonerLightningIdle@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00970280, 51 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): vtable 0x0129708C (RTTI ?A0xe09fd86b::CCreatureAction_SummonerLightningIdle) slot 12 (CCombatActionBase::Initialise 0x00857520 holds slot 12 of CCombatActionBase's vtable 0x01259E44); Ego_r.pdb ?Initialise@CCreatureAction_SummonerLightningIdle@?A0x0ef6cb01@@UAEXXZ (anonymous namespace; retail RTTI ?A0xe09fd86b); RTTI base CCombatActionBase; animation literal 0x012971D0 "LIGHTNING_FOLLOWUP_IDLE".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(
//   const CCharString&) 0x008574D0, CCombatActionBase::Initialise() 0x00857520.
// Shape (family L089): the landed 0x008C1E00 opening -- the animation name a block-scoped local, then the qualified base call.
// Header debt: engine/CCombatActionBase.h and engine/CCreatureAction_SummonerLightningIdle.h are flat layouts without bases or methods; modelled locally.
#include "fable_string.h"
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    void SetCombatAnimation(const CCharString& name);      // 0x008574D0
    virtual void Initialise();                             // 0x00857520
};
namespace {
class CCreatureAction_SummonerLightningIdle : public CCombatActionBase {
public:
    virtual void Initialise();
};
void CCreatureAction_SummonerLightningIdle::Initialise()
{
    {
        CCharString name("LIGHTNING_FOLLOWUP_IDLE", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}
}   // anonymous namespace