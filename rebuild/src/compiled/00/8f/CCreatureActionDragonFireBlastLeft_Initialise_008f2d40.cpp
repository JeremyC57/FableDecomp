// CCreatureAction_DragonFireBlastLeft::Initialise() -- set the combat animation FIRE_BLAST_LEFT, then the base CCombatActionBase::Initialise()
// Leaf: ?Initialise@CCreatureAction_DragonFireBlastLeft@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008f2d40, 51 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): vtable 0x0129422C (RTTI CCreatureAction_DragonFireBlastLeft) slot 12 (CCombatActionBase::Initialise 0x00857520 holds slot 12 of CCombatActionBase's vtable 0x01259E44); Ego_r.pdb ?Initialise@CCreatureAction_DragonFireBlastLeft@@UAEXXZ; RTTI bases CCreatureAction_DragonFireBlastBase <- CCombatActionBase (the base call resolves to CCombatActionBase::Initialise); animation literal 0x0128840C "FIRE_BLAST_LEFT".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(
//   const CCharString&) 0x008574D0, CCombatActionBase::Initialise() 0x00857520.
// Shape (family L089): the landed 0x008C1E00 opening -- the animation name a block-scoped local, then the qualified base call.
// Header debt: engine/CCombatActionBase.h and engine/CCreatureAction_DragonFireBlastLeft.h are flat layouts without bases or methods; modelled locally.
#include "fable_string.h"
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    void SetCombatAnimation(const CCharString& name);      // 0x008574D0
    virtual void Initialise();                             // 0x00857520
};
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase { };   // no Initialise of its own (Ego_r.pdb)
class CCreatureAction_DragonFireBlastLeft : public CCreatureAction_DragonFireBlastBase {
public:
    virtual void Initialise();
};
void CCreatureAction_DragonFireBlastLeft::Initialise()
{
    {
        CCharString name("FIRE_BLAST_LEFT", -1);
        SetCombatAnimation(name);
    }
    CCreatureAction_DragonFireBlastBase::Initialise();
}