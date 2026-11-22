// CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType type, long handle, const COpinionDeedEffectsDef& def, bool flag) -- this creature learns of a hero deed: shop keepers and guards react to a witnessed crime (guards in a crime-free region raise the village's enmity instead), others turn enemy on a severe crime; a flirt seen by the spouse or a wife posts a jealousy deed; violence informs a wife and the boasting crowd; the deed is queued by reaction priority, its transient opinion offsets applied (rate-limited per deed type) and an unknown-threat opinion source marker placed
// Leaf: ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXW4EOpinionDeedType@@JABVCOpinionDeedEffectsDef@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00682810, 3442 bytes, `ret 10h`.
// Spellings (successor 2): EVIDENCED -- one early return for the deed log (`log = 0; if (hero) hero->GetTC(..., &log); if (log == 0)
//   return;`: c2 static frequency, push_back<long> expanded at 0x0068337F), cached Effects end (0x006833C9), IS_THING_ALIVE on a
//   const_cast temp of the wife (0x00682FA7), GetFrameBribeEnds() (Ego_r accessor; load at 0x00682BE5), implicit CIntelligentPointer
//   conversion for the marker (0x006834EF). FITTED -- `&GetParentThing()` in against_me (gives the ParentThing reload at 0x00682C11).
// Spellings (successor 10): EVIDENCED -- IS_THING_ALIVE(hero_thing) in the flirt and violence blocks with the shared engine/ThingAlive.h
//   body (retail keeps hero_thing in a frame home [esp+1Ch] while testing it from edi, 0x00682D45; Anniversary PDB lists `phero` as a
//   memory local in both blocks; c2 still calls the CThingAICreature* / CIntelligentPointer instantiations 0x004A88F0 / 0x004CC340 as
//   retail); crowd_group declared in its own block after `GetTC(...) && IsActive()` (retail slot [esp+30h] shared with the key temp,
//   0x0068314C); one && condition for the perceive / IsAwareOfThingIndirectly tests of a wife (hero reloaded from [esp+1Ch], 0x00683008).
//   FITTED (owner acceptance test device a; chosen by grading, no behaviour change) -- (1) `GetParentThing().GetType()` in the second
//   violence test and `&GetParentThing()` as the IsMember argument (the TC's own inline accessor, as in against_me; fix the scratch
//   registers of the boasting block, retail's duplicated epilogue at the deed==0 return 0x0068318F and the handle push 0x0068317F);
//   (2) the two effects assignments of the guard block inside `do { ... } while (!deed);` (deed is non-null there, so the body runs
//   once): retail keeps the redundant AddRef null test after `mov esi,eax` at 0x00682A3E, which VC7.1 folds in every acyclic spelling
//   (unique in Fable.exe; 4 folded twins) and keeps only when a back edge it removes late reaches the first assignment.
// Identity (two sources): Ego_r.pdb ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXW4EOpinionDeedType@@JABVCOpinionDeedEffectsDef@@_N@Z, and
//   the landed callers InformOfDeed(long, bool) 0x00683590 and CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6040.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_SPOUSE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_GETFRAMEBRIBEENDS
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CTCVILLAGE_INCREASEHEROENEMYOFGUARDS
#define FABLE_RETAIL_CTCSHOPKEEPER_WITNESSEDCRIME
#define FABLE_RETAIL_CTCGUARD_WITNESSEDCRIME
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_WIVES
#define FABLE_RETAIL_CHEROMARRIAGEINFO_CTOR
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL_ISACTIVE_INLINE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
#define FABLE_RETAIL_STLP_LIST
// NF246 (2026-11-22, the 0x00685BC0 family, nf243 NOTES section 2): FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (a noinline `~list() {}`,
//   so the leaf called ~list and bound it at 0x00685BC0) -> FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE (NF241's swap): ~list
//   inline, the leaf calls ~_List_base<const CThingAICreature*> 0x00685BC0 out of line, as retail (the 5-byte ~list jmp is 0x006860E0).
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_TRAITS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CRegionDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCGuard.h"
#include "engine/CTCWife.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CHeroMarriageInfo.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCCreatureGroup.h"
#include "engine/CTCCreatureGroupBoastingCrowd.h"
#include "engine/CTCBoastingCrowdControl.h"
#include "engine/CTCDInternalMarker.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionOfHeroDef.h"
#include "engine/COpinionPersonalityDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/ThingAlive.h"
// IS_THING_ALIVE<T> (engine/ThingAlive.h): retail calls the <CThingAICreature*> COMDAT 0x004A88F0 (2 sites) and the
// <CIntelligentPointer<CThing> > COMDAT 0x004CC340 (3 sites) and expands the <CThing*> tests of hero_thing inline.

   // 0x004A88F0 <CThingAICreature*>, 0x004CC340 <CIntelligentPointer<CThing> >
