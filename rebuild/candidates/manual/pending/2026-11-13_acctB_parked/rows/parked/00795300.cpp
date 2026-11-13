#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
// CTCUnholyPower::FrameUpdate() -- per-frame driver of the Unholy Power spell: fade-out wait / self-removal once Finished; build-up
// Grade: DIFFER(9501v9268) hunks=459 -- attempt 3: EVERY call/expand split now = retail (ordered call list identical) except (a) the two cancel-sound arms are not tail-merged (ours pushes the zero register edi before the stamina-arm CDefString::GetString call and passes eax; retail calls GetString 0x00415DD0 first, then `lea; push 0`), (b) K1 `events->AddEvent(new CEventDied)` tail-duplicated in ours (zero register ebp). Remaining code residue: frame 0x240 v 0x230 with a different local layout (ours `xor edi,edi` at entry, retail only at the release start 0x79570C), stamina loop aligned in ours, operator- loads Z,Y,X where retail loads X,Y,Z (sweep-1 range test 0x79625E). Unproven pieces: __assume in CDefPointer/CIntelligentPointer operator-> (overlay opt-ins), sweep-1 test via GetSquaredMagnitude (XYZ) -- both only lift c2 g to retail's spell-over decisions.
// Inline-budget: see $W/NOTES.md (regime-0 score c1*(1/f+6)/7 v T 74; pen sz*(a+g)/18000 once a+g>9000).
//   0x00797A60 in sweep 1 (ours expands it), the sweep-2 iterator ++ 0x00793A00 and ~list 0x006F09D0 after sweep 2 (ours expand),
//   and in the spell-over loop the IP copy 0x0078E5B0, IS_THING_ALIVE 0x006961F0, bitset::test(0xF5) and SetLeaveExperienceOrbsOverride
//   0x0078DDB0 x2 (ours expand: c2 size penalty starts ~2 decisions later than retail's); 2nd cancel sound calls ??B not GetString; C2DBoxI
//   store order; GetPItem placement around the OnHit vcalls.
//   (env-change counter, build-up action, cancel on button release or empty stamina, mark creatures in range); release (release
//   effects, rumble, sounds, release action + one-shot object hits, victim effects, drop out-of-range creatures, re-spawn dead
//   effects, periodic damage ticks with kill-and-explode); spell over (final hit on every marked creature, finish move, finish sound).
// Leaf: ?FrameUpdate@CTCUnholyPower@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00795300, 9268 bytes, public virtual thiscall (vtable 0x0126BFDC slot 9; the manifest label
//   CTCDivineWrath::FrameUpdate is wrong -- the CTCDivineWrath twin is 0x0078FD60, same text with 11 constants changed).
// Callees (by address, see $W/DOSSIER.md section 3): CWillButtonHelperTimer::FrameUpdate 0x008EDDD0, CThing::RemoveTC 0x004C9840,
//   SetPTarget 0x00793D20, CCreatureAction_* ctors 0x008BD2E0/0x008BD330/0x008BD390, SetCurrentAction 0x006644F0, ~CCombatActionBase
//   0x0062E900, PlaySoundW 0x007940F0, StopSound 0x00794180, FinishCurrentAction 0x00662650, UnsheatheWeapon 0x00793EC0, FinishMove
//   0x00794C10, GetAllThingsInRoughArea 0x00792E50 / 0x007930B0, map find 0x007978C0 / operator[] 0x00798330 / erase 0x00797A60,
//   CHitParameters 0x00789238 / 0x00789358 / AddDamageWithAttribute 0x00868CA0, CTCWillResponse queries 0x007E45D0/0x007E46D0/0x007E4680,
//   CreateParticleEffect 0x0072AB70, FadeOrKillEffect 0x0072ABD0, CTCDRumble::Create 0x007F1B70, ResetCreatureHitState 0x00793E00.
// Inline-budget: c2 trace ($W/tools/inl, v6.txt): T=74, size penalty from g~9000; see $W/NOTES.md.
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSERT_ASSUME
#define FABLE_RETAIL_CDEFPOINTER_ASSERT_ASSUME
#define FABLE_RETAIL_CTCUNHOLYPOWER_INLINE_ACCESSORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCUNHOLYPOWER_IMPLICIT_INFO_DTORS
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_GETSTATE_INLINE
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
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
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
#define FABLE_RETAIL_CHITPARAMETERS_SPECIALABILITY
#define FABLE_RETAIL_CTCWILLRESPONSE_THINGQUERIES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCREATUREACTION_UNHOLYPOWER
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
#define FABLE_RETAIL_C2DBOXI_RADIUS_INLINE
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
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#include "engine/CThingCreatureBase.h"
#include "engine/CTCUnholyPower.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CHitParameters.h"
#include "engine/CCreatureAction_BuildUpUnholyPowerSpell.h"
#include "engine/CCreatureAction_ReleaseUnholyPowerSpell.h"
#include "engine/CCreatureAction_ReleaseAndLoopUnholyPowerSpell.h"
#include "engine/CTCWillResponse.h"
#include "engine/NCreatureMode.h"
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

