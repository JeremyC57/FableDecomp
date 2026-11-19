#ifndef FABLE_ENGINE_CCOMBATACTION_WILLFORCEPUSHINTO_H
#define FABLE_ENGINE_CCOMBATACTION_WILLFORCEPUSHINTO_H
// New header (chunk 2017, row 0x00974110 CAIStateGroup_JackOfBladesBase::Summon). RTTI .?AVCCombatAction_WillForcePushInto@@ (vtable
// 0x012804FC) : CCombatActionBase, no own members (Ego_r size 300 = the Ego_r CCombatActionBase; retail 0x114). Constructor Ego_r.pdb
// ??0CCombatAction_WillForcePushInto@@QAE@AAVCThingCreatureBase@@@Z, inline: Summon expands it (0x009745FE..0x0097464D: CCharString
// "SWORD_SWING", GetSoundPair, CCombatActionBase(owner, 150, ..) 0x00858030, the vtable store); its out-of-line COMDAT is the landed
// 0x008C38B0. Implicit destructor (Summon destroys the temporary with ~CCombatActionBase 0x0062E900). Needs
// FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR.
#include "engine/CCombatActionBase.h"
class CThingCreatureBase;
class CCombatAction_WillForcePushInto : public CCombatActionBase {
public:
    CCombatAction_WillForcePushInto(CThingCreatureBase& owner) : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")) { }
#ifdef FABLE_RETAIL_CCOMBATACTION_WILLFORCEPUSHINTO_FINISHACTION
    // NF93 opt-in (round 8, q11261 / q11262 row 0x008C2980): Ego_r.pdb / FableWin.pdb ?FinishAction@CCombatAction_WillForcePushInto@@UAEXXZ
    // (vtable slot 4, 0x008C2980). Declaration only: no layout change.
    virtual void FinishAction();                                         // slot 4, 0x008C2980
#endif
};
#endif
