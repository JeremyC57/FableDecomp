// 0092cda0 CAIStateGroup_MinionSleep::OnReplace
// Leaf: ?OnReplace@CAIStateGroup_MinionSleep@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCSLEEP_WAKE_PASS27
#define FABLE_RETAIL_CTCBED
#define FABLE_RETAIL_CTCBED_CLEAR_PASS27
#define FABLE_RETAIL_CBEDDEF
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_GoToBed.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCSinglePersonOccupiable.h"
#include "engine/CTCSleep.h"
#include "engine/CWorld.h"
#include "engine/CBedDef.h"
#include "engine/CTCBed.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAIStateGroup_MinionSleep.h"
namespace NCreatureMode {
bool FABLE_FASTCALL GetUpFromSleeping(CThingCreatureBase&, CThing&, const CCharString&, CAIBrain*);
}

void CAIStateGroup_MinionSleep::OnReplace()
{
    CTCSleep* tc_sleep;
    if (GetCreature().GetTC(&tc_sleep)) {
        tc_sleep->SetIsNotReadyToSleep();
        tc_sleep->SetFastTrack(false);
        tc_sleep->SetLastFrameWokeUp(PeekWorld().GetFrame());
    }
    CTCCreatureModeManager* ptc_mode_manager;
    GetCreature().GetTC(&ptc_mode_manager);
    bool needToRemoveMode = ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SLEEP_BED;
    if (needToRemoveMode) {
        if (IS_THING_ALIVE(PBed)) {
            const CTCBed* ptcbed = 0;
            PBed->PeekTC(&ptcbed);
            CDefPointer<const CBedDef> pbed_def = ptcbed->GetDefinition();
            CCharString getUpAnim = pbed_def->GetUpFromBedFastAnimName;
            if (getUpAnim == "")
                getUpAnim = pbed_def->GetUpFromBedAnimName;
            if (NCreatureMode::GetUpFromSleeping(GetCreature(), *PBed.Get(), getUpAnim, PBrain)) {
                CTCBed* ptc_bed;
                if (IS_THING_ALIVE(PBed) && PBed->GetTC(&ptc_bed) && BedGetOutOfBedFastAnim)
                    ptc_bed->StartGetOutOfBedAnimation(BedGetOutOfBedFastAnim);
                needToRemoveMode = false;
            }
        }
        if (needToRemoveMode) {
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_SLEEP_BED);
            CTCBed* ptc_bed;
            if (IS_THING_ALIVE(PBed) && PBed->GetTC(&ptc_bed))
                ptc_bed->Clear();
        }
    }
    CAIStateGroup_GoToBed::OnReplace();
    PlayEmergencyGetOutOfBed = false;
}