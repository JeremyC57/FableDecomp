// 006c6e80 CTCHeroOpinionDeedLog::FrameUpdate
// Leaf: ?FrameUpdate@CTCHeroOpinionDeedLog@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
// Retail 006c6e80. FableWin 01f298da, 01f29f12 and 01f2a023 each retain the
// VC6 for-scope guard. The three guarded loops below preserve their debug shape.
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_POST_DEED_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEED_FRAME_PASS29
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_TAKENOUTWEAPON
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_PUTWEAPONAWAY
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_NENTITYEVENTS_BASE_ORIGINAL_PASS26
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ISQUESTACTIVE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_PLAYER
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/COpinionDeed.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCCarrying.h"
#include "engine/CQuestManager.h"
#include "engine/NEntityEvents_CEventHeroHasBeenSpottedStealing.h"
#include "engine/CTCEntityEvents.h"
CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType type) const
{
    CDefPointer<const COpinionDeedEffectsDef> def;
    GetDefinitionManager().GetDef(DeedEffectsDefIndices[type], &def);
    return def;
}

const COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) const
{
    std::map<long,COpinionDeed*>::const_iterator it=OpinionDeedMap.find(handle);
    if (it==OpinionDeedMap.end()) return 0;
    return it->second;
}

void CTCHeroOpinionDeedLog::PlayerPutWeaponAway()
{
    if(WeaponOutDeedHandle!=-1) {
        RemoveDeedStillSearchingForWitnesses(WeaponOutDeedHandle);
        RemoveDeed(WeaponOutDeedHandle);
        RemoveCrimeFromVillage(WeaponOutDeedHandle);
        WeaponOutDeedHandle=-1;
    }
}

void CTCHeroOpinionDeedLog::FrameUpdate()
{
    const long SECONDS_BETWEEN_CLEAROUTS=3;
    long constant_fps=CMainGameComponent::GetConstantFPS();
    unsigned __int64 val=GetParentThing().GetUniqueID()+CWorld::GetFrame();
    if (!(val % static_cast<unsigned long>(SECONDS_BETWEEN_CLEAROUTS*constant_fps))) {
        typedef std::map<long,COpinionDeed*>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedMap.begin(),end_it=OpinionDeedMap.end(),next_it=it;it!=end_it;it=next_it) {
            ++next_it;
            COpinionDeed* opinion_deed=it->second;
            if (!opinion_deed->IsStillHappening) {
                long effective_frame_happened=opinion_deed->FrameHappened;
                if(opinion_deed->IsCrimeReportedToVillage && VillageForCrimes!=0) {
                    CTCVillage* ptc_village=0;
                    if(!VillageForCrimes->GetTC(&ptc_village)) { FABLE_RETAIL_ASSERT_ARM(false); }
                    if(ptc_village->GetFramePlayerLastSeenByGuard()>effective_frame_happened)
                        effective_frame_happened=ptc_village->GetFramePlayerLastSeenByGuard();
                }
                if(effective_frame_happened+PeekPDeedEffectsDef(opinion_deed->DeedType)->KnowledgeLifeInSeconds*constant_fps<CWorld::GetFrame()) {
                    delete it->second;
                    OpinionDeedMap.erase(it);
                }
            } else if ((opinion_deed->DeedType==OPINION_DEED_TYPE_CRIME_THEFT && !CQuestManager::Get().IsQuestActive("Expression_Steal"))
                   || (opinion_deed->DeedType==OPINION_DEED_TYPE_CRIME_LOCKPICKING && !CQuestManager::Get().IsQuestActive("Expression_Picklock"))) {
                FABLE_RETAIL_ASSERT_ARM(false);
                RemoveDeedStillSearchingForWitnesses(opinion_deed->DeedHandle);
            }
        }
    }
    const long SECONDS_BETWEEN_WITNESS_CHECKS=1;
    val+=5;
    if (!(val % static_cast<unsigned long>(constant_fps*SECONDS_BETWEEN_WITNESS_CHECKS))) {
        typedef std::list<CWitnessesSearchingDeed>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedsSearchingForWitnesses.begin(),end_it=OpinionDeedsSearchingForWitnesses.end();it!=end_it;++it)
            InformRelevantThingsOfDeed(it->DeedHandle,it->Recipient.Get(),it->PostType,it->PFaction,it->ReverseFactionSearch);
    }
    const long SECONDS_BETWEEN_WITNESS_CHECK_CLEAROUT=300;
    val+=5;
    if (!(val % static_cast<unsigned long>(constant_fps*SECONDS_BETWEEN_WITNESS_CHECK_CLEAROUT))) {
        typedef std::list<CWitnessesSearchingDeed>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedsSearchingForWitnesses.begin(),end_it=OpinionDeedsSearchingForWitnesses.end(),next_it=it;it!=end_it;it=next_it) {
            ++next_it;
            if(!GetDeedFromHandle(it->DeedHandle))
                OpinionDeedsSearchingForWitnesses.erase(it);
        }
    }
    CTCEntityEvents* ptc_entity_events;
    if(GetParentThing().GetTC(&ptc_entity_events)) {
        NEntityEvents::CEventHeroHasBeenSpottedPickpocketing* pevent=0;
        NEntityEvents::CEventHeroHasBeenSpottedStealing* psteal_event=0;
        if(ptc_entity_events->GetEvent(&psteal_event,NEntityEvents::CIsEventOfType(NEntityEvents::EVENT_HERO_HAS_BEEN_SPOTTED_STEALING)))
        {
            FABLE_RETAIL_ASSERT_ARM(false);
            ptc_entity_events->RemoveEvent(*psteal_event);
        }
    }
    CThingPlayerCreature* pplayer=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if(IS_THING_ALIVE(pplayer)) {
        bool has_weapon_out=CTCCarrying::IsThingCarryingWeapon(*pplayer);
        if (WeaponOutDeedHandle!=-1 && OpinionDeedMap.find(WeaponOutDeedHandle)==OpinionDeedMap.end())
            WeaponOutDeedHandle=-1;
        if(has_weapon_out) {
            if(WeaponOutDeedHandle==-1)PlayerTakenOutWeapon();
        } else PlayerPutWeaponAway();
    }
}