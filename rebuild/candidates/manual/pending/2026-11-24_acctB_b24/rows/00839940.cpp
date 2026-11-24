// 0x00839940 CThingGameObject::OnHit (B24B8, overlay hdrg)
// Grade: RELOCATION_MATCH (B24B8, 2026-11-24).
// Recovered asserts (not fitted): FableWin.exe's debug OnHit (0x027EC280, read by Account A) carries eleven DoErrorMessage asserts, in
//   source order: (1) pweapon==NULL || IS_THING_ALIVE(pweapon), (2) phit_location != NULL, (3) PeekDefinitionManager().GetDef(mat_def,
//   &pmat_def), (4) IS_THING_ALIVE(pexplosion) "Can't create explosion for explosive strike", (5) pexplosion->GetTCDriver(&ptcd_explosion),
//   (6) PeekDefinitionManager().GetDef(mat_def, &pmaterial_def), (7) phitter->GetTC(&ptc_guard), (8..10) GetDef("MATERIAL_LIGHTNING" /
//   "MATERIAL_DIVINE_WRATH" / "MATERIAL_UNHOLY_POWER", &phitting_material), (11) GetDef(pTCHitLocations->GetPDefaultHitLocation()->
//   GetArmourDef()->DamageMaterial, &phitting_material). The eight success forms (3, 5..11) keep their call in retail, so in the release
//   build they are the call statement itself (VERIFY-style: the expression is evaluated, nothing else is emitted); the three condition
//   asserts (1, 2, 4) are release dead arms (fable_assert.h FABLE_RETAIL_ASSERT_DEAD_ARM), which emit no code.
// Inferred: the release spelling of the eight success forms. Measured (c2 trace, inl/gh.py; window: summonable bitset<274>::test site
//   g <= ~8990 expanded as retail, hero-experience IS_THING_ALIVE g >= ~9001 called as retail): this form g 8675 / 9025 RELOCATION_MATCH;
//   all eleven as dead arms + the kept call (the 0x00627BE0 success-form spelling) g 9254 / 9499 DIFFER(5214v5195) 149 hunks (frame 0xA4);
//   asserts 1..6 that way g 8876 / 9226 RELOCATION_MATCH, adding 7 or 8..11 DIFFER; success forms as `if (!(call)) { dead arm }`: DIFFER(5198v5195)
//   51 hunks (frame-slot / register / block-order shifts; IL not traced). The VERIFY-style form here is one spelling that fits (hero-site margin ~24 IL, about one arm), not
//   proven to be the only one. Positions: the coordinator gave the ORDER only; asserts 1, 2 and 4 are placed from that order
//   next to the value they test (unverified). Unverified: assert 7's position -- here it is the guard-test condition itself (FableWin may test the
//   GetTC result twice; FableWin.exe is not on the lane).
#define FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CCREATUREDEF_STATGETTYPENAME
#define FABLE_RETAIL_CHEROEXPERIENCEDEF_STATGETTYPENAME
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
// CThingGameObject::OnHit(CHitParameters& hit_params) -- apply damage, combat effects and hit notifications
// Leaf: ?OnHit@CThingGameObject@@UAEXAAVCHitParameters@@@Z
// Retail: 0x00839940, 5195 bytes, thiscall.
// Name: the call at 0x0083A37B binds 0x006EA430 under the landed spelling DeregisterCombatant; NAME_FIXES.tsv proves it is
//   CCombatManager::SetAttackerAsBeenStruck (Ego_r / FableWin object order and size) -- respell this caller with that fix.
// Inline-budget: FABLE_FORCEINLINE on original HasTC<T> only; native839c5b..839d58 expands window type check. Plain and scoped depth255 reject cost190/score2015; S200 changes unrelated sites.
// Inline-budget: HasTC(ETCInterfaceType) wrapper forced only for this row; native late calls are bitset::test410de0, not HasTC. S28 over-expanded alive helpers; depth cannot override the observed cost rejection.
// Identity: both original PDBs, donor027ec280 and nativecallers667d44/7039b4.
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BREAKDOORS
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_BLOCKING
#define FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_WEAPONUSE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT_HASTC_CLASSTYPE
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT
#define FABLE_RETAIL_CHITPARAMETERS_ONHIT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGMANAGER_COLLISIONDEBUG
#define FABLE_RETAIL_CPLAYER_GETNUMBER
#define FABLE_RETAIL_CTHINGCREATUREBASE_BLOCKEDREACTION
#define FABLE_RETAIL_CTHINGGAMEOBJECT_HITQUERIES
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCDEXPLOSION_ONHIT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCLASSFACTORY
#include "engine/CThingCreatureBase.h"
#include "engine/CHitParameters.h"
#include "engine/CDefClassBase.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingManager.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCombatManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCOwnedEntity.h"
#include "stlport/fable_map.h"


