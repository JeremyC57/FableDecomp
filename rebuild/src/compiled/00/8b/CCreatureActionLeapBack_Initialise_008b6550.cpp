// CCreatureAction_LeapBack::Initialise() -- set the combat animation MOVEMENT_STRAFE_BACKWARD, then the base CCombatActionBase::Initialise()
// Leaf: ?Initialise@CCreatureAction_LeapBack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008b6550, 51 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): vtable 0x01263024 (RTTI CCreatureAction_LeapBack) slot 12 (CCombatActionBase::Initialise 0x00857520 holds slot 12 of CCombatActionBase's vtable 0x01259E44); Ego_r.pdb ?Initialise@CCreatureAction_LeapBack@@UAEXXZ; RTTI base CCombatActionBase; animation literal 0x0127A400 "MOVEMENT_STRAFE_BACKWARD".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(
//   const CCharString&) 0x008574D0, CCombatActionBase::Initialise() 0x00857520.
// Shape (family L089): the landed 0x008C1E00 opening -- the animation name a block-scoped local, then the qualified base call.
// Header debt: engine/CCombatActionBase.h and engine/CCreatureAction_LeapBack.h are flat layouts without bases or methods; modelled locally.
#include "fable_string.h"
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    void SetCombatAnimation(const CCharString& name);      // 0x008574D0
    virtual void Initialise();                             // 0x00857520
};
class CCreatureAction_LeapBack : public CCombatActionBase {
public:
    virtual void Initialise();
};
void CCreatureAction_LeapBack::Initialise()
{
    {
        CCharString name("MOVEMENT_STRAFE_BACKWARD", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}