// CAIStateGroup_GoToBed::IsRunnable() -- find a bed when the creature can sleep
// Leaf: ?IsRunnable@CAIStateGroup_GoToBed@@UAE_NXZ
// Retail: 0x0092D6F0,198 bytes,thiscall
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCSleep.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_GoToBed.h"
bool CAIStateGroup_GoToBed::IsRunnable() {
    if(IS_THING_ALIVE(PBed)) CAIStateGroup_GoToBed::OnReplace();
    CTCSleep* tc_sleep;
    if(GetCreature().GetTC(&tc_sleep)) {
        if(GetWorld().GetFrame()-tc_sleep->GetLastFrameWokeUp()<120) return false;
    }
    if(IsUnreportedCrimeKnown()) return false;
    if(IsHeroTrespassingPerceived()) return false;
    if(IsInSleepTime()) {
        if(FindFreeBed(false)) {
            CAIStateGroup_GoToBed::OnReplace();
            return true;
        }
        if(FindFreeBanditBed()) {
            CAIStateGroup_GoToBed::OnReplace();
            return true;
        }
    }
    return false;
}