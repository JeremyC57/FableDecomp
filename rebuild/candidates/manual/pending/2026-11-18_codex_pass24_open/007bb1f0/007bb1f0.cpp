// CTCTrespasser::FrameUpdate -- track the occupied building, update trespassing state and escalate or decay its deeds.
// Grade: DIFFER(958v960) -- The original typed field and helper declarations reach the expected virtual and callees, with ten scheduling and CPU operand hunks beginning at the opinion-source setup.
// Leaf: ?FrameUpdate@CTCTrespasser@@UAEXXZ
// Retail: Fable.exe 0x007BB1F0, 960 bytes, virtual thiscall.
// Flags: none
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_SIMPLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTRESPASSER
#define FABLE_RETAIL_CTCTRESPASSER_FRAMEUPDATE
#define FABLE_RETAIL_CTCTRESPASSABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND
#define FABLE_RETAIL_CWORLD_OPINIONREACTIONMANAGER
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCTrespasser.h"
#include "engine/CTCTrespassable.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CWorld.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"

class CThingFilter_HasBuildingAsHomeOrWork {
public:
    explicit CThingFilter_HasBuildingAsHomeOrWork(CThing* building) : Building(building) { }
    CThing* Building;
};

void CTCTrespasser::FrameUpdate()
{
    CTCVolumeContainmentTracker* ptc_volume_containment_tracker;
    if (!ParentThing->GetTC(&ptc_volume_containment_tracker))
        return;
    CThing* thing_in = ptc_volume_containment_tracker->GetPThingInside();
    CTCTrespassable* trespassable;
    bool state_changed = thing_in != TrespassingThing.Get();
    if (state_changed || (thing_in && !thing_in->IsDead() && thing_in->GetTC(&trespassable)
                       && trespassable->Trespassable && !trespassable->IsBeingTrespassedIn)) {
        LeavingRegion();
        CTCTrespassable* ptc_trespassable;
        if (thing_in && !thing_in->IsDead() && thing_in->GetTC(&ptc_trespassable)) {
            if (ptc_trespassable->Trespassable) {
                ptc_trespassable->IsBeingTrespassedIn = true;
                PeekThingSearchTools().GetAllThings<CThingAICreature>
                    (CThingFilter_HasBuildingAsHomeOrWork(thing_in), OccupantsOfCurrentTrespassingThing);
                FrameSomeoneWasInformedOfLastDeed = 0;
                const COpinionReactionManager& manager = GetWorld().PeekOpinionReactionManager();
                AttachedOpinionSource = manager.AttachOpinionSource(*ParentThing, "OPINION_SOURCE_TRESPASSER", 0);
            } else {
                CTCHeroOpinionDeedLog* ptc_deed_log;
                if (ParentThing->GetTC(&ptc_deed_log))
                    InsideBuildingNotTrespassingDeedHandle = ptc_deed_log->PostDeed(
                        (EOpinionDeedType)0x4d, ParentThing->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, true, 0, false, 0);
            }
        }
    }
    TrespassingThing = CIntelligentPointer<CThing>(thing_in);
    CTCTrespassable* ptc_trespassable;
    if (IS_THING_ALIVE(TrespassingThing) && TrespassingThing->GetTC(&ptc_trespassable) && ptc_trespassable->IsBeingTrespassedIn) {
        long last_deed_handle_index = CurrentDeedHandleIndex - 1;
        bool there_is_no_last_deed = last_deed_handle_index < 0 || DeedHandles[last_deed_handle_index] == -1;
        if (CurrentDeedHandleIndex < 3) {
            if (!there_is_no_last_deed && OccupantsKnowOfDeedHandleIndex != last_deed_handle_index &&
                !SeeIfAnyOccupantsKnowAboutDeed(last_deed_handle_index, GetDeedType(last_deed_handle_index))) {
                if (TrespassingThing->HasTC((ETCInterfaceType)0x53))
                    CurrentDeedHandleIndex = 3;
            } else if ((unsigned long)CWorld::GetFrame() - FrameSomeoneWasInformedOfLastDeed > (unsigned long)(CWorld::GetFramesPerSecond() * 5))
                ++CurrentDeedHandleIndex;
        }
        CTCHeroOpinionDeedLog* ptc_deed_log;
        if (ParentThing->GetTC(&ptc_deed_log)) {
            for (long c1 = 0; c1 < CurrentDeedHandleIndex && c1 < 3; ++c1) {
                if (DeedHandles[c1] == -1)
                    DeedHandles[c1] = ptc_deed_log->PostDeed(GetDeedType(c1), ParentThing->GetPos(),
                         ptc_volume_containment_tracker->GetPThingInside(), 0, (EOpinionPostDeedType)2, true, true, 0, false, 0);
            }
        }
    }
    if (!IS_THING_ALIVE(TrespassingThing) && CurrentDeedHandleIndex > 0) {
        long frames_between_forgetting_posting_deeds = CWorld::GetFramesPerSecond() * 10;
        if ((unsigned long)CWorld::GetFrame() - FramePlayerLeft > (unsigned long)frames_between_forgetting_posting_deeds) {
            --CurrentDeedHandleIndex;
            FramePlayerLeft += frames_between_forgetting_posting_deeds;
        }
    }
}