class CThingFilter_CanBeHitByEpicSpell : public std::unary_function<CThing,bool> {   // no members (Ego_r.pdb size 1)
public:
    bool operator()(const CThing& test_thing) const;                    // 0x006A8D30
};
enum EQuakeStrength { };
enum EQuakeLength { };
class CTCDRumble {
public:
    static CThing* FABLE_FASTCALL Create(const C3DVector& pos, EQuakeStrength strength, EQuakeLength length, bool p3);   // 0x007F1B70
};
class CTCNymph : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_NYMPH; }   // FableWin.pdb inline accessor (0x01BBE09E)
    void StopFlyingAround(long p0);                                     // 0x0080F870
};
namespace NEntityEvents {
class CEventBase { public: virtual ~CEventBase(); };
class CEventHitBySpecialAbility : public CEventBase {
public:
    CEventHitBySpecialAbility(const CHitParameters& hit, EHeroAbility ability);   // 0x00811830
    unsigned char _pad04[0x18 - 0x04];
};
class CEventKilledCreature : public CEventBase {
public:
    CEventKilledCreature(const CThing& killer, const CThing& killed);  // 0x00811190
    unsigned char _pad04[0x0c - 0x04];
};
class CEventDied : public CEventBase {
public:
    CEventDied(CThing* killed_by, CThing* dead_body);                   // 0x00669300
    unsigned char _pad04[0x18 - 0x04];
};
}
class CTCCreatureTintEffect;
class CMessageEventDef : public CDefClassBase { };          // body not needed here (Ego_r.pdb CMessageEventDef : CDefClassBase)
extern const CCharString MAIN_FOCAL_POSITION_NAME;                      // 0x013BAC48