#include "engine/CMaterialDef.h"
#include "engine/CArmourDef.h"
#include "engine/CHeroExperienceDef.h"
#include "engine/CCreatureDef.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCActionUseWindow.h"
#include "engine/CTCElectrocuteOnTouch.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCFlammable.h"
#include "engine/CTCBerserk.h"
#include "engine/CTCCombat.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CTCMultiStrike.h"
#include "engine/CTCGuard.h"
#include "engine/CTCHero.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCAbsorbHits.h"
#include "engine/CTCWound.h"
#include "engine/CTCDegradableBase.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCSummonableCreature.h"
#include "engine/CTCGhostSword.h"
#include "engine/NHeroStatModifierEvent_CHitThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CInputManager.h"
#include "engine/CPlayerGui.h"
#include "engine/ThingAlive.h"
#include "fable_assert.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEnginePrimitiveLine.h"

extern const float DELTA_NOTIONAL_ZERO;
// Original public/data symbol records and donor027ee54f prove the global spelling.
extern CInputManager* GPInputManager;
namespace { CEnginePrimitiveHandle hit_pos; CEnginePrimitiveHandle hit_dir; }
namespace NThingHelpers { void FABLE_FASTCALL CreateEffectOnHit(const CThing&,const CHitParameters&,const CWorldMap&,float); }
void CThingGameObject::OnHit(CHitParameters& hit_params)
{
    // Debug: a red line up from the hit_params pos, a green one along the hit_params dir.
    if (GetThingManager().IsDrawCombatCollisionDebug() && hit_params.IsLocalised()) {
        C3DVector pos = hit_params.GetHitPosition();
        {
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(hit_pos,
                CEnginePrimitiveLine(0, pos, pos + C3DVector(0.0f, 0.0f, 0.1f), CRGBColour(255, 0, 0),
                                     0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD), 0);
        }
        if (hit_params.IsDirectional()) {
            C3DVector dir = hit_params.GetHitDirection();
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(hit_dir,
                CEnginePrimitiveLine(0, pos, dir + pos, CRGBColour(0, 255, 0),
                                     0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD), 0);
        }
    }
    RemoveTC(TCI_FLOURISH_TARGET);
    CThing* pweapon = hit_params.GetPWeapon();
    FABLE_RETAIL_ASSERT(pweapon == NULL || IS_THING_ALIVE(pweapon));                    // FableWin assert 1
    CThing* phitter = hit_params.GetPThingHitting();
    NHitLocations::CHitLocation* phit_location = hit_params.GetPHitLocation();
    CTCHitLocations* ptc_hit_locations;
    if (!phit_location) {
        if (!GetTC(&ptc_hit_locations))
            return;
        phit_location = ptc_hit_locations->GetPDefaultHitLocation();
        if (!phit_location)
            return;
        hit_params.SetHitLocation(*phit_location);
    }
    FABLE_RETAIL_ASSERT(phit_location != NULL);                                         // FableWin assert 2
    hit_params.SetHitLocation(*phit_location);
    CDefPointer<const CMaterialDef> pmat_def = PeekPMaterial();
    if (GetTypeFlags() & 8) {
        long mat_def = phit_location->GetArmourDef()->DamageMaterial;
        if (mat_def > 0)
            PeekDefinitionManager().GetDef(mat_def, &pmat_def);                         // FableWin assert 3 (success form)
    }
    hit_params.SetPHitMaterialDef(pmat_def);
    CTCPerceiveThing* ptc_perceive;
    if (IS_THING_ALIVE(phitter) && GetTC(&ptc_perceive))
        ptc_perceive->OnHit(*phitter);
    if (hit_params.IsMeleeStrike()) {
        if (IS_THING_ALIVE(phitter)) {
            if (phitter->GetTypeFlags() & 8) {
                CThingCreatureBase& attacker_creature = *static_cast<CThingCreatureBase*>(phitter);
                attacker_creature.MakeNoise("SIMPLE_NOISE");
                CTCElectrocuteOnTouch* ptc_electrocute;
                if (GetTC(&ptc_electrocute)) {
                    ptc_electrocute->OnHit(*phitter);
                    hit_params.SetHitNegated(true);
                    return;
                }
            }
            if (IS_THING_ALIVE(phitter)) {
                const CTCHeroExperience* ptc_hero_experience;
                if ((HasTC(TCI_DOOR) || HasTC<CTCActionUseWindow>()) && phitter->PeekTC(&ptc_hero_experience) && !ptc_hero_experience->IsAbleToBreakDownDoors())
                    hit_params.SetHitNegated(true);
            }
        }
        CTCWeapon* ptc_weapon;
        if (IS_THING_ALIVE(pweapon) && IS_THING_ALIVE(phitter) && pweapon->GetTC(&ptc_weapon)) {
                long explosion_index = ptc_weapon->GetExplosionType();
                if (explosion_index > 0) {
                    CThing* pexplosion = CTCDExplosion::Create(explosion_index, phitter->GetPos());
                    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pexplosion));                    // FableWin assert 4
                    if (IS_THING_ALIVE(pexplosion)) {
                        CTCDExplosion* ptcd_explosion;
                        pexplosion->GetTCDriver(&ptcd_explosion);                       // FableWin assert 5 (success form)
                        ptcd_explosion->SetSpecificThingToHit(*this);
                    }
                }
            }
            CTCFlammable* ptc_my_flammable;
            CTCFlammable* ptc_weapon_flammable;
            if (IS_THING_ALIVE(pweapon) && GetTC(&ptc_my_flammable) && pweapon->GetTC(&ptc_weapon_flammable)) {
                if (ptc_my_flammable->IsOnFire() && !ptc_weapon_flammable->IsOnFire())
                    ptc_weapon_flammable->SetOnFire();
                else if (!ptc_my_flammable->IsOnFire() && ptc_weapon_flammable->IsOnFire())
                    ptc_my_flammable->SetOnFire();
            }
    }
    if (!HasTC(TCI_BATTLE_CHARGE) && !HasTC(TCI_ASSASSIN_RUSH)) {
    CTCBerserk* ptc_berserk;
    if (GetTC(&ptc_berserk))
        ptc_berserk->IncreaseBerserkDamageToMeMultiplier();
    CTCCombat* ptc_combat = 0;
    if (GetTC(&ptc_combat))
        ptc_combat->SetStruck(*phit_location, hit_params);
    float damage_absorbed = 0.0f;
    if (hit_params.IsBlocked()) {
        CTCHeroExperience* ptc_hero_experience;
        if (GetTC(&ptc_hero_experience))
            ptc_hero_experience->AddSuccessfulBlockParry();
    } else {
        if (IsToNegateAllHits()) {
            hit_params.SetHitNegated(true);
            if (GetTypeFlags() & 8) {
                long mat_def = phit_location->GetArmourDef()->BlockMaterial;
                if (mat_def > 0) {
                    CDefPointer<const CMaterialDef> pmaterial_def;
                    PeekDefinitionManager().GetDef(mat_def, &pmaterial_def);            // FableWin assert 6 (success form)
                    hit_params.SetPHitMaterialDef(pmaterial_def);
                }
            }
        }
        if (ptc_combat && ptc_combat->IsBlocking())
            ptc_combat->UnsetBlocking();
        CTCEmoteIcon* ptc_emote;
        if (GetTC(&ptc_emote))
            ptc_emote->SetEmoteIcon(CTCEmoteIcon::EMOTE_PRIORITY_NONE, 0, 0.0f, 1.0f, 0xFF);
        if (!phit_location->IsToPlayGetHitResponse())
            hit_params.SetPlayHitResponse(false);
        CDefPointer<const CArmourDef> armour_def = phit_location->GetArmourDef();
        hit_params.SetAsModifiedByArmour(*this, armour_def);
        if (hit_params.IsHitNegated()) {
            hit_params.SetPlayHitResponse(false);
            hit_params.SetToCauseRecoil(true);
        }
        damage_absorbed = hit_params.GetDamageToThing(*this);
        {
        if (!hit_params.IsHitNegated()) {
            CTCMultiStrike* ptc_multistrike;
            if (hit_params.IsMeleeStrike() && ((GetTypeFlags() & 8) || IsDamageable()) && IS_THING_ALIVE(pweapon) && pweapon != phitter
                && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_multistrike) && !ptc_multistrike->IsStrikeStarted())
                ptc_multistrike->StartMultiHit(hit_params, this);
            bool allowed_to_kill = true;
            CTCGuard* ptc_guard;
            const CTCHero* ptc_hero;
            if (GetType() == 2 && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_guard)) {   // FableWin assert 7 (success form; position unverified)
                if (ptc_guard->IsAttackingToFineRatherThanKill())
                    allowed_to_kill = false;
            } else if ((GetTypeFlags() & 8) && IS_THING_ALIVE(phitter) && phitter->PeekTC(&ptc_hero) && !ptc_hero->IsWeaponUseAllowed())
                allowed_to_kill = false;
            CTCOwnedEntity* tc_owned_entity;
            if (HasTC(TCI_DOOR) && GetTC(&tc_owned_entity)) {
                CThing* powner = tc_owned_entity->GetPOwner();
                CTCVillageMember* tc_village_member;
                if (powner && powner->GetTC(&tc_village_member)) {
                    CTCVillage* ptc_village = tc_village_member->GetPVillage();
                    if (ptc_village && ptc_village->GetVillageLimbo())
                        allowed_to_kill = false;
                }
            }
            bool already_dead = GetHealth() < DELTA_NOTIONAL_ZERO;
            CTCAbsorbHits* ptc_absorb_hits;
            if (!GetTC(&ptc_absorb_hits) || ptc_absorb_hits->DecrementHitsRemaining() < 0)
                ModifyHealth(damage_absorbed, allowed_to_kill);
            if (!already_dead && GetHealth() < DELTA_NOTIONAL_ZERO) {
                CThing* pweapon_here = pweapon;
                const NThingShot::CTCShotDefinitionBase* ptc_shot_base;
                if (IS_THING_ALIVE(pweapon_here) && pweapon_here->PeekTC(&ptc_shot_base))
                    pweapon_here = ptc_shot_base->GetPWeapon();
                CTCWeapon* ptc_weapon;
                if (IS_THING_ALIVE(pweapon_here) && pweapon_here->GetTC(&ptc_weapon))
                    ptc_weapon->IncrementNumThingsKilled(this);
            }
            if (GetTypeFlags() & 8) {
                CThingCreatureBase& creature = *static_cast<CThingCreatureBase*>(this);
                if (creature.IsInMeleeAsAttacker())
                    creature.GetCombatManager().DeregisterCombatant(creature);
            }
            if ((GetTypeFlags() & 8) || HasTC(TCI_CREATURE_GENERATOR)) {
                float display_damage = -damage_absorbed;
                if (IsDamageable() && display_damage > DELTA_NOTIONAL_ZERO) {
                    if (GetPlayerManager().IsThingMainPlayer(*this))
                        CPlayerGui::Get().AddDamageChange(display_damage, true);
                    else if (IS_THING_ALIVE(phitter) && GetPlayerManager().IsThingMainPlayer(*phitter))
                        CPlayerGui::Get().AddDamageChange(display_damage, false);
                }
            }
            if (damage_absorbed < 0.0f) {
                CTCHeroExperience* ptc_hero_experience;
                if (GetTC(&ptc_hero_experience))
                    ptc_hero_experience->SetAsHit();
                if (hit_params.IsLocalised()) {
                    const C3DVector& hit_position = hit_params.GetHitPosition();
                    CTCWound* ptc_wound;
                    if (GetTC(&ptc_wound))
                        ptc_wound->AddPossibleWoundGivenStrikePoint(hit_position);
                    if (hit_params.IsDirectional()) {
                        const C3DVector& hit_direction = hit_params.GetHitDirection();
                        CTCDegradableBase* tc_degradable;
                        if (GetTC(&tc_degradable))
                            tc_degradable->OnHit(hit_position, hit_direction);
                    }
                }
            }
        } else {
            CTCHeroOpinionDeedLog* ptc_deed_log;
            if (hit_params.IsMeleeStrike() && IS_THING_ALIVE(phitter) && !(GetTypeFlags() & 8) && phitter->GetTC(&ptc_deed_log))
                ptc_deed_log->PostDeed((EOpinionDeedType)0x49, GetPos(), this, 0, (EOpinionPostDeedType)2,
                                false, false, 0, false, 0);
        }
        if (IS_THING_ALIVE(phitter) && ptc_combat)
            ptc_combat->OnHit(*phitter);
        phit_location->ProcessHitLocationCode(*this, hit_params);
        }
    }
    {
    CTCHeroStats* ptc_stats;
    if (!hit_params.IsHitNegated() && !hit_params.IsBlocked() && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_stats) && IsGivingHeroStatChangesOnBeingHit()) {
        CDefPointer<const CCreatureDef> pcreature_def;
        if (GetPBaseDef()->GetSubDef(&pcreature_def)) {
            bool unarmed = true;
            if (IS_THING_ALIVE(pweapon) && pweapon != phitter)
                unarmed = false;
            CDefPointer<const CHeroExperienceDef> phero_exp_def;
            if (phitter->GetPBaseDef()->GetSubDef(&phero_exp_def)) {
                for (std::map<EDamageAttribute,float>::const_iterator it = hit_params.GetDamageMap().begin(); it != hit_params.GetDamageMap().end(); ++it) {
                    const EDamageAttribute att = it->first;
                    std::map<EDamageAttribute,CHeroStatIncreaseData>::const_iterator it_lookup = phero_exp_def->StatIncreasePerHitType.find(att);
                    if (it_lookup != phero_exp_def->StatIncreasePerHitType.end()) {
                        for (std::map<EHeroExperienceStatCategory,float>::const_iterator it2 = it_lookup->second.StatIncreases.begin();
                             it2 != it_lookup->second.StatIncreases.end(); ++it2) {
                            const float increase = it2->second;
                            const EHeroExperienceStatCategory stat = it2->first;
                            NHeroStatModifierEvent::CHitThing hit_event(pcreature_def->GetStatsDef());
                            hit_event.SetAsUnarmed(unarmed);
                            hit_event.SetStatToIncrement(stat);
                            hit_event.SetStatIncrementAmount(increase);
                            bool summoned_creature = false;
                            CTCSummonableCreature* ptc_summonable;
                            if (GetTC(&ptc_summonable) && ptc_summonable->GetHasBeenSummoned())
                                summoned_creature = true;
                            bool ghost_sword = CTCGhostSword::IsThingGhostSword(this);
                            if (!summoned_creature && !ghost_sword)
                                ptc_stats->InformOfEvent(hit_event);
                        }
                    }
                }
            }
        }
    }
    CTCHeroExperience* ptc_hero_experience;
    if (!hit_params.IsHitNegated() && !hit_params.IsBlocked() && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_hero_experience) && IsAddToComboMultiplierOnHit())
        ptc_hero_experience->AddSuccessfulStrikeWithHitParams(hit_params);
    CTCEntityEvents* ptc_entity_events;
    if (GetTC(&ptc_entity_events) && IS_THING_ALIVE(phitter)) {
        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBy(hit_params));
        if (phitter->HasTC(TCI_BERSERK))
            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_BERSERK));
    }
    if (hit_params.IsToCauseRecoil() && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 8)
        && !(IS_THING_ALIVE(pweapon) && pweapon->HasTC(TCI_SHOT_DEFINITION)))
    {
        CThingCreatureBase& attacker_creature = *static_cast<CThingCreatureBase*>(phitter);
        attacker_creature.PerformBlockedReaction(0, true);
    }
    if (hit_params.IsMeleeStrike()) {
        CDefPointer<const CMaterialDef> pmaterial = hit_params.GetPHitMaterialDef();
        if (pmaterial.Get() && pmaterial->OnHitRumbleLevel != 0.0f && IS_THING_ALIVE(phitter) && phitter->GetType() == 2
            && phitter->PeekPlayer().GetNumber() == PeekPlayerManager().GetMainPlayer().GetNumber())
            GPInputManager->GetJoystick(phitter->GetJoystickDeviceNumber()).AddRumble(3,
                CJoystick::CInitRumble(pmaterial->OnHitRumbleLevel, 0.5f, 0.0f, pmaterial->OnHitRumbleFalloff, false), true);
    }
    CTCSoundPlayer* ptc_sound;
    if (GetTC(&ptc_sound) && hit_params.IsLocalised()) {
        CDefPointer<const CMaterialDef> phitting_material;
        if (IS_THING_ALIVE(pweapon) && (pweapon->GetTypeFlags() & 4))
            phitting_material = static_cast<CThingPhysical*>(pweapon)->PeekPMaterial();
        if (!phitting_material.Get() && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 4)) {
            phitting_material = static_cast<CThingPhysical*>(phitter)->PeekPMaterial();
            if (hit_params.IsLightningStrike())
                PeekDefinitionManager().GetDef("MATERIAL_LIGHTNING", &phitting_material);   // FableWin assert 8 (success form)
            else if (hit_params.IsDivineWrathStrike())
                PeekDefinitionManager().GetDef("MATERIAL_DIVINE_WRATH", &phitting_material);   // FableWin assert 9 (success form)
            else if (hit_params.IsUnholyPowerStrike())
                PeekDefinitionManager().GetDef("MATERIAL_UNHOLY_POWER", &phitting_material);   // FableWin assert 10 (success form)
            else {
                CTCHitLocations* pTCHitLocations;
                if ((phitter->GetTypeFlags() & 8) && phitter->GetTC(&pTCHitLocations))     // FableWin assert 11 (success form)
                    PeekDefinitionManager().GetDef(pTCHitLocations->GetPDefaultHitLocation()->GetArmourDef()->DamageMaterial, &phitting_material);
            }
        }
        CDefPointer<const CMaterialDef> phit_material = hit_params.GetPHitMaterialDef();
        if (phit_material.Get())
            ptc_sound->PlayCollisionSound(hit_params.GetHitPosition(), phit_material, phitting_material);
    }
    NThingHelpers::CreateEffectOnHit(*this, hit_params, PeekWorldMap(), damage_absorbed);
    }
    return;
    }
    hit_params.SetPlayHitResponse(false);
}
