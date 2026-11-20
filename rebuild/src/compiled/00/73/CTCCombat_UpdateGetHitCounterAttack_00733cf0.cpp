// CTCCombat::UpdateGetHitCounterAttack(const CThingCreatureBase& hit_by) -- when the creature has a get-hit counter-attack ability that is
//   active, or idle and wins its percentage roll on the world seed, and its entity events hold an unblocked melee hit by hit_by while its
//   current action can be interrupted: set the ability active (frames = ConstantFPS * SecondsDuration) and finish the current action
// Leaf: ?UpdateGetHitCounterAttack@CTCCombat@@QAEXABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733CF0, 288 bytes, __fastcall, `ret 4`. Grade: RELOCATION_MATCH (288/288). No flag, no pragma.
// PDB: Ego_r.pdb / FableWin.pdb ?UpdateGetHitCounterAttack@CTCCombat@@QAEXABVCThingCreatureBase@@@Z (catalogue TCCombat_ApplyBlockResponse
//   was descriptive). Ego_d.pdb locals: this, hit_by, it { pevent_hit, ptc_entity_events { attacker_creature } }.
// Callees: CTCCombat::GetCombatAbilityData 0x00733620 (same TU, below), CTCBase::PeekWorld 0x00686A80, CWorld::GetWorldSeed 0x0049D810,
//   GFROR13 0x00497890, the const component-map LowerBound 0x004365B0, CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind3<
//   CIsHitEventHitByMelee, unary_negate<CIsHitEventBlocked>, CIsHitEventHitBy> > 0x00738A80, CThingCreatureBase::IsPerformingAnyAction
//   0x006622A0 / FinishCurrentAction 0x00662650, CCreatureActionBase::IsActionReadyForInterruption (slot 27, +0x6C), __ftol2.
// The bare `call 0x00686A80` (0x00733D1C, no `mov ecx,this`): GetCombatAbilityData never writes ecx, and with its body compiled earlier in
//   the TU VC7.1 knows `this` is still in ecx after the call (GOTCHAS nf96a, precedent 0x005C150B). The earlier drafts' "this-less LTCG
//   call" reading was wrong; without the same-TU body the row reloads ecx (evidence/ablation.txt).
// Same-TU bodies (Ego_d.pdb puts CCombatAbility::IsActive / IsValid / SetAsActive in tc_combat.cpp, line-table file id 3264, the row's
//   file): verbatim copies of the rows 0x00733090, 0x007332F0, 0x007330A0, 0x00733620 (same define block). IsValid and SetAsActive are
//   expanded in the row (0x00733D2A.., 0x00733DEB..), as retail does.
// Text oracle: Ego_d line sizes at /Od (row 1117..1135: decl 15 [Dinkumware iterator; ours 8], 89, 100, 20, 46 [ours 44], 37, 8; IsValid
//   15/4/29/4/2, IsActive 14, SetAsActive 28) -- ours 89, 100, 20, 37, 8 and every CCombatAbility line equal. One condition line (1118)
//   holds both the lookup and IsValid; the lookup-then-return spelling would need two more line records.
//   GFFilter_Bind3: FableWin.pdb has this exact instance (??$GFFilter_Bind3@VCIsHitEventHitByMelee@..unary_negate..CIsHitEventHitBy..),
//   and it gives Ego_d's 100 bytes (the CFilter_Bind3 constructor spelling: 99). attacker_creature = GFDebugPDynamicCast(&GetParentThing(),
//   ..) gives Ego_d's 20 (FableWin.pdb ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingCreatureBase@@@@..; the plain cast: 9).
// Layout: engine/CTCCombat.h (CombatAbilities multimap +0x70; node value key +0x10, CCountedPointer<CCombatAbilityData> +0x14,
//   CCombatAbility +0x1C RepeatTimer / +0x20 ActiveTimer), CCombatAbilityData (PercentageChance +4, SecondsDuration +8, InterfaceType +0x14),
//   CThing (UsingComponents +0x20, Components +0x44), CThingCreatureBase (PCurrentAction +0x100).
// Header opt-ins (overlay, add-only): FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK (declarations), FABLE_RETAIL_CCOMBATABILITY_MEMBERS
//   (declarations).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITY_MEMBERS
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
#include "engine/ECombatAbilityInterfaceType.h"
#include "engine/CTCCombat.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatAbilityData.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

// Fidelity: same-TU strong definition of 0x00733090 (retail TU-mate)
bool CCombatAbility::IsActive() const
{
    return ActiveTimer >= 0;
}

// Fidelity: same-TU strong definition of 0x007332F0 (retail TU-mate)
bool CCombatAbility::IsValid(const CCombatAbilityData& combat_ability, unsigned long* prandom_seed) const
{
    if (IsActive())
        return true;
    if (RepeatTimer == -1 && GFRandom(100, prandom_seed) < (unsigned long)combat_ability.PercentageChance)
        return true;
    return false;
}

// Fidelity: same-TU strong definition of 0x007330A0 (retail TU-mate)
void CCombatAbility::SetAsActive(const CCombatAbilityData& combat_ability)
{
    ActiveTimer = (long)(CMainGameComponent::GetConstantFPS() * combat_ability.SecondsDuration);
}

// Fidelity: same-TU strong definition of 0x00733620 (retail TU-mate)
bool CTCCombat::GetCombatAbilityData(TCombatAbilityList::iterator& ittybitty, ECombatAbilityInterfaceType interface_type)
{
    for (ittybitty = CombatAbilities.begin(); ittybitty != CombatAbilities.end(); ++ittybitty) {
        if (ittybitty->second.first->GetInterfaceType() == interface_type)
            return true;
    }
    return false;
}

void CTCCombat::UpdateGetHitCounterAttack(const CThingCreatureBase& hit_by)
{
    TCombatAbilityList::iterator it;
    if (GetCombatAbilityData(it, COMBAT_ABILITY_INTERFACE_ATTACK_GET_HIT_COUNTER) &&
        it->second.second.IsValid(*it->second.first, PeekWorld().GetWorldSeed())) {
        const NEntityEvents::CEventHitBy* pevent_hit;
        const CTCEntityEvents* ptc_entity_events;
        if (GetParentThing().PeekTC(&ptc_entity_events)
            && ptc_entity_events->GetEvent(&pevent_hit, GFFilter_Bind3(
                   NEntityEvents::CIsHitEventHitByMelee(), std::not1(NEntityEvents::CIsHitEventBlocked()), NEntityEvents::CIsHitEventHitBy(hit_by)))) {
            CThingCreatureBase* attacker_creature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
            if (attacker_creature->IsPerformingAnyAction() && attacker_creature->PeekPCurrentAction()->IsActionReadyForInterruption()) {
                it->second.second.SetAsActive(*it->second.first);
                attacker_creature->FinishCurrentAction();
            }
        }
    }
}