namespace NCreatureAI {
bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool flag);              // 0x008645E0
bool FABLE_FASTCALL IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool flag);    // 0x00864700
}
namespace NGlobalConsole { extern bool EnableUnknownThreatOpinionSources; }   // 0x01375733
namespace {
CCharString UNKNOWN_THREAT_OPINION_SOURCE("OPINION_SOURCE_UNKNOWN_THREAT", -1);   // 0x013BA898
}

void CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType type, long handle, const COpinionDeedEffectsDef& def, bool flag)
{
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroOpinionDeedLog* log = 0;
    if (hero)
        hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &log);
    if (log == 0)
        return;
    CDefPointer<const CRegionDef> region_def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
    bool crimes_count;
    if (!region_def.IsNull() && (region_def->UseCrimes || region_def->PermitCrimes))
        crimes_count = true;
    else
        crimes_count = false;
    const CTCVillageMember* village_member = 0;
    if (ParentThing->PeekTC(TCI_VILLAGE_MEMBER, &village_member)) {
        CTCVillage* village = village_member->GetPVillage();
        if (village && village->GetVillageLimbo())
            crimes_count = false;
    }
    if (ParentThing->HasTC(TCI_SHOP_KEEPER) && crimes_count) {
        long crime_handle;
        if (type > 0 && type < 10)
            crime_handle = handle;
        else if (type == 0x35) {
            const COpinionDeed* deed = log->GetDeedFromHandle(handle);
            if (deed == 0)
                goto shop_done;
            crime_handle = deed->ExtraData;
        } else
            goto shop_done;
        if (crime_handle != -1) {
            const COpinionDeed* crime = log->GetDeedFromHandle(crime_handle);
            if (crime && crime->IsCrime) {
                CTCShopKeeper* shop_keeper = 0;
                ParentThing->GetTC(TCI_SHOP_KEEPER, &shop_keeper);
                shop_keeper->WitnessedCrime(*crime, crime_handle);
            }
        }
    }
shop_done:
    if (ParentThing->HasTC(TCI_GUARD)) {
        if (!crimes_count) {
            const COpinionDeed* deed = log->GetDeedFromHandle(handle);
            CDefPointer<const COpinionDeedEffectsDef> effects;
            if (deed) {
                do {
                if (deed->DeedType > 0 && deed->DeedType < 10)
                    effects = log->PeekPDeedEffectsDef(deed->DeedType);
                if (deed->DeedType == 0x35)
                    effects = log->PeekPDeedEffectsDef((EOpinionDeedType)8);
                } while (!deed);
                if (!effects.IsNull()) {
                    if (effects->CrimeSeverity != 1) {
                        const CTCVillageMember* member;
                        if (ParentThing->PeekTC(TCI_VILLAGE_MEMBER, &member)) {
                            CTCVillage* village = member->GetPVillage();
                            if (village)
                                village->IncreaseHeroEnemyOfGuardsUntil((long)(effects->CrimeFineAmount * 0.2f));
                        }
                    }
                }
            }
        } else {
            long crime_handle;
            if (type > 0 && type < 10)
                crime_handle = handle;
            else if (type == 0x35)
                crime_handle = log->GetDeedFromHandle(handle)->ExtraData;
            else
                goto guard_done;
            if (crime_handle != -1) {
                const COpinionDeed* crime = log->GetDeedFromHandle(crime_handle);
                if (crime && crime->IsCrime) {
                    bool bribed = false;
                    CTCVillageMember* member;
                    if (ParentThing->GetTC(TCI_VILLAGE_MEMBER, &member)) {
                        CTCVillage* village = member->GetPVillage();
                        if (village)
                            bribed = GetWorld().GetFrame() < village->GetFrameBribeEnds();
                    }
                    bool against_me = crime->PTarget.Get() == &GetParentThing();
                    if (bribed && !against_me)
                        return;
                    CTCGuard* guard = 0;
                    ParentThing->GetTC(TCI_GUARD, &guard);
                    guard->WitnessedCrime(*crime, crime_handle);
                }
            }
        }
    } else {
        const COpinionDeed* deed = log->GetDeedFromHandle(handle);
        if (deed && deed->IsCrime) {
            CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef(deed->DeedType);
            if (effects->CrimeSeverity == 1) {
                if (village_member && village_member->GetPVillage() && village_member->GetPVillage()->IsHeroCriminal())
                    return;
            } else if (!PDef.IsNull() && PDef->WillAttackCriminals && effects->CrimeSeverity == 3) {
                HeroOpinionEnemy = true;
                ForcedAttitude = (EOpinionAttitudeType)3;
            }
        }
    }
guard_done:
    if (def.IsFlirting) {
        CThing* hero_thing = &log->GetParentThing();
        if (IS_THING_ALIVE(hero_thing)) {
            CThingAICreature* me = static_cast<CThingAICreature*>(ParentThing);
            if (me->GetType() == 1) {
                const CTCWife* wife;
                if (me->PeekTC(TCI_WIFE, &wife) && wife->IsMarriedToThePlayer())
                    goto flirt_done;
                {
                    const CTCTargetingBase* targeting;
                    if (!hero_thing->PeekTC(TCI_TARGETING, &targeting))
                        goto flirt_done;
                    CThing* target = targeting->GetPTargetedThing();
                    if (target) {
                        const CTCWife* target_wife;
                        if (targeting->GetPTargetedThing()->PeekTC(TCI_WIFE, &target_wife) && target_wife->IsMarriedToThePlayer())
                            goto flirt_done;
                    }
                }
                if (me->GetSex() == 2) {
                    CThingAICreature* spouse = me->GetSpouseCreature();
                    if (IS_THING_ALIVE(spouse)) {
                        const CTCPerceiveThing* perceive;
                        if (me->GetSpouseCreature()->PeekTC(TCI_PERCEIVE_THING, &perceive) && perceive->GetPerceivedThing() == hero_thing
                            && perceive->IsThingIgnoreFOVLineOfSight() && NCreatureAI::IsAwareOfThing(me->GetSpouseCreature(), hero_thing, true))
                            log->PostDeed((EOpinionDeedType)0x4b, log->GetParentThing().GetPos(), me,
                                          me->GetSpouseCreature(), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    }
                }
                if (me->IsCourtable()) {
                    const CTCHeroStats* stats;
                    if (hero_thing->PeekTC(TCI_HERO_STATS, &stats)) {
                        std::list<const CThingAICreature*> wives;
                        stats->GetWivesInMarriageState(wives, (EMarriageState)0);
                        for (std::list<const CThingAICreature*>::iterator it = wives.begin(); it != wives.end(); ++it) {
                            const CThingAICreature* wife_creature = *it;
                            if (!IS_THING_ALIVE(const_cast<CThingAICreature*>(wife_creature)))
                                continue;
                            const CTCPerceiveThing* perceive;
                            if (!wife_creature->PeekTC(TCI_PERCEIVE_THING, &perceive))
                                continue;
                            if (perceive->GetPerceivedThing() == hero_thing && perceive->IsThingIgnoreFOVLineOfSight()
                                && NCreatureAI::IsAwareOfThingIndirectly(wife_creature, hero_thing, true)) {
                                CHeroMarriageInfo info;
                                stats->PeekMarriageInfo((*it)->GetUniqueID(), info);
                                if (info.State != 5)
                                    log->PostDeed((EOpinionDeedType)0x55, hero_thing->GetPos(), me, const_cast<CThingAICreature*>(wife_creature), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                            }
                        }
                    }
                }
            }
        }
    }
flirt_done:
    if (type == 7 || type == 8 || type == 9 || type == 0x35) {
        CThing* hero_thing = &log->GetParentThing();
        if (IS_THING_ALIVE(hero_thing) && ParentThing->GetType() == 1) {
            CTCWife* wife;
            if (ParentThing->GetTC(TCI_WIFE, &wife))
                wife->OnHeroViolence();
        }
        if (GetParentThing().GetType() == 1) {
            CThing* hero_thing = &log->GetParentThing();
            CTCBoastingCrowdControl* crowd;
            if (hero_thing->GetTC(TCI_BOASTING_CROWD_CONTROL, &crowd) && crowd->IsActive()) {
                CTCCreatureGroupBoastingCrowd* crowd_group;
                if (crowd->GetCrowdGroup(*hero_thing, &crowd_group)
                    && crowd_group->IsMember(static_cast<CThingAICreature*>(&GetParentThing())))
                    crowd->OnHeroAttackCrowdMember();
            }
        }
    }
    if (log->GetDeedFromHandle(handle) == 0)
        return;
    bool apply_effects = true;
    if (def.MinimumSecondsBetweenEffectsForIndividual > 0) {
        long gap = def.MinimumSecondsBetweenEffectsForIndividual * CMainGameComponent::GetConstantFPS();
        long since = gap >= CWorld::GetFrame() ? 0 : CWorld::GetFrame() - gap;
        apply_effects = LastFrameAppliedDeedEffect[type] <= since;
        if (!apply_effects)
            goto effects_checked;
    }
    {
        const CTCWife* wife;
        bool courting_flirt = log->PeekPDeedEffectsDef(type)->IsFlirting && ParentThing->GetType() == 1
            && static_cast<CThingCreatureBase*>(ParentThing)->GetSex() == 2
            && static_cast<CThingCreatureBase*>(ParentThing)->GetCreatureType() == 2
            && ParentThing->PeekTC(TCI_WIFE, &wife) && !wife->IsMarriedToThePlayer() && !wife->IsCourtableAndNotBlockingIt();
        if (courting_flirt)
            apply_effects = false;
    }
effects_checked:
    long priority = log->PeekPDeedEffectsDef(type)->ReactionPriority;
    std::list<long>::iterator it = PendingDeeds.begin();
    while (it != PendingDeeds.end()) {
        const COpinionDeed* pending = log->GetDeedFromHandle(*it);
        if (pending == 0) {
            it = PendingDeeds.erase(it);
            continue;
        }
        if (log->PeekPDeedEffectsDef(pending->DeedType)->ReactionPriority >= priority) {
            PendingDeeds.insert(it, handle);
            goto inserted;
        }
        ++it;
    }
    PendingDeeds.push_back(handle);
inserted:
    if (apply_effects) {
        LastFrameAppliedDeedEffect[type] = GetWorld().GetFrame();
        for (std::list<COpinionTransientOffset>::const_iterator offset = def.Effects.begin(), end = def.Effects.end(); offset != end; ++offset) {
            if (!PPersonalityDef.IsNull())
                TransientOffsets.push_back(COpinionTransientOffset(*offset, PPersonalityDef->PersonalityTraits[offset->Opinion]));
            else
                TransientOffsets.push_back(COpinionTransientOffset(*offset));
        }
    }
    if (NGlobalConsole::EnableUnknownThreatOpinionSources && flag && log->GetDeedFromHandle(handle)) {
        if (!IS_THING_ALIVE(PUnknownThreatOpinionSource)) {
            long marker = GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT");
            PUnknownThreatOpinionSource = CTCDInternalMarker::Create(marker, ParentThing->GetPos());
            if (IS_THING_ALIVE(PUnknownThreatOpinionSource))
                GetWorld().GetOpinionReactionManager().AttachOpinionSource(*PUnknownThreatOpinionSource.Get(), UNKNOWN_THREAT_OPINION_SOURCE, 0);
        }
        if (IS_THING_ALIVE(PUnknownThreatOpinionSource)) {
            PUnknownThreatOpinionSource.Get()->GetPhysicsTC()->ResetPosition(log->GetParentThing().GetPos());
            FrameLastUpdatedUnknownThreat = GetWorld().GetFrame();
        }
    }
}