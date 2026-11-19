// CTCCreatureNavigation::FrameUpdate()
// Leaf: ?FrameUpdate@CTCCreatureNavigation@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCURRENTSPEED
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFFloatToLongCeil.h"

namespace {
struct FrameUpdateThing {
    std::vector<const CSteeringAgent*>& Agents;
    FrameUpdateThing(std::vector<const CSteeringAgent*>& agents) : Agents(agents) {}
    bool operator()(const CThing& thing) const;            // 0x006CE730
};
}

void CTCCreatureNavigation::FrameUpdate()
{
    if ((GetParentThing().GetUniqueID() + CWorld::GetFrame()) % 5 == 0)
        PNavigator->UpdateLastPosition(GetParentCreature().PeekPos());
    if ((CWorld::GetFrame() >= NextUpdate && (NextUpdate == 0 || (GetParentThing().GetUniqueID() + CWorld::GetFrame()) % CMainGameComponent::GetConstantFPS() == 0))
        || GetParentCreature().GetCurrentSpeed() > CurrentSpeed)
    {
        long next_update = CMainGameComponent::GetConstantFPS();
        CurrentSpeed = GetParentCreature().GetCurrentSpeed();
        NextUpdate = CWorld::GetFrame() + next_update;
        Agents.clear();
        long box_width_and_height = GFFloatToLongCeil(next_update * CurrentSpeed * 3.0f);
        if (box_width_and_height < 2)
            box_width_and_height = 2;
        C2DBoxI area(GetParentCreature().GetPos().ToC2DCoordI(), box_width_and_height);
        const CThingSearchTools& search_tools = CMainGameComponent::Get()->GetWorld().PeekThingSearchTools();
        search_tools.ForEachInRoughArea<CThing>(area, CThingFilter_IsNot(GetParentCreature()), FrameUpdateThing(Agents));
    }
}
