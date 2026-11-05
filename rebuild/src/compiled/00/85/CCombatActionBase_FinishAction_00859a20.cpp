// CCombatActionBase::FinishAction() -- if the target is still alive drop its TC 0x100, reset the melee-ability effects, then run CCreatureActionBase::FinishAction
// Leaf: ?FinishAction@CCombatActionBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00859A20, 72 bytes, thiscall, tail `jmp` to the base FinishAction.
// Identity: Ego_r.pdb ?FinishAction@CCombatActionBase@@MAEXXZ (the catalog's mangled label agrees); it ends in the explicit base call
//   CCreatureActionBase::FinishAction 0x00694430 (?FinishAction@CCreatureActionBase@@UAEXXZ) and calls the CCombatActionBase member
//   ResetMeleeCombatAbilityEffects 0x00859660 (Ego_r ..@IAEXXZ); 91 callers are derived FinishAction overrides.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::RemoveTC(ETCInterfaceType) 0x004C9840 (TC type 0x100; the enumerator
//   name is unproven), ResetMeleeCombatAbilityEffects 0x00859660, CCreatureActionBase::FinishAction 0x00694430.
// Layout: local model -- engine/CCreatureActionBase.h flattens the vptr; retail CCreatureActionBase is 0xA8 bytes, so PTarget
//   (Ego_r +0xB4) is +0xA8; CThing::Dead is bit 0 of +0x91 (Ego_r bitfield).
// Shape: IS_THING_ALIVE(PTarget) as the inline __fastcall template the PDB name sets list (??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_N..)
//   -- GetPItem twice then the Dead test; a third GetPItem for the RemoveTC call.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing.GetPItem() && !thing.GetPItem()->IsDead();
}
class CCreatureActionBase {
public:
    virtual void FinishAction();                              // 0x00694430
    unsigned char _pad04[0xa8 - 4];
};
class CCombatActionBase : public CCreatureActionBase {
protected:
    virtual void FinishAction();
    void ResetMeleeCombatAbilityEffects();                    // 0x00859660
    CIntelligentPointer<CThing> PTarget;                      // +0xa8
};
void CCombatActionBase::FinishAction()
{
    if (IS_THING_ALIVE(PTarget))
        PTarget.GetPItem()->RemoveTC((ETCInterfaceType)0x100);
    ResetMeleeCombatAbilityEffects();
    CCreatureActionBase::FinishAction();
}