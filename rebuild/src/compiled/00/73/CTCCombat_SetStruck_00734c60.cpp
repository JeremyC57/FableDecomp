// 00734c60 CTCCombat::SetStruck
// Flags: none
#define FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK
#define FABLE_RETAIL_STLP_MULTIMAP_PASS22_ATMOS_ITERATION
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_COMBAT_SETSTRUCK_PASS29
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_CLEARALLDAMAGE
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITY_MEMBERS
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND
#define FABLE_RETAIL_CCREATUREACTION_BLOCKRESPOND_VIRTUAL_MODEL
#define FABLE_RETAIL_CCREATUREACTION_BLOCKCOUNTER_VIRTUAL_MODEL
#define FABLE_RETAIL_CCREATUREACTION_BLOCKBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DCOORDI
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
// Leaf: ?SetStruck@CTCCombat@@QAEXABVCHitLocation@NHitLocations@@AAVCHitParameters@@@Z
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/ECombatAbilityInterfaceType.h"
#include "engine/EWeaponClass.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureActionBase.h"
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatActionBase.h"
#undef FABLE_RETAIL_KRAKEN_ROAR


#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_BlockBroken.h"
#include "engine/CCreatureAction_BlockCounter.h"
#include "engine/CCreatureAction_BlockRespond.h"
#include "engine/CMaterialDef.h"
#include "engine/CArmourDef.h"
#include "engine/CHitParameters.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CTCCombat.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCHeroExperience.h"
#include "engine/NCreatureMode.h"
#include "engine/CMainGameComponent.h"
#define FABLE_RETAIL_BRIAR_ROSE
#include "engine/CTCPhysicalShieldSpell.h"
#undef FABLE_RETAIL_BRIAR_ROSE
inline ETCInterfaceType FABLE_FASTCALL CTCPhysicalShieldSpell::StatGetInterfaceType(){return TCI_PHYSICAL_SHIELD_SPELL;}
bool CTCCombat::GetCombatAbilityData(TCombatAbilityList::const_iterator& ittybitty,
                                    ECombatAbilityInterfaceType interface_type) const
{
    TCombatAbilityList::iterator iterator_guy;
    CTCCombat* ptc_me = const_cast<CTCCombat*>(this);
    if (ptc_me->GetCombatAbilityData(iterator_guy,interface_type)) {
        ittybitty = iterator_guy;
        return true;
    }
    return false;
}
void CTCCombat::SetStruck(const NHitLocations::CHitLocation& hit_location, CHitParameters& hit_params)
{
    CThingCreatureBase& defender_creature = GetParentCreature();
    const CCreatureActionBase* pdefender_action = defender_creature.PeekPCurrentAction();
    bool block_counter = hit_params.IsBlockCounter();
    bool counter_attack = false;
    const C3DVector* ppoint_of_hit = 0;
    if (hit_params.IsLocalised())
        ppoint_of_hit = &hit_params.GetHitPosition();
    CThing* pthing_hitting = hit_params.GetPThingHitting();
    if (defender_creature.GetType() == 1 && IsBlocking() &&
        IS_THING_ALIVE(pthing_hitting) && !block_counter) {
        TCombatAbilityList::const_iterator it;
        if (GetCombatAbilityData(it, COMBAT_ABILITY_INTERFACE_ATTACK_BLOCK_COUNTER) &&
            it->second.second.IsValid(*it->second.first, PeekWorld().GetWorldSeed()))
            counter_attack = true;
    }

    CTCPhysicalShieldSpell* ptc_shield;
    if (!hit_params.IsDrainHealStrike() && defender_creature.GetTC(&ptc_shield) &&
        ptc_shield->IsShieldActive()) {
        if (!hit_params.IsDrainStrike())
            hit_params.SetToCauseRecoil(true);
        if (ppoint_of_hit)
            ptc_shield->OnHitAtPos(*ppoint_of_hit);
        ptc_shield->TakeDamage(hit_params.GetDamageToThing(defender_creature));
        if (ptc_shield->GetAllowReactionsOnHit(pthing_hitting)) {
            hit_params.ClearAllDamage();
            hit_params.SetGuaranteeHitResponse(true);
        } else {
            hit_params.SetBlocked(true);
        }
        return;
    }
    if (NCreatureMode::IsToDeflectAllHits(defender_creature)) {
        hit_params.SetBlocked(true);
        hit_params.SetToCauseRecoil(true);
        CDefPointer<const CMaterialDef> pmaterial_def;
        long material_index = hit_params.GetPHitLocation()->GetArmourDef()->BlockMaterial;
        if (material_index > 0)
            GetDefinitionManager().GetDef(material_index, &pmaterial_def);
        hit_params.SetPHitMaterialDef(pmaterial_def);
        return;
    }
    if (!((hit_location.IsDefendableAgainstMelee() && hit_params.IsMeleeStrike()) ||
          (hit_location.IsDefendableAgainstShots() && hit_params.IsShotStrike())) ||
        !IS_THING_ALIVE(pthing_hitting) || !IsToBlock(hit_params))
        return;
    if (BlockingNumFrames <= 6) {
        CTCHeroExperience* ptc_hero_exp;
        if (GetParentThing().GetTC(&ptc_hero_exp))
            ptc_hero_exp->AddSuccessfulBlockParry();
    }
    bool block_effective = false;
    CThing* pweapon = hit_params.GetPWeapon();
    CThing* phitter = pthing_hitting;
    CDefPointer<const CMaterialDef> pblock_material;
    const CTCCarrying* ptc_carrying;
    const CThing* pcarried = defender_creature.PeekTC(&ptc_carrying)
        ? ptc_carrying->GetThingInPrimarySlot() : 0;
    CDefPointer<const CMaterialDef> pcarried_material;
    if (IS_THING_ALIVE(pcarried) && (pcarried->GetTypeFlags() & 4)) {
        const CThingGameObject* pgo = GFDebugPDynamicCast(pcarried, (const CThingGameObject*)0);
        pcarried_material = pgo->PeekPMaterial();
    }
    const CTCWeapon* ptc_weapon;
    if (!IS_THING_ALIVE(pcarried)) {
        long block_mat = hit_params.GetPHitLocation()->GetArmourDef()->BlockMaterial;
        CDefPointer<const CMaterialDef> pdefender_block_material;
        if (block_mat > 0)
            GetDefinitionManager().GetDef(block_mat, &pdefender_block_material);
        block_effective = true;
        pblock_material = pdefender_block_material;
    } else if (pcarried->PeekTC(&ptc_weapon) &&
               ptc_weapon->IsAbleToBeUsedToBlock(defender_creature, phitter, pweapon)) {
        if (ptc_weapon->IsBlockBrokenOnBlock(defender_creature, phitter, pweapon)) {
            block_effective = true;
            block_counter = true;
            pblock_material = pcarried_material;
        } else {
            block_effective = true;
            pblock_material = pcarried_material;
        }
    }
    if (block_effective && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 8)) {
        CThingCreatureBase& attacker_creature = *GFDebugDynamicCast(phitter, (CThingCreatureBase*)0);
        bool attacker_recoil = true;
        if (pdefender_action && pdefender_action->IsStrikeAction()) {
            defender_creature.PerformBlockedReaction(0, true);
        } else {
            if (defender_creature.GetType() != 2 || IsStartingToBlock())
                defender_creature.FinishCurrentAction();
            if (block_counter && defender_creature.SetCurrentAction(
                    CCreatureAction_BlockBroken(defender_creature, attacker_creature))) {
                attacker_recoil = false;
                hit_params.SetBlockCounter(true);
            } else if (counter_attack && defender_creature.SetCurrentAction(
                         CCreatureAction_BlockCounter(defender_creature, attacker_creature))) {
            } else {
                defender_creature.SetCurrentAction(
                    CCreatureAction_BlockRespond(defender_creature, attacker_creature, 0));
                EWeaponClass wclass = WC_UNARMED;
                if (IS_THING_ALIVE(pweapon) && pweapon->PeekTC(&ptc_weapon))
                    wclass = ptc_weapon->GetWeaponClass();
                SetBlocking();
                SetBlocked(attacker_creature);
                TCombatAbilityList::const_iterator it;
                if (GetBlockAbilityIterator(it, wclass)) {
                    const CCombatAbilityData& comb_ability_dat = *it->second.first;
                    long const_fps = CMainGameComponent::GetConstantFPS();
                    float seconds_duration = comb_ability_dat.SecondsDuration;
                    BlockHoldTimer = (long)(const_fps * seconds_duration);
                }
                CTCCombat* ptc_combat;
                if (attacker_creature.GetTC(&ptc_combat))
                    ptc_combat->SetMeleeFlourishValid(true);
            }
        }
        hit_params.SetBlocked(true);
        hit_params.SetPHitMaterialDef(pblock_material);
        if (attacker_recoil)
            hit_params.SetToCauseRecoil(true);
    }
}