void CTCUnholyPower::FrameUpdate()
{
    if (!HasUnholyPowerActionInitialised())
        return;
    Timer.FrameUpdate();
    CThingCreatureBase* owner = (CThingCreatureBase*)&GetParentThing();
    CWillButtonHelperTimer::EButtonHeldState state = Timer.GetCurrentButtonHeldState();
    if (Finished) {
        if (NumFramesToWaitForFadeOut <= 0)
            owner->RemoveTC(TCI_UNHOLY_POWER);
        else
            --NumFramesToWaitForFadeOut;
        return;
    }
    if (IsCharging()) {
        if (++CurFramesForEnvChange >= NumFramesForEnvChange)
            SetPTarget();
        owner->SetCurrentAction(CCreatureAction_BuildUpUnholyPowerSpell(*owner, "ST_BUILD_UNHOLY_POWER"));
        if (state != CWillButtonHelperTimer::BEING_HELD_DOWN) {
            PlaySoundW(PDef->CancelSound[SpellLevel], false);
            owner->FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
        } else {
            CTCHeroStats* stats;
            owner->GetTC(&stats);
            long stamina_modify = StaminaModify;
            AccModifyStamina += PerFrameStaminaModify;
            while (AccModifyStamina > 1.0f) {
                AccModifyStamina -= 1.0f;
                ++stamina_modify;
            }
            stats->ModifyStaminaBy(-stamina_modify);
            if (stats->GetStamina() <= 0) {
                PlaySoundW(PDef->CancelSound[SpellLevel].GetString(), false);
                owner->FinishCurrentAction();
                UnsheatheWeapon();
                FinishMove();
            }
        }
        float radius = PDef->RadiusOfEffect[SpellLevel];
        C3DVector pos = owner->GetPos();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        long r = (long)(radius + 20.0f);
        C2DBoxI box(centre, r);
        std::list<CThingCreatureBase*> things;
        CThingFilter_OverlappingCircle circle(pos, radius);
        CThingFilter_CanBeHitByEpicSpell can_be_hit;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(box, CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(circle, can_be_hit), things);
        for (std::list<CThingCreatureBase*>::iterator it = things.begin(); it != things.end(); ++it) {
            CThingCreatureBase* creature = *it;
            if (creature == owner) continue;
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(creature)) continue;
            if (!CTCWillResponse::IsThingEpicSpellable(creature)) continue;
            if (!Attackable(creature)) continue;
            if (NCreatureMode::IsUnderground(*creature)) continue;
            if (LastFrameCreaturesHit.find(creature) != LastFrameCreaturesHit.end()) continue;
            if (!IS_THING_ALIVE(creature)) continue;
            if (!(creature->GetHealth() > 0.0f)) continue;
            LastFrameCreaturesHit[creature].ActualCreature = CIntelligentPointer<CThingCreatureBase>(creature);
            LastFrameCreaturesHit[creature].FramesToGetHit = 0;
            NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
            if (ga) {
                LastFrameCreaturesHit[creature].DefaultChannel = ga->PeekLightingChannel();
                ga->SetLightingChannel(SpellExclusionChannel, true, true);
            }
            CTCVolumeContainmentTracker* ptc_volume_contain = 0;
            if (creature->GetTC(&ptc_volume_contain))
                ptc_volume_contain->SetUpdateLightingChannels(false);
        }
        return;
    }

    CDefPointer<const CSpecialEffectsDef> effects;
    PDef->GetSubDef(&effects);
    if (CurFramesForEnvChange == NumFramesForEnvChange) {
        ++CurFramesForEnvChange;
        if (effects.Get()) {
            long effect = 0;
            bool found = effects->GetEffectFromString("RELEASE_EFFECT", effect);
            if (found)
                CTCSpecialAbilities::CreateParticleEffect(effect, owner->GetPos());
            found = effects->GetEffectFromString("RELEASE_COLUMN_EFFECT", effect);
            if (found)
                CTCSpecialAbilities::CreateParticleEffect(effect, owner->GetPos());
        }
        CTCDRumble::Create(owner->GetPos(), (EQuakeStrength)3, (EQuakeLength)0, false);
        PlaySoundW(PDef->ReleaseSound[SpellLevel], false);
        StopSound(LoopingSound);
        LoopingSound = PlaySoundW(PDef->ReleaseLoopSound[SpellLevel], false);
    }
    if (DurationOfSpellFrames > 0) {
        --DurationOfSpellFrames;
        float damage = PDef->DamagePerHit[SpellLevel];
        float radius = PDef->RadiusOfEffect[SpellLevel];
        C3DVector pos = owner->GetPos();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        long r = (long)(radius + 20.0f);
        C2DBoxI box(centre, r);
        if (!PerformedReleaseAction) {
            PerformedReleaseAction = owner->SetCurrentAction(CCreatureAction_ReleaseUnholyPowerSpell(*owner));
            if (PerformedReleaseAction) {
                CTCHeroExperience* experience;
                if (owner->GetTC(&experience)) {
                    CDefPointer<const CHeroAbilityDef> ability;
                    CGameDefinitionManager::Get().GetDef("HERO_ABILITY_SKILL_UNHOLY_POWER_SPELL", &ability);
                    experience->AddTrainableStatExperience(ability->Stat, ability->StatExperienceGainedOnUse);
                }
                std::list<CThingObject*> objects;
                CThingFilter_OverlappingCircle circle(pos, radius);
                CThingFilter_CanBeHitByEpicSpell can_be_hit;
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(box, CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(circle, can_be_hit), objects);
                std::list<CThingObject*>::iterator it = objects.begin();
                while (it != objects.end()) {
                    CThingObject* object = *it;
                    if (IS_THING_ALIVE(object) && !CTCWillResponse::IsThingNotBloodyWillableAtAll(object) && CTCWillResponse::IsThingEpicSpellable(object)) {
                        C3DVector dir = object->GetPos() - owner->GetPos();
                        dir.Normalise();
                        CHitParameters hit(0, owner, 0, object->GetPos() - dir, dir, true, false, false, false);
                        hit.AddDamageWithAttribute((EDamageAttribute)10, damage * 100.0f);
                        hit.SetCreateParticleEffectOnHit(false);
                        hit.SetEpicSpell(true);
                        bool explode = false;
                        if ((object->GetTypeFlags() & 8) && !static_cast<const CThingCreatureDef&>(object->PeekBaseDef()).LeaveDeadCreature
                            && CTCWillResponse::IsThingEpicPowerExplodable(object))
                            explode = true;
                        bool leave_orbs = true;
                        if (explode) {
                            leave_orbs = ((CThingCreatureBase*)object)->GetLeaveExperienceOrbsOverride();
                            ((CThingCreatureBase*)object)->SetLeaveExperienceOrbsOverride(false);
                        }
                        object->OnHit(hit);
                        if (explode)
                            ((CThingCreatureBase*)object)->SetLeaveExperienceOrbsOverride(leave_orbs);
                        CTCEntityEvents* events;
                        if (object->GetTC(&events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, (EHeroAbility)0x13));
                    }
                    ++it;
                }
            }
        } else {
            CCreatureAction_ReleaseAndLoopUnholyPowerSpell action(*owner);
            action.SetPItemRemoved(PItemRemoved);
            owner->SetCurrentAction(action);
        }
        CTCDRumble::Create(owner->GetPos(), (EQuakeStrength)0, (EQuakeLength)0, false);

        std::list<CThingCreatureBase*> things;
        CThingFilter_OverlappingCircle circle(pos, radius);
        CThingFilter_CanBeHitByEpicSpell can_be_hit;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(box, CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(circle, can_be_hit), things);
        for (std::list<CThingCreatureBase*>::iterator lit = things.begin(); lit != things.end(); ++lit) {
            CThingCreatureBase* creature = *lit;
            if (creature == owner) goto next_creature;
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(creature)) goto next_creature;
            if (!CTCWillResponse::IsThingEpicSpellable(creature)) goto next_creature;
            if (!Attackable(creature)) goto next_creature;
            if (NCreatureMode::IsUnderground(*creature)) goto next_creature;
            MapOfCreaturesToPerCreatureInfo::iterator found = LastFrameCreaturesHit.find(creature);
            if (found == LastFrameCreaturesHit.end()) {
                if (!IS_THING_ALIVE(creature)) goto next_creature;
                if (!(creature->GetHealth() > 0.0f)) goto next_creature;
                LastFrameCreaturesHit[creature].ActualCreature = CIntelligentPointer<CThingCreatureBase>(creature);
                LastFrameCreaturesHit[creature].FramesToGetHit = 0;
                long effect = 0;
                CCharString name("VICTIM_EFFECT");
                if (effects.Get() && effects->GetEffectFromString(name, effect)) {
                    C3DVector effect_pos = creature->GetPos();
                    NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                    if (ga)
                        ga->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0, effect_pos, 1.0f);
                    effect_pos.Z = creature->GetHeightZBelowAt(effect_pos, false, true);
                    LastFrameCreaturesHit[creature].Effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos));
                    PerEffectInfo info;
                    info.Position = effect_pos;
                    info.Effect = LastFrameCreaturesHit[creature].Effect;
                    HitEffects.push_back(info);
                }
                NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                if (ga) {
                    LastFrameCreaturesHit[creature].DefaultChannel = ga->PeekLightingChannel();
                    ga->SetLightingChannel(SpellExclusionChannel, true, true);
                }
                CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                if (creature->GetTC(&ptc_volume_contain))
                    ptc_volume_contain->SetUpdateLightingChannels(false);
            } else {
                if (!IS_THING_ALIVE(creature)) goto next_creature;
                CIntelligentPointer<CThing>& creature_effect = found->second.Effect;
                if (IS_THING_ALIVE(creature_effect)) goto next_creature;
                long effect = 0;
                CCharString name("VICTIM_EFFECT");
                if (effects.Get() && effects->GetEffectFromString(name, effect)) {
                    C3DVector effect_pos = creature->GetPos();
                    NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                    if (ga)
                        ga->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0, effect_pos, 1.0f);
                    effect_pos.Z = creature->GetHeightZBelowAt(effect_pos, false, true);
                    creature_effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos));
                    PerEffectInfo info;
                    info.Position = effect_pos;
                    info.Effect = creature_effect;
                    HitEffects.push_back(info);
                }
            }
        next_creature:
            ;
        }

        MapOfCreaturesToPerCreatureInfo::iterator it;
        for (it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ) {
            CIntelligentPointer<CThingCreatureBase> creature = it->second.ActualCreature;
            if (!(creature.Get() && !creature->IsDead())) {
                ++it;
            } else {
                float reach = radius + creature->GetPhysicsTC()->GetRadius();
                C3DVector offset = creature->GetPos() - owner->GetPos();
                if (offset.GetSquaredMagnitude() <= reach * reach + 5.0f
                    && !CTCWillResponse::IsThingNotBloodyWillableAtAll(creature.Get())
                    && CTCWillResponse::IsThingEpicSpellable(creature.Get())) {
                    ++it;
                } else {
                    ResetCreatureHitState(creature);
                    CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                    if (IS_THING_ALIVE(creature)) {
                        NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                        if (ga)
                            ga->SetLightingChannel(it->second.DefaultChannel, true, true);
                        CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                        if (creature->GetTC(&ptc_volume_contain))
                            ptc_volume_contain->SetUpdateLightingChannels(true);
                        creature->SetUnderInfluenceOfEpicSpell(false);
                    }
                    MapOfCreaturesToPerCreatureInfo::iterator old_it = it;
                    ++it;
                    LastFrameCreaturesHit.erase(old_it);
                }
            }
        }

        VectorOfCreaturesToPerEffectInfo::iterator eit = HitEffects.begin(), eend = HitEffects.end();
        while (eit != eend) {
            if (!IS_THING_ALIVE(eit->Effect)) {
                long effect = 0;
                CCharString name("VICTIM_EFFECT");
                if (effects.Get() && effects->GetEffectFromString(name, effect))
                    eit->Effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, eit->Position));
            }
            ++eit;
        }

        for (it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ) {
            CIntelligentPointer<CThingCreatureBase> creature = it->second.ActualCreature;
            if (IS_THING_ALIVE(creature)) {
                CTCNymph* nymph;
                if (creature->GetTC(&nymph))
                    nymph->StopFlyingAround(0);
                if (--it->second.FramesToGetHit <= 0) {
                    it->second.FramesToGetHit = (long)(CMainGameComponent::GetConstantFPS() * PDef->RandomFactorForNumSecsBetweenDamageHit) + NumFramesBetweenDamageHit;
                    C3DVector dir = creature->GetPos() - owner->GetPos();
                    dir.Normalise();
                    CHitParameters hit(0, owner, 0, creature->GetPos() - dir, dir, false, false, false, false);
                    hit.AddDamageWithAttribute((EDamageAttribute)10, damage);
                    hit.SetCreateParticleEffectOnHit(false);
                    hit.SetMoveBackOnHit(false);
                    hit.SetEpicSpell(true);
                    CTCCreatureModeManager* modes;
                    if (creature->GetTC(&modes)) {
                        if (!modes->IsModeActive(0x5F)) {
                            creature->FinishCurrentAction();
                            modes->AddMode(0x5F);
                        }
                    }
                    bool explode = false;
                    if (!static_cast<const CThingCreatureDef&>(creature->PeekBaseDef()).LeaveDeadCreature && CTCWillResponse::IsThingEpicPowerExplodable(creature.Get()))
                        explode = true;
                    bool leave_orbs = true;
                    if (explode) {
                        leave_orbs = creature->GetLeaveExperienceOrbsOverride();
                        creature->SetLeaveExperienceOrbsOverride(false);
                    }
                    creature->OnHit(hit);
                    if (!creature->HasTC(TCI_CREATURE_TINT_EFFECT))
                        creature->AddTC("CTCCreatureTintEffect", true, 0);
                    creature->SetUnderInfluenceOfEpicSpell(true);
                    if (explode)
                        creature->SetLeaveExperienceOrbsOverride(leave_orbs);
                    CTCEntityEvents* events;
                    if (creature->GetTC(&events))
                        events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, (EHeroAbility)0x13));
                }
            }
            if (creature.Get() && !creature->IsDead() && !(creature->GetHealth() <= 0.0f)) {
                ++it;
            } else {
            if (creature.Get()) {
                if (CTCWillResponse::IsThingEpicPowerExplodable(creature.Get())) {
                    CThing* hero = &GetParentThing();
                    GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)9, CMainGameComponent::GetConstantFPS(), creature->GetPhysicsTC()->GetPosition(), hero, creature.Get(), 0, 0, 0);
                    CTCEntityEvents* hero_events;
                    if (GetParentThing().GetTC(&hero_events))
                        hero_events->AddEvent(new NEntityEvents::CEventKilledCreature(GetParentThing(), *creature.Get()));
                    CTCDExperienceOrb::Create(*creature.Get());
                    CTCBase* container;
                    if (creature->GetTC(TCI_CONTAINER, &container))
                        container->OnDie();
                    creature->Kill(false);
                    CDefPointer<const CMessageEventDef> murder;
                    CThingCreatureBase* killer = creature->GetPLastAttackedByCreature();
                    if (killer && killer->IsPlayerCharacter()) {
                        if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &murder))
                            GetWorld().GetMessageEventManager().AddMessage(*murder, creature->PeekPos(), killer, creature.Get(), 0);
                    }
                    CTCWife* wife = 0;
                    creature->GetTC(&wife);
                    CTCHeroOpinionDeedLog* deeds;
                    if (killer && killer->GetTC(&deeds)) {
                        const CTCEnemy* enemy;
                        if (creature->PeekTC(TCI_ENEMY, &enemy)) {
                            const CFaction* faction = enemy->GetPFaction();
                            if (faction) {
                                deeds->PostDeed((EOpinionDeedType)9, creature->PeekPos(), creature.Get(), 0, (EOpinionPostDeedType)2, true, false, enemy->GetPFaction(), false, 0);
                                deeds->PostDeed((EOpinionDeedType)0x3A, creature->PeekPos(), creature.Get(), 0, (EOpinionPostDeedType)7, false, false, enemy->GetPFaction(), true, 0);
                            }
                        }
                    }
                    if (wife)
                        wife->Murdered();
                    CTCEntityEvents* events;
                    if (creature->GetTC(&events))
                        events->AddEvent(new NEntityEvents::CEventDied(killer, creature.Get()));
                    CDefPointer<const CSpecialEffectsDef> creature_effects;
                    bool found_effect;
                    long effect;
                    if (creature->GetPBaseDef()->GetSubDef(&creature_effects)) {
                        effect = 0;
                        found_effect = creature_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect);
                        if (found_effect) {
                            C3DVector effect_pos;
                            effect_pos = creature->GetPos();
                            NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                            if (ga)
                                ga->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, effect_pos, 1.0f);
                            CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        }
                    } else if (effects.Get()) {
                        effect = 0;
                        found_effect = effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect);
                        if (found_effect) {
                            C3DVector effect_pos;
                            effect_pos = creature->GetPos();
                            NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                            if (ga)
                                ga->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, effect_pos, 1.0f);
                            CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        }
                    }
                } else {
                    ResetCreatureHitState(creature);
                    CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                }
            }
            MapOfCreaturesToPerCreatureInfo::iterator old_it = it;
                    ++it;
                    LastFrameCreaturesHit.erase(old_it);
            }
        }
    } else if (DurationOfSpellFrames <= 0) {
    float damage = PDef->DamagePerHit[SpellLevel];
    for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(), endit = LastFrameCreaturesHit.end(); it != endit; ++it) {
        CIntelligentPointer<CThingCreatureBase> creature = it->second.ActualCreature;
        if (IS_THING_ALIVE(creature)) {
            CTCNymph* nymph;
            if (creature->GetTC(&nymph))
                nymph->StopFlyingAround(0);
            ResetCreatureHitState(creature);
            C3DVector dir = creature->GetPos() - owner->GetPos();
            dir.Normalise();
            CHitParameters hit(0, owner, 0, creature->GetPos() - dir, dir, true, false, false, false);
            hit.AddDamageWithAttribute((EDamageAttribute)10, damage);
            hit.SetCreateParticleEffectOnHit(false);
            hit.SetEpicSpell(true);
            bool explode = false;
            if (!static_cast<const CThingCreatureDef&>(creature->PeekBaseDef()).LeaveDeadCreature && CTCWillResponse::IsThingEpicPowerExplodable(creature.Get()))
                explode = true;
            bool leave_orbs = true;
            if (explode) {
                leave_orbs = creature->GetLeaveExperienceOrbsOverride();
                creature->SetLeaveExperienceOrbsOverride(false);
            }
            creature->OnHit(hit);
            if (!creature->HasTC(TCI_CREATURE_TINT_EFFECT))
                creature->AddTC("CTCCreatureTintEffect", true, 0);
            if (explode)
                creature->SetLeaveExperienceOrbsOverride(leave_orbs);
            CTCEntityEvents* events;
            if (creature->GetTC(&events))
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, (EHeroAbility)0x13));
            if (creature->GetHealth() <= 0.0f && CTCWillResponse::IsThingEpicPowerExplodable(creature.Get())) {
                CThing* hero = &GetParentThing();
                GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)9, CMainGameComponent::GetConstantFPS(), creature->GetPhysicsTC()->GetPosition(), hero, creature.Get(), 0, 0, 0);
                CTCEntityEvents* hero_events;
                if (GetParentThing().GetTC(&hero_events))
                    hero_events->AddEvent(new NEntityEvents::CEventKilledCreature(GetParentThing(), *creature.Get()));
                CTCDExperienceOrb::Create(*creature.Get());
                CTCBase* container;
                if (creature->GetTC(TCI_CONTAINER, &container))
                    container->OnDie();
                creature->Kill(false);
                CDefPointer<const CMessageEventDef> murder;
                CThingCreatureBase* killer = creature->GetPLastAttackedByCreature();
                if (killer && killer->IsPlayerCharacter()) {
                    if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &murder))
                        GetWorld().GetMessageEventManager().AddMessage(*murder, creature->PeekPos(), killer, creature.Get(), 0);
                }
                CTCWife* wife = 0;
                creature->GetTC(&wife);
                CTCHeroOpinionDeedLog* deeds;
                if (killer && killer->GetTC(&deeds)) {
                    const CTCEnemy* enemy;
                    if (creature->PeekTC(TCI_ENEMY, &enemy)) {
                        const CFaction* faction = enemy->GetPFaction();
                        if (faction) {
                            deeds->PostDeed((EOpinionDeedType)9, creature->PeekPos(), creature.Get(), 0, (EOpinionPostDeedType)2, true, false, enemy->GetPFaction(), false, 0);
                            deeds->PostDeed((EOpinionDeedType)0x3A, creature->PeekPos(), creature.Get(), 0, (EOpinionPostDeedType)7, false, false, enemy->GetPFaction(), true, 0);
                        }
                    }
                }
                if (wife)
                    wife->Murdered();
                CTCEntityEvents* died_events;
                if (creature->GetTC(&died_events))
                    died_events->AddEvent(new NEntityEvents::CEventDied(killer, creature.Get()));
                if (effects.Get()) {
                    long effect = 0;
                    bool found_effect = effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect);
                    if (found_effect) {
                        C3DVector effect_pos;
                        effect_pos = creature->GetPos();
                        NGraphicAppearance::CTCGraphicAppearance* ga = creature->GetTCGraphicAppearance();
                        if (ga)
                            ga->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, effect_pos, 1.0f);
                        CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                    }
                }
            }
        }
    }
    owner->FinishCurrentAction();
    UnsheatheWeapon();
    FinishMove();
    PlaySoundW(PDef->FinishSound[SpellLevel], false);
    }
}
