// 006c6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed
// Leaf: ?InformRelevantThingsOfDeed@CTCHeroOpinionDeedLog@@AAE_NJPAVCThingAICreature@@W4EOpinionPostDeedType@@PBVCFaction@@_N@Z
// Flags: /d2inlT80
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDTYPE_QUERY_PASS28
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_DEED_WITNESS_PASS28
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCSLEEP_WAKE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCOWNEDENTITY
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CDefClassBase.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

namespace std {
template<class _InputIter, class _Tp> inline _InputIter FABLE_FASTCALL find(_InputIter first, _InputIter last, const _Tp& val)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
}

#include "engine/ThingAlive.h"
#include "engine/EOpinionPostDeedType.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CThingFilter_IsEligibleWitness.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCSleep.h"
// FableWin PUB32 and GDATA32 at 04A40A15 and Ego_d PUB32 identify the native 01375732 feature switch.
namespace NGlobalConsole { extern bool EnableUnknownThreatDeedPostings; }


// Inline-budget: the native caller invokes 0069CE20 rather than expanding the intelligent-pointer instance.
#ifdef _MSC_VER
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool FABLE_FASTCALL IS_THING_ALIVE(const CIntelligentPointer<const CThing>& thing) {
    if (thing != 0) {
        if (!thing->IsDead()) return true;
    }
    return false;
}
const COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) const
{
    std::map<long, COpinionDeed*>::const_iterator found = OpinionDeedMap.find(handle);
    if (found == OpinionDeedMap.end())
        return NULL;
    return found->second;
}

bool CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed(long deed_handle, CThingAICreature* recipient, EOpinionPostDeedType post_type,
                                                       const CFaction* pfaction, bool reverse_faction_search)
{
    bool ret = false;
    const COpinionDeed* opinion_deed = GetDeedFromHandle(deed_handle);
    if (opinion_deed) {
        EOpinionDeedType deed_type = opinion_deed->DeedType;
        std::list<CThingAICreature*> informees;
        std::list<CThingAICreature*> unknown_threat_informees;
        if (post_type & OPINION_POST_DEED_TYPE_WITNESSES) {
            PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(opinion_deed, *ParentThing,
                PeekPDeedEffectsDef(deed_type)->CanBeHeard, recipient, pfaction, reverse_faction_search, false), informees);
            CThingBuilding* inform_buildings_villagers = 0;
            if (deed_type == OPINION_DEED_TYPE_CRIME_VANDALISM) {
                const CTCOwnedEntity* ptc_owned;
                if (opinion_deed->PTarget.Get() && opinion_deed->PTarget->PeekTC(&ptc_owned) &&
                    ptc_owned->GetPOwner() && ptc_owned->GetPOwner()->GetType() == 3)
                    inform_buildings_villagers = static_cast<CThingBuilding*>(ptc_owned->GetPOwner());
            } else if (deed_type == OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH || deed_type == OPINION_DEED_TYPE_CRIME_MURDER) {
                const CTCVolumeContainmentTracker* tracker;
                if (IS_THING_ALIVE(opinion_deed->PTarget) && opinion_deed->PTarget->PeekTC(&tracker) &&
                    IS_THING_ALIVE(tracker->GetPThingInside()) && tracker->GetPThingInside()->GetType() == 3)
                    inform_buildings_villagers = static_cast<CThingBuilding*>(tracker->GetPThingInside());
            }
            CTCVillageMember* ptc_village_member;
            if (inform_buildings_villagers && inform_buildings_villagers->GetTC(&ptc_village_member)) {
                CTCVillage* ptc_village = ptc_village_member->GetPVillage();
                if (ptc_village) {
                    const std::list<CIntelligentPointer<CThingAICreature> >& peeps = ptc_village->PeekVillagePeople();
                    for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator it = peeps.begin();
                         it != peeps.end(); ++it) {
                        CThingAICreature* creature = it->Get();
                        if (IS_THING_ALIVE(creature)
                            && (creature->GetWorkBuilding() == inform_buildings_villagers || creature->GetHomeBuilding() == inform_buildings_villagers)
                            && inform_buildings_villagers->IsPointInside(creature->GetPos())) {
                            if (std::find(informees.begin(), informees.end(), creature) == informees.end())
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                                informees.push_back(creature);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                            CTCSleep* tc_sleep;
                            if (creature->GetTC(&tc_sleep))
                                tc_sleep->SetIsNotReadyToSleep();
                        }
                    }
                }
            }
        }
        bool recipient_receiving_unknown_threat_posting = false;
        if (NGlobalConsole::EnableUnknownThreatDeedPostings && (deed_type == OPINION_DEED_TYPE_CRIME_ASSAULT || deed_type == OPINION_DEED_TYPE_CRIME_GBH || deed_type == OPINION_DEED_TYPE_CRIME_MURDER
                                            || deed_type == OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)) {
            if (post_type & OPINION_POST_DEED_TYPE_WITNESSES)
                PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(opinion_deed, *ParentThing,
                    PeekPDeedEffectsDef(deed_type)->CanBeHeard, recipient, pfaction, reverse_faction_search, true), unknown_threat_informees);
            if ((post_type & OPINION_POST_DEED_TYPE_RECIPIENT) && IS_THING_ALIVE(recipient)) {
                CThingFilter_IsEligibleWitness filter(opinion_deed, *ParentThing, PeekPDeedEffectsDef(deed_type)->CanBeHeard, NULL, pfaction,
                                                      reverse_faction_search, true);
                if (filter(*recipient)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    unknown_threat_informees.push_back(recipient);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    recipient_receiving_unknown_threat_posting = true;
                }
            }
        }
        if (IS_THING_ALIVE(recipient) && !recipient_receiving_unknown_threat_posting) {
            bool faction_ok = true;
            if (pfaction) {
                faction_ok = false;
                const CTCEnemy* ptc_enemy;
                if (recipient->PeekTC(&ptc_enemy))
                    faction_ok = reverse_faction_search ? ptc_enemy->IsEnemyOfFaction(pfaction) : ptc_enemy->IsAlliedWithFaction(pfaction);
            }
            if (faction_ok) {
                CTCCreatureOpinionOfHero* ptc_opinion;
                if (recipient->GetTC(&ptc_opinion) && ptc_opinion && ptc_opinion->IsDeedTypeEnabled(deed_type))
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    informees.push_back(recipient);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if (post_type & OPINION_POST_DEED_TYPE_GUARDS) {
            std::vector<CThing*> things;
            GetWorld().GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_GUARD), things);
            for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == 1) {
                    CThingAICreature* guard = static_cast<CThingAICreature*>(*it);
                    if (std::find(informees.begin(), informees.end(), guard) == informees.end())
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        informees.push_back(guard);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
        }
        if (!informees.empty()) {
            CThing* pvillage = 0;
            for (std::list<CThingAICreature*>::iterator it = informees.begin(), end_it = informees.end(); it != end_it; ++it) {
                if (((post_type & OPINION_POST_DEED_TYPE_RECIPIENT) && *it == recipient) || ((post_type & OPINION_POST_DEED_TYPE_WITNESSES) && *it != recipient)) {
                    CTCCreatureOpinionOfHero* ptc_opinion;
                    if ((*it)->GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                        ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), false);
                        ret = true;
                    }
                }
                if (post_type & OPINION_POST_DEED_TYPE_VILLAGE) {
                    const CTCVillageMember* ptc_village_member;
                    if ((*it)->PeekTC(&ptc_village_member)) {
                        CTCVillage* ptc_village = ptc_village_member->GetPVillage();
                        if (ptc_village) {
                            CTCCreatureOpinionOfHero* ptc_opinion;
                            if (ptc_village->GetParentThing().GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                                ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), false);
                                ret = true;
                            }
                        }
                    }
                }
            }
        }
        if (NGlobalConsole::EnableUnknownThreatDeedPostings && !unknown_threat_informees.empty()) {
            for (std::list<CThingAICreature*>::iterator it = unknown_threat_informees.begin(), end_it = unknown_threat_informees.end(); it != end_it; ++it) {
                CTCCreatureOpinionOfHero* ptc_opinion;
                if ((*it)->GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                    ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), true);
                    ret = true;
                }
            }
        }
    }
    return ret;
}