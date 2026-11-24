// CTCDivineWrath::FrameUpdate -- spell charge, release, damage and completion.
// Grade: see STATUS.tsv and final_audit/AUDITED_RESULT.json; immutable build final61_named_gumph.
// Leaf: ?FrameUpdate@CTCDivineWrath@@UAEXXZ
// Flags: (none)
// Donor-evidence: original locals, cast/lookup helpers, expression order and nine success/error conditions restored; see DONOR_ASSERT_STRINGS.json.
// Inferred: error reports compile away in release while their predicate expressions are evaluated once; no unsupported damage assertions or hidden dead-report arms are used.
// Bracket: normalise whole-sum and Newton product grouping preserve left-associated arithmetic and reproduce the retail schedule.
// Inline-budget: original C3DVector copy body/formal p is forced inline via the new shared-header opt-in, because retail expands that copy at every site.
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORCEINLINE_PASS25
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CTCDIVINEWRATH_INLINE_ACCESSORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCDIVINEWRATH_IMPLICIT_INFO_DTORS
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_STATE_INLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCDIVINEWRATH_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CTHINGCREATUREBASE_LEAVEORBS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_EPICSPELL_SETTERS
#define FABLE_RETAIL_CTCWILLRESPONSE_THINGQUERIES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCREATUREACTION_DIVINEWRATH
#define FABLE_RETAIL_CTCDEXPERIENCEORB_CREATE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CTCENEMY_FACTION
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDDEFMESSAGE
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
#define FABLE_RETAIL_CTCHEROSTATS_STAMINA
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_MARRIAGEEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#include "fable_assert.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CSpecialAbilitiesDivineWrathDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CHitParameters.h"
#include "engine/CCreatureAction_BuildUpDivineWrathSpell.h"
#include "engine/CCreatureAction_ReleaseDivineWrathSpell.h"
#include "engine/CCreatureAction_ReleaseAndLoopDivineWrathSpell.h"
#include "engine/CTCWillResponse.h"
#include "engine/NCreatureMode.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind2.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingObject.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCWife.h"
#include "engine/CTCContainerRewardHero.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"

#include "engine/CThingFilter_CanBeHitByEpicSpell.h"
enum EQuakeStrength {};
enum EQuakeLength {};
class CTCDRumble {
public:
    static CThing* FABLE_FASTCALL Create(const C3DVector& targetpos, EQuakeStrength strength,
                                         EQuakeLength length, bool p3); // 0x007F1B70
};
class CTCNymph : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() {
        return TCI_NYMPH;
    }                               // FableWin.pdb inline accessor (0x01BBE09E)
    void StopFlyingAround(long p0); // 0x0080F870
};
#include "engine/NEntityEvents_CEventHitBy.h"
namespace NEntityEvents {
class CEventKilledCreature : public CEventBase {
public:
    CEventKilledCreature(const CThing& my_attacker, const CThing& killed); // 0x00811190
    unsigned char _pad08[0x0c - 0x08];
};
class CEventDied : public CEventBase {
public:
    CEventDied(CThing* killed_by, CThing* dead_body); // 0x00669300
    unsigned char _pad08[0x18 - 0x08];
};
}
class CTCCreatureTintEffect : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() {
        return TCI_CREATURE_TINT_EFFECT;
    }
};
#include "engine/CMessageEventDef.h"
extern const CCharString MAIN_FOCAL_POSITION_NAME; // 0x013BAC48

void CTCDivineWrath::FrameUpdate() {
    if (!HasDivineWrathActionInitialised())
        return;
    Timer.FrameUpdate();
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    CWillButtonHelperTimer::EButtonHeldState buttonstate = Timer.GetCurrentButtonHeldState();
    if (Finished) {
        if (NumFramesToWaitForFadeOut <= 0)
            parent_creature.RemoveTC(TCI_DIVINE_WRATH);
        else
            --NumFramesToWaitForFadeOut;
        return;
    }
    if (IsCharging()) {
        if (++CurFramesForEnvChange >= NumFramesForEnvChange)
            SetPTarget();
        parent_creature.SetCurrentAction(
            CCreatureAction_BuildUpDivineWrathSpell(parent_creature, "ST_BUILD_DIVINE_WRATH"));
        if (buttonstate != CWillButtonHelperTimer::BEING_HELD_DOWN) {
            PlaySoundW(PDef->CancelSound[SpellLevel], false);
            parent_creature.FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
        } else {
            CTCHeroStats* ptc_hero_stats;
            if (!parent_creature.GetTC(&ptc_hero_stats))
                FABLE_RETAIL_ASSERT(false);
            long modify = StaminaModify;
            AccModifyStamina += PerFrameStaminaModify;
            while (AccModifyStamina > 1.0f) {
                AccModifyStamina -= 1.0f;
                ++modify;
            }
            ptc_hero_stats->ModifyStaminaBy(-modify);
            if (ptc_hero_stats->GetStamina() <= 0) {
                PlaySoundW(PDef->CancelSound[SpellLevel], false);
                parent_creature.FinishCurrentAction();
                UnsheatheWeapon();
                FinishMove();
            }
        }
        float search_radius = PDef->RadiusOfEffect[SpellLevel];
        C3DVector targetpos = parent_creature.GetPos();
        C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)(search_radius + 20.0f));
        std::list<CThingCreatureBase*> creatures;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
            search_area,
            CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                CThingFilter_OverlappingCircle(targetpos, search_radius), CThingFilter_CanBeHitByEpicSpell()),
            creatures);
        for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(), endit = creatures.end();
             it != endit; ++it) {
            CThingCreatureBase* something = *it;
            if (something == &parent_creature)
                continue;
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(something))
                continue;
            if (!CTCWillResponse::IsThingEpicSpellable(something))
                continue;
            if (!Attackable(something))
                continue;
            if (NCreatureMode::IsUnderground(*something))
                continue;
            MapOfCreaturesToPerCreatureInfo::iterator findit = LastFrameCreaturesHit.find(something);
            if (findit != LastFrameCreaturesHit.end())
                continue;
            if (!IS_THING_ALIVE(something))
                continue;
            if (!(something->GetHealth() > 0.0f))
                continue;
            LastFrameCreaturesHit[something].ActualCreature = something;
            LastFrameCreaturesHit[something].FramesToGetHit = 0;
            NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                something->GetTCGraphicAppearance();
            if (ptc_graphic_appearance) {
                LastFrameCreaturesHit[something].DefaultChannel =
                    ptc_graphic_appearance->PeekLightingChannel();
                ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
            }
            CTCVolumeContainmentTracker* ptc_volume_contain = 0;
            if (something->GetTC(&ptc_volume_contain))
                ptc_volume_contain->SetUpdateLightingChannels(false);
        }
    } else {

        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        if (!PDef->GetSubDef(&pspecial_effects))
            FABLE_RETAIL_ASSERT(false);
        if (CurFramesForEnvChange == NumFramesForEnvChange) {
            ++CurFramesForEnvChange;
            if (pspecial_effects.Get()) {
                long effect = 0;
                if (pspecial_effects->GetEffectFromString("RELEASE_EFFECT", effect)) {
                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, parent_creature.GetPos());
                }
                if (pspecial_effects->GetEffectFromString("RELEASE_COLUMN_EFFECT", effect)) {
                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, parent_creature.GetPos());
                }
            }
            CTCDRumble::Create(parent_creature.GetPos(), (EQuakeStrength)3, (EQuakeLength)0, false);
            PlaySoundW(PDef->ReleaseSound[SpellLevel], false);
            StopSound(LoopingSound);
            LoopingSound = PlaySoundW(PDef->ReleaseLoopSound[SpellLevel], false);
        }
        if (DurationOfSpellFrames > 0) {
            --DurationOfSpellFrames;
            float damage = PDef->DamagePerHit[SpellLevel];
            float search_radius = PDef->RadiusOfEffect[SpellLevel];
            C3DVector targetpos = parent_creature.GetPos();
            C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)(search_radius + 20.0f));
            if (!PerformedReleaseAction) {
                PerformedReleaseAction = parent_creature.SetCurrentAction(
                    CCreatureAction_ReleaseDivineWrathSpell(parent_creature));
                if (PerformedReleaseAction) {
                    CTCHeroExperience* ptc_experience;
                    if (parent_creature.GetTC(&ptc_experience)) {
                        CDefPointer<const CHeroAbilityDef> pdef;
                        CGameDefinitionManager::Get().GetDef("HERO_ABILITY_SKILL_DIVINE_WRATH_SPELL", &pdef);
                        ptc_experience->AddTrainableStatExperience(pdef->Stat,
                                                                   pdef->StatExperienceGainedOnUse);
                    }
                    std::list<CThingObject*> objects;
                    PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(
                        search_area,
                        CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                            CThingFilter_OverlappingCircle(targetpos, search_radius),
                            CThingFilter_CanBeHitByEpicSpell()),
                        objects);
                    std::list<CThingObject*>::iterator it = objects.begin(), endit = objects.end();
                    while (it != endit) {
                        CThingObject* something = *it;
                        if (IS_THING_ALIVE(something) &&
                            !CTCWillResponse::IsThingNotBloodyWillableAtAll(something) &&
                            CTCWillResponse::IsThingEpicSpellable(something)) {
                            C3DVector direction = something->GetPos() - parent_creature.GetPos();
                            direction.Normalise();
                            C3DVector point_of_hit = something->GetPos() - direction;
                            CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, true,
                                                     false, false, false);
                            hit_param.AddDamageWithAttribute((EDamageAttribute)9, damage * 100.0f);
                            hit_param.SetCreateParticleEffectOnHit(false);
                            hit_param.SetEpicSpell(true);
                            bool do_exp_override = false;
                            CThingGameObject* somethingobject = something;
                            if (somethingobject->GetTypeFlags() & 8) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                const CThingCreatureDef& creature_def = *GFDebugDynamicCast(
                                    &somethingcreaturebase->PeekBaseDef(), (const CThingCreatureDef*)0);
                                if (!creature_def.LeaveDeadCreature &&
                                    CTCWillResponse::IsThingEpicPowerExplodable(somethingcreaturebase))
                                    do_exp_override = true;
                            }
                            bool prev_exp_override_flag = true;
                            if (do_exp_override) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                prev_exp_override_flag =
                                    somethingcreaturebase->GetLeaveExperienceOrbsOverride();
                                somethingcreaturebase->SetLeaveExperienceOrbsOverride(false);
                            }
                            somethingobject->OnHit(hit_param);
                            if (do_exp_override) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                somethingcreaturebase->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                            }
                            CTCEntityEvents* ptc_entity_events;
                            if (something->GetTC(&ptc_entity_events))
                                ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                                    hit_param, HERO_ABILITY_DIVINE_WRATH_SPELL));
                        }
                        ++it;
                    }
                }
            } else {
                CCreatureAction_ReleaseAndLoopDivineWrathSpell finalaction(parent_creature);
                finalaction.SetPItemRemoved(PItemRemoved);
                parent_creature.SetCurrentAction(finalaction);
            }
            CTCDRumble::Create(parent_creature.GetPos(), (EQuakeStrength)0, (EQuakeLength)0, false);

            std::list<CThingCreatureBase*> creatures;
            PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
                search_area,
                CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                    CThingFilter_OverlappingCircle(targetpos, search_radius),
                    CThingFilter_CanBeHitByEpicSpell()),
                creatures);
            for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(), endit = creatures.end();
                 it != endit; ++it) {
                CThingCreatureBase* something = *it;
                if (something == &parent_creature)
                    continue;
                if (CTCWillResponse::IsThingNotBloodyWillableAtAll(something))
                    continue;
                if (!CTCWillResponse::IsThingEpicSpellable(something))
                    continue;
                if (!Attackable(something))
                    continue;
                if (NCreatureMode::IsUnderground(*something))
                    continue;
                MapOfCreaturesToPerCreatureInfo::iterator findit = LastFrameCreaturesHit.find(something);
                if (findit == LastFrameCreaturesHit.end()) {
                    if (!IS_THING_ALIVE(something))
                        continue;
                    if (!(something->GetHealth() > 0.0f))
                        continue;
                    LastFrameCreaturesHit[something].ActualCreature = something;
                    LastFrameCreaturesHit[something].FramesToGetHit = 0;
                    long effect = 0;
                    CCharString effectname("VICTIM_EFFECT");
                    if (pspecial_effects.Get() && pspecial_effects->GetEffectFromString(effectname, effect)) {
                        C3DVector effect_pos = something->GetPos();
                        const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                            something->GetTCGraphicAppearance();
                        if (ptc_graphic_appearance) {
                            if (!ptc_graphic_appearance->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0,
                                                                        effect_pos, 1.0f))
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        float groundZ = something->GetHeightZBelowAt(effect_pos, false, true);
                        effect_pos.Z = groundZ;
                        CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        LastFrameCreaturesHit[something].Effect = fx;
                        PerEffectInfo info;
                        info.Position = effect_pos;
                        info.Effect = LastFrameCreaturesHit[something].Effect;
                        HitEffects.push_back(info);
                    }
                    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                        something->GetTCGraphicAppearance();
                    if (ptc_graphic_appearance) {
                        LastFrameCreaturesHit[something].DefaultChannel =
                            ptc_graphic_appearance->PeekLightingChannel();
                        ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
                    }
                    CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                    if (something->GetTC(&ptc_volume_contain))
                        ptc_volume_contain->SetUpdateLightingChannels(false);
                } else {
                    if (!IS_THING_ALIVE(something))
                        continue;
                    if (IS_THING_ALIVE(findit->second.Effect))
                        continue;
                    long effect = 0;
                    CCharString effectname("VICTIM_EFFECT");
                    if (pspecial_effects.Get() && pspecial_effects->GetEffectFromString(effectname, effect)) {
                        C3DVector effect_pos = something->GetPos();
                        const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                            something->GetTCGraphicAppearance();
                        if (ptc_graphic_appearance) {
                            if (!ptc_graphic_appearance->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0,
                                                                        effect_pos, 1.0f))
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        float groundZ = something->GetHeightZBelowAt(effect_pos, false, true);
                        effect_pos.Z = groundZ;
                        CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        findit->second.Effect = fx;
                        PerEffectInfo info;
                        info.Position = effect_pos;
                        info.Effect = findit->second.Effect;
                        HitEffects.push_back(info);
                    }
                };
                ;
            }

            {
                MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin();
                if (it != LastFrameCreaturesHit.end())
                    for (; it != LastFrameCreaturesHit.end();) {
                        CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                        if (!IS_THING_ALIVE(something)) {
                            ++it;
                        } else {
                            float radiusPlusRadius = search_radius + something->GetPhysicsTC()->GetRadius();
                            C3DVector d = something->GetPos() - parent_creature.GetPos();
                            static const float GUMPH = 5.0f;
                            if (!((d.Y * d.Y + d.Z * d.Z + d.X * d.X) >
                                  radiusPlusRadius * radiusPlusRadius + GUMPH) &&
                                !CTCWillResponse::IsThingNotBloodyWillableAtAll(something.Get()) &&
                                CTCWillResponse::IsThingEpicSpellable(something.Get())) {
                                ++it;
                            } else {
                                ResetCreatureHitState(something);
                                CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                                if (IS_THING_ALIVE(something)) {
                                    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance)
                                        ptc_graphic_appearance->SetLightingChannel(it->second.DefaultChannel,
                                                                                   true, true);
                                    CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                                    if (something->GetTC(&ptc_volume_contain))
                                        ptc_volume_contain->SetUpdateLightingChannels(true);
                                    something->SetUnderInfluenceOfEpicSpell(false);
                                }
                                MapOfCreaturesToPerCreatureInfo::iterator tmp = it;
                                ++it;
                                LastFrameCreaturesHit.erase(tmp);
                            }
                        }
                    }
            }
            {
                VectorOfCreaturesToPerEffectInfo::iterator it = HitEffects.begin(), endit = HitEffects.end();
                while (it != endit) {
                    PerEffectInfo& info = *it;
                    if (!IS_THING_ALIVE(info.Effect)) {
                        long effect = 0;
                        CCharString effectname("VICTIM_EFFECT");
                        if (pspecial_effects.Get() &&
                            pspecial_effects->GetEffectFromString(effectname, effect))
                            info.Effect = CTCSpecialAbilities::CreateParticleEffect(effect, info.Position);
                    }
                    ++it;
                }
            }
            for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin();
                 it != LastFrameCreaturesHit.end();) {
                CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                if (IS_THING_ALIVE(something)) {
                    CTCNymph* ptc_nymph;
                    if (something->GetTC(&ptc_nymph))
                        ptc_nymph->StopFlyingAround(0);
                    if (--it->second.FramesToGetHit <= 0) {
                        it->second.FramesToGetHit = (long)(CMainGameComponent::GetConstantFPS() *
                                                           PDef->RandomFactorForNumSecsBetweenDamageHit) +
                                                    NumFramesBetweenDamageHit;
                        C3DVector direction = something->GetPos() - parent_creature.GetPos();
                        direction.Normalise();
                        C3DVector point_of_hit = something->GetPos() - direction;
                        CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, false,
                                                 false, false, false);
                        hit_param.AddDamageWithAttribute((EDamageAttribute)9, damage);
                        hit_param.SetCreateParticleEffectOnHit(false);
                        hit_param.SetMoveBackOnHit(false);
                        hit_param.SetEpicSpell(true);
                        CTCCreatureModeManager* ptc_cmm;
                        if (something->GetTC(&ptc_cmm)) {
                            if (!ptc_cmm->IsModeActive(0x5F)) {
                                something->FinishCurrentAction();
                                ptc_cmm->AddMode(0x5F);
                            }
                        }
                        bool do_exp_override = false;
                        const CThingCreatureDef& creature_def =
                            *GFDebugDynamicCast(&something->PeekBaseDef(), (const CThingCreatureDef*)0);
                        if (!creature_def.LeaveDeadCreature &&
                            CTCWillResponse::IsThingEpicPowerExplodable(something.Get()))
                            do_exp_override = true;
                        bool prev_exp_override_flag = true;
                        if (do_exp_override) {
                            prev_exp_override_flag = something->GetLeaveExperienceOrbsOverride();
                            something->SetLeaveExperienceOrbsOverride(false);
                        }
                        something->OnHit(hit_param);
                        CTCCreatureTintEffect* ptc_tint;
                        if (!something->GetTC(&ptc_tint))
                            ptc_tint = GFDebugPDynamicCast(something->AddTC("CTCCreatureTintEffect", true, 0),
                                                           (CTCCreatureTintEffect*)0);
                        something->SetUnderInfluenceOfEpicSpell(true);
                        if (do_exp_override)
                            something->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                        CTCEntityEvents* ptc_entity_events;
                        if (something->GetTC(&ptc_entity_events))
                            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                                hit_param, HERO_ABILITY_DIVINE_WRATH_SPELL));
                    }
                }
                if (something.Get() && !something->IsDead() && !(something->GetHealth() <= 0.0f)) {
                    ++it;
                } else {
                    if (something.Get()) {
                        if (CTCWillResponse::IsThingEpicPowerExplodable(something.Get())) {

                            GetWorld().GetMessageEventManager().AddMessage(
                                (EMessageEventType)9, CMainGameComponent::GetConstantFPS(),
                                something->GetPhysicsTC()->GetPosition(), &GetParentThing(), something.Get(),
                                0, 0, 0);
                            CTCEntityEvents* ptc_attacker_entity_events;
                            if (GetParentThing().GetTC(&ptc_attacker_entity_events))
                                ptc_attacker_entity_events->AddEvent(new NEntityEvents::CEventKilledCreature(
                                    GetParentThing(), *something.Get()));
                            CTCDExperienceOrb::Create(*something.Get());
                            CTCContainerRewardHero* ptc_reward;
                            if (something->GetTC(TCI_CONTAINER, &ptc_reward))
                                ptc_reward->OnDie();
                            something->Kill(false);
                            CDefPointer<const CMessageEventDef> pdef;
                            CThingCreatureBase* my_attacker = something->GetPLastAttackedByCreature();
                            if (my_attacker && my_attacker->IsPlayerCharacter()) {
                                if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &pdef))
                                    GetWorld().GetMessageEventManager().AddMessage(
                                        *pdef, something->PeekPos(), my_attacker, something.Get(), 0);
                                else
                                    FABLE_RETAIL_ASSERT_ARM(false);
                            }
                            CTCWife* tc_wife = 0;
                            something->GetTC(&tc_wife);
                            CTCHeroOpinionDeedLog* ptc_deed_log;
                            if (my_attacker && my_attacker->GetTC(&ptc_deed_log)) {
                                const CTCEnemy* pmy_enemy_tc;
                                if (something->PeekTC(TCI_ENEMY, &pmy_enemy_tc)) {
                                    if (pmy_enemy_tc->GetPFaction()) {
                                        ptc_deed_log->PostDeed((EOpinionDeedType)9, something->PeekPos(),
                                                               something.Get(), 0, (EOpinionPostDeedType)2,
                                                               true, false, pmy_enemy_tc->GetPFaction(),
                                                               false, 0);
                                        ptc_deed_log->PostDeed((EOpinionDeedType)0x3A, something->PeekPos(),
                                                               something.Get(), 0, (EOpinionPostDeedType)7,
                                                               false, false, pmy_enemy_tc->GetPFaction(),
                                                               true, 0);
                                    }
                                }
                                if (tc_wife)
                                    tc_wife->Murdered();
                            }
                            CTCEntityEvents* ptc_entity_events;
                            if (something->GetTC(&ptc_entity_events))
                                ptc_entity_events->AddEvent(
                                    new NEntityEvents::CEventDied(my_attacker, something.Get()));
                            CDefPointer<const CSpecialEffectsDef> pcreature_special_effects;
                            bool create_custom_explode_effect = false;
                            if (something->GetPBaseDef()->GetSubDef(&pcreature_special_effects)) {
                                long effect = 0;
                                if (pcreature_special_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT",
                                                                                   effect)) {
                                    C3DVector focus = something->GetPos();
                                    const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance) {
                                        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,
                                                                                    0, 0, focus, 1.0f))
                                            FABLE_RETAIL_ASSERT_ARM(false);
                                    }
                                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                                    create_custom_explode_effect = true;
                                }
                            }
                            if (!create_custom_explode_effect && pspecial_effects.Get()) {
                                long effect = 0;
                                if (pspecial_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect)) {
                                    C3DVector focus = something->GetPos();
                                    const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance) {
                                        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,
                                                                                    0, 0, focus, 1.0f))
                                            FABLE_RETAIL_ASSERT_ARM(false);
                                    }
                                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                                }
                            }
                        } else {
                            ResetCreatureHitState(something);
                            CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                        }
                    }
                    MapOfCreaturesToPerCreatureInfo::iterator tmp = it;
                    ++it;
                    LastFrameCreaturesHit.erase(tmp);
                }
            }
        } else if (DurationOfSpellFrames <= 0) {
            float damage = PDef->DamagePerHit[SpellLevel];
            for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(),
                                                           endit = LastFrameCreaturesHit.end();
                 it != endit; ++it) {
                CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                if (IS_THING_ALIVE(something)) {
                    CTCNymph* ptc_nymph;
                    if (something->GetTC(&ptc_nymph))
                        ptc_nymph->StopFlyingAround(0);
                    ResetCreatureHitState(something);
                    C3DVector direction = something->GetPos() - parent_creature.GetPos();
                    direction.Normalise();
                    C3DVector point_of_hit = something->GetPos() - direction;
                    CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, true, false,
                                             false, false);
                    hit_param.AddDamageWithAttribute((EDamageAttribute)9, damage);
                    hit_param.SetCreateParticleEffectOnHit(false);
                    hit_param.SetEpicSpell(true);
                    bool do_exp_override = false;
                    const CThingCreatureDef& creature_def =
                        *GFDebugDynamicCast(&something->PeekBaseDef(), (const CThingCreatureDef*)0);
                    if (!creature_def.LeaveDeadCreature &&
                        CTCWillResponse::IsThingEpicPowerExplodable(something.Get()))
                        do_exp_override = true;
                    bool prev_exp_override_flag = true;
                    if (do_exp_override) {
                        prev_exp_override_flag = something->GetLeaveExperienceOrbsOverride();
                        something->SetLeaveExperienceOrbsOverride(false);
                    }
                    something->OnHit(hit_param);
                    CTCCreatureTintEffect* ptc_tint;
                    if (!something->GetTC(&ptc_tint))
                        ptc_tint = GFDebugPDynamicCast(something->AddTC("CTCCreatureTintEffect", true, 0),
                                                       (CTCCreatureTintEffect*)0);
                    if (do_exp_override)
                        something->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                    CTCEntityEvents* ptc_entity_events;
                    if (something->GetTC(&ptc_entity_events))
                        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                            hit_param, HERO_ABILITY_DIVINE_WRATH_SPELL));
                    if (something->GetHealth() <= 0.0f &&
                        CTCWillResponse::IsThingEpicPowerExplodable(something.Get())) {

                        GetWorld().GetMessageEventManager().AddMessage(
                            (EMessageEventType)9, CMainGameComponent::GetConstantFPS(),
                            something->GetPhysicsTC()->GetPosition(), &GetParentThing(), something.Get(), 0,
                            0, 0);
                        CTCEntityEvents* ptc_attacker_entity_events;
                        if (GetParentThing().GetTC(&ptc_attacker_entity_events))
                            ptc_attacker_entity_events->AddEvent(
                                new NEntityEvents::CEventKilledCreature(GetParentThing(), *something.Get()));
                        CTCDExperienceOrb::Create(*something.Get());
                        CTCContainerRewardHero* ptc_reward;
                        if (something->GetTC(TCI_CONTAINER, &ptc_reward))
                            ptc_reward->OnDie();
                        something->Kill(false);
                        CDefPointer<const CMessageEventDef> pdef;
                        CThingCreatureBase* my_attacker = something->GetPLastAttackedByCreature();
                        if (my_attacker && my_attacker->IsPlayerCharacter()) {
                            if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &pdef))
                                GetWorld().GetMessageEventManager().AddMessage(
                                    *pdef, something->PeekPos(), my_attacker, something.Get(), 0);
                            else
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        CTCWife* tc_wife = 0;
                        something->GetTC(&tc_wife);
                        CTCHeroOpinionDeedLog* ptc_deed_log;
                        if (my_attacker && my_attacker->GetTC(&ptc_deed_log)) {
                            const CTCEnemy* pmy_enemy_tc;
                            if (something->PeekTC(TCI_ENEMY, &pmy_enemy_tc)) {
                                if (pmy_enemy_tc->GetPFaction()) {
                                    ptc_deed_log->PostDeed((EOpinionDeedType)9, something->PeekPos(),
                                                           something.Get(), 0, (EOpinionPostDeedType)2, true,
                                                           false, pmy_enemy_tc->GetPFaction(), false, 0);
                                    ptc_deed_log->PostDeed((EOpinionDeedType)0x3A, something->PeekPos(),
                                                           something.Get(), 0, (EOpinionPostDeedType)7, false,
                                                           false, pmy_enemy_tc->GetPFaction(), true, 0);
                                }
                            }
                            if (tc_wife)
                                tc_wife->Murdered();
                        }
                        CTCEntityEvents* ptc_entity_events;
                        if (something->GetTC(&ptc_entity_events))
                            ptc_entity_events->AddEvent(
                                new NEntityEvents::CEventDied(my_attacker, something.Get()));
                        if (pspecial_effects.Get()) {
                            long effect = 0;
                            if (pspecial_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect)) {
                                C3DVector focus = something->GetPos();
                                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                    something->GetTCGraphicAppearance();
                                if (ptc_graphic_appearance) {
                                    if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0,
                                                                                0, focus, 1.0f))
                                        FABLE_RETAIL_ASSERT_ARM(false);
                                }
                                CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                            }
                        }
                    }
                }
            }
            parent_creature.FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
            PlaySoundW(PDef->FinishSound[SpellLevel], false);
        }
    }
}
