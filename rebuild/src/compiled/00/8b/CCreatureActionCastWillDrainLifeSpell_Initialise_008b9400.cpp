// CCreatureAction_CastWillDrainLifeSpell::Initialise() -- names this action's animation "ST_CAST_DRAIN_LIFE" then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9400, length 64 bytes. Vtable slot 12 of CCreatureAction_CastWillDrainLifeSpell (0x012645EC).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520 (direct base call).
// Shape: the scoped named CCharString (a bare temporary would reuse the constructor's eax where retail re-derives
// `lea eax,[esp+4]`), destroyed before the base call. Landed family shape (0x008CC720).
#include "fable_string.h"

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    void SetAnimationName(const CCharString& name);   // 0x008574D0 (propagated manifest label; named as in the landed family)
};

struct CCreatureAction_CastWillDrainLifeSpell : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_CastWillDrainLifeSpell::Initialise()
{
    {
        CCharString name("ST_CAST_DRAIN_LIFE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}