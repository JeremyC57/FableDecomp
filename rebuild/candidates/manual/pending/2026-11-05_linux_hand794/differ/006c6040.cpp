// CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, EOpinionPostDeedType postType, const CFaction* faction, bool reverseFactionSearch) -- collect the deed's witnesses (eligible creatures, villagers inside a vandalised / broken-into building, unknown-threat witnesses, the creature itself, guards) and inform each creature (and its village) that does not know of the deed yet
// Grade: DIFFER(2793v2377) -- retail tail-merges the vandalism (CTCOwnedEntity::GetPOwner) and 0x35/9 (CTCVolumeContainmentTracker::GetPThingInside) village-witness branches at 0x006C6228 (esi = the intelligent pointer, GetType() == 3 and the villager loop shared); retail keeps the witnesses list node in ebx and tests 3 as an immediate (mine loads `mov ebx,3`), the villager IS_THING_ALIVE is inline in retail
// Next idea: make both branch tails allocate identically (one set of shared locals did not merge: try/ir_a3.cpp) or find the source that shares the tail (POwner is private, so no pointer-to-intelligent-pointer spelling); then the list-node register (ebx) and the std::list construction (retail stores the new node to two slots, no zero pre-store)
// Leaf: ?InformRelevantThingsOfDeed@CTCHeroOpinionDeedLog@@AAE_NJPAVCThingAICreature@@W4EOpinionPostDeedType@@PBVCFaction@@_N@Z
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
template<class _InputIter, class _Tp> inline _InputIter __fastcall find(_InputIter first, _InputIter last, const _Tp& val)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
}

template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing.Get() && !thing->IsDead(); }
template<class T> inline bool __fastcall IS_THING_ALIVE(T* const& thing) { return thing && !thing->IsDead(); }

class COpinionDeed {
public:
    EOpinionDeedType DeedType;                          // +0x00
    long DeedHandle;                                    // +0x04
    C3DVector Position;                                 // +0x08
    long FrameHappened;                                 // +0x14
    CIntelligentPointer<const CThing> PTarget;          // +0x18
    bool WasSearchingForWitnesses;                      // +0x20
    bool IsStillHappening;                              // +0x21
    bool IsCrimeReportedToVillage;                      // +0x22
    bool IsCrime;                                       // +0x23
    long ExtraData;                                     // +0x24
};

class COpinionDeedEffectsDef : public CDefClassBase {
    unsigned char _pad_0x25[0x7c - 0x25];
public:
    bool CanBeHeard : 1;                                // +0x7c
};

class CThingFilter_IsEligibleWitness {
public:
    CThingFilter_IsEligibleWitness(const COpinionDeed* deed, CThing& perpetrator, bool useHearing, CThingAICreature* recipient,
                                   const CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch);   // 0x006C8DE0
    bool operator()(const CThing& thing) const;         // 0x006C8E90
private:
    unsigned char _pad[0x2c];
};

class CTCEnemy : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENEMY; }
    bool IsEnemyOfFaction(const CFaction* faction) const;       // 0x0076C7D0
    bool IsAlliedWithFaction(const CFaction* faction) const;    // 0x0076C7F0
};

class CTCSleep : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SLEEP; }
    bool IsReadyToSleep;                                // +0x0c
    bool FastTrack;                                     // +0x0d
};



extern bool GDeedUnknownThreatWitnesses;                // 0x01375732 (descriptive)


// Same-TU definition (retail 0x006C58F0, before this row in the TU): /Ob2 inlines it here.
const COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) const
{
    std::map<long, COpinionDeed*>::const_iterator found = OpinionDeedMap.find(handle);
    if (found == OpinionDeedMap.end())
        return NULL;
    return found->second;
}

bool CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, EOpinionPostDeedType postType,
                                                       const CFaction* faction, bool reverseFactionSearch)
{
    bool informed = false;
    const COpinionDeed* deed = GetDeedFromHandle(handle);
    if (!deed)
        return informed;
    EOpinionDeedType type = deed->DeedType;
    std::list<CThingAICreature*> witnesses;
    std::list<CThingAICreature*> unknownThreatWitnesses;
    if (postType & 2) {
        PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(deed, GetParentThing(),
            PeekPDeedEffectsDef(type)->CanBeHeard, creature, faction, reverseFactionSearch, false), witnesses);
        if (type == OPINION_DEED_TYPE_CRIME_VANDALISM) {
            const CTCOwnedEntity* owned;
            if (deed->PTarget.Get() && deed->PTarget->PeekTC(&owned) && owned->GetPOwner() && owned->GetPOwner()->GetType() == 3) {
                CThingBuilding* building = static_cast<CThingBuilding*>(owned->GetPOwner());
                CTCVillageMember* member;
                if (building && building->GetTC(&member)) {
                    CTCVillage* village = member->GetPVillage();
                    if (village) {
                        for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = village->VillagePeople.begin();
                             it != village->VillagePeople.end(); ++it) {
                            CThingAICreature* villager = it->Get();
                            if (IS_THING_ALIVE(villager)
                                && (villager->GetWorkBuilding() == building || villager->GetHomeBuilding() == building)
                                && building->IsPointInside(villager->GetPos())) {
                                if (std::find(witnesses.begin(), witnesses.end(), villager) == witnesses.end())
                                    witnesses.push_back(villager);
                                CTCSleep* sleep;
                                if (villager->GetTC(&sleep))
                                    sleep->IsReadyToSleep = false;
                            }
                        }
                    }
                }
            }
        } else if (type == (EOpinionDeedType)0x35 || type == (EOpinionDeedType)9) {
            const CTCVolumeContainmentTracker* tracker;
            if (IS_THING_ALIVE(deed->PTarget) && deed->PTarget->PeekTC(&tracker) && IS_THING_ALIVE(tracker->GetPThingInside())
                && tracker->GetPThingInside()->GetType() == 3) {
                CThingBuilding* building = static_cast<CThingBuilding*>(tracker->GetPThingInside());
                CTCVillageMember* member;
                if (building && building->GetTC(&member)) {
                    CTCVillage* village = member->GetPVillage();
                    if (village) {
                        for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = village->VillagePeople.begin();
                             it != village->VillagePeople.end(); ++it) {
                            CThingAICreature* villager = it->Get();
                            if (IS_THING_ALIVE(villager)
                                && (villager->GetWorkBuilding() == building || villager->GetHomeBuilding() == building)
                                && building->IsPointInside(villager->GetPos())) {
                                if (std::find(witnesses.begin(), witnesses.end(), villager) == witnesses.end())
                                    witnesses.push_back(villager);
                                CTCSleep* sleep;
                                if (villager->GetTC(&sleep))
                                    sleep->IsReadyToSleep = false;
                            }
                        }
                    }
                }
            }
        }
    }
searched:
    bool added = false;
    if (GDeedUnknownThreatWitnesses && (type == (EOpinionDeedType)7 || type == (EOpinionDeedType)8 || type == (EOpinionDeedType)9
                                        || type == (EOpinionDeedType)0x35)) {
        if (postType & 2)
            PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(deed, GetParentThing(),
                PeekPDeedEffectsDef(type)->CanBeHeard, creature, faction, reverseFactionSearch, true), unknownThreatWitnesses);
        if ((postType & 1) && IS_THING_ALIVE(creature)) {
            CThingFilter_IsEligibleWitness filter(deed, GetParentThing(), PeekPDeedEffectsDef(type)->CanBeHeard, NULL, faction,
                                                  reverseFactionSearch, true);
            if (filter(*creature)) {
                unknownThreatWitnesses.push_back(creature);
                added = true;
            }
        }
    }
    if (IS_THING_ALIVE(creature) && !added) {
        const CTCEnemy* enemy;
        if (!faction || (creature->PeekTC(&enemy)
                         && (reverseFactionSearch ? enemy->IsEnemyOfFaction(faction) : enemy->IsAlliedWithFaction(faction)))) {
            CTCCreatureOpinionOfHero* opinion;
            if (creature->GetTC(&opinion) && opinion && ((unsigned char*)opinion->DeedMask_[0])[type])
                witnesses.push_back(creature);
        }
    }
    if (postType & 0x10) {
        std::vector<CThing*> guards;
        GetWorld().GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_GUARD), guards);
        for (std::vector<CThing*>::iterator it = guards.begin(); it != guards.end(); ++it) {
            CThing* thing = *it;
            if (IS_THING_ALIVE(thing) && thing->GetType() == 1) {
                CThingAICreature* guard = static_cast<CThingAICreature*>(thing);
                if (std::find(witnesses.begin(), witnesses.end(), guard) == witnesses.end())
                    witnesses.push_back(guard);
            }
        }
    }
    for (std::list<CThingAICreature*>::iterator it = witnesses.begin(); it != witnesses.end(); ++it) {
        CThingAICreature* witness = *it;
        if (((postType & 1) && witness == creature) || ((postType & 2) && witness != creature)) {
            CTCCreatureOpinionOfHero* opinion;
            if (witness->GetTC(&opinion) && !opinion->KnowsOfDeed(handle, type, true)) {
                opinion->InformOfDeed(type, handle, *PeekPDeedEffectsDef(type), false);
                informed = true;
            }
        }
        if (postType & 4) {
            const CTCVillageMember* member;
            if (witness->PeekTC(&member)) {
                CTCVillage* village = member->GetPVillage();
                if (village) {
                    CTCCreatureOpinionOfHero* opinion;
                    if (village->GetParentThing().GetTC(&opinion) && !opinion->KnowsOfDeed(handle, type, true)) {
                        opinion->InformOfDeed(type, handle, *PeekPDeedEffectsDef(type), false);
                        informed = true;
                    }
                }
            }
        }
    }
    if (GDeedUnknownThreatWitnesses) {
        for (std::list<CThingAICreature*>::iterator it = unknownThreatWitnesses.begin(); it != unknownThreatWitnesses.end(); ++it) {
            CTCCreatureOpinionOfHero* opinion;
            if ((*it)->GetTC(&opinion) && !opinion->KnowsOfDeed(handle, type, true)) {
                opinion->InformOfDeed(type, handle, *PeekPDeedEffectsDef(type), true);
                informed = true;
            }
        }
    }
    return informed;
}
