// CAIStateGroup_RangedCombatUsePreferredFirePoint::IsStillRunnable() -- still runnable while the fire point and the enemy are alive and the group is not stopping; once a shot has been fired, advance the last-shot frame over every newer fired-shot event and give up after five seconds without one
// Leaf: ?IsStillRunnable@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095D0E0, 285 bytes, __fastcall. Ego_r.pdb
//   ?IsStillRunnable@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAE_NXZ (protected).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE on PFirePoint, then PEnemy), CAIStateGroupBase::IsStopping
//   0x008FD010, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCEntityEvents>, type 0x42), CTCEntityEvents::GetEvent<CEventFiredShot,
//   CIsEventCreatedAfter> 0x0095D920, CAIStateGroupBase::GetWorld 0x008FCFC0 then the static CWorld::GetFrame 0x0049D870,
//   CMainGameComponent::ConstantFPS 0x01375550 (`lea ecx,[ecx+ecx*4]`: five seconds).
// Shape: the filter is built from the member each time (0x0095D143 `mov ebp,[edi+24h]` serves the > 0 test and the first filter; in
//   the loop VC7.1 forwards the stored frame, `mov edx,[ecx+4] ; mov eax,edx`, 0x0095D1A4); the elapsed frames are a named local.
// Header overlay (hdr/): FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_STILLRUNNABLE (the declaration).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT
#define FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_STILLRUNNABLE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"

namespace NEntityEvents {
class CEventFiredShot : public CEventBase {                // no shared header (Ego_r.pdb ??0CEventFiredShot@NEntityEvents@@QAE@AAVCThing@@@Z)
};
}

bool CAIStateGroup_RangedCombatUsePreferredFirePoint::IsStillRunnable()
{
    if (IS_THING_ALIVE(PFirePoint) && IS_THING_ALIVE(PEnemy) && !IsStopping()) {
        if (TimeOutCounter > 0) {
            CTCEntityEvents* events;
            if (GetCreature().GetTC(&events)) {
                NEntityEvents::CEventFiredShot* shot;
                while (events->GetEvent(&shot, NEntityEvents::CIsEventCreatedAfter(TimeOutCounter)))
                    TimeOutCounter = shot->WorldFrame;
            }
            long frames_since_shot = GetWorld().GetFrame() - TimeOutCounter;
            if (frames_since_shot > CMainGameComponent::GetConstantFPS() * 5)
                return false;
        }
        return true;
    }
    return false;
}