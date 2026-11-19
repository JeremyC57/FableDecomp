// CTCCreatureNavigation::FrameUpdate() -- every fifth frame (staggered by the thing's unique ID) tell the navigator the creature's position;
//   once a second (staggered; at once on the first update) or when the creature speeds up, rescan the steering agents nearby: remember
//   the speed, schedule the next scan a second ahead, and collect the things in a box of half-size ceil(fps * speed * 3) (at least 2)
//   around the creature, other than itself
// Leaf: ?FrameUpdate@CTCCreatureNavigation@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CC800, 399 bytes, ebp frame, `ret`; CTCBase vtable slot 9. Callees: CWorld::GetFrame 0x0049D870,
//   _allrem 0x00BFEC30, CThingPhysical::PeekPos 0x006A4D00, the navigator's UpdateLastPosition (slot 13, +0x34),
//   CThingCreatureBase::GetCurrentSpeed 0x00661CD0, memmove 0x00BFEAE6 (Agents.clear()), CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70
//   (C3DVector::ToC2DCoordI), CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing> 0x006CE800 (landed).
// Retail-asm: 0x006CC903 `fistp dword ptr [ebp-4]` (after `fadd dword ptr [ebp-0Ch]`, then `fild ; fsub ; fstp ; cmp ; jne ; dec dword
//   ptr [ebp-4]`): the engine inline-assembly ceiling, expanded through the shared engine/GFFloatToLongCeil.h helper. FableWin calls
//   ?GFFloatToLongCeil@@YIJM@Z. No asm in this source.
// Shape: FableWin.pdb 0x02479E80 statement for statement (its ASSERT(PNavigator) compiled out); Ego_d locals search_tools, area,
//   box_width_and_height, next_update. The scale is the float 3.0f (retail 0x0122DED4; FableWin multiplies by the double 3.0).
//   FrameUpdateThing is the anonymous-namespace functor the landed 0x006CE800 instance declares (operator() 0x006CE730, landed).
//   The earlier parks (DIFFER 373v399) used local models; the shared headers close it on the first grade.
// Inferred: the (__int64) cast on the unique ID (unsigned __int64, Ego_r ?GetUniqueID@CThing@@QBE_KXZ): both staggers are signed in
//   retail and FableWin (`cdq ; add ; adc` then _allrem 0x00BFEC30 / FableWin __allrem); without a signed operand VC7.1 binds
//   __aullrem. The cast sits on the ID, as the landed stagger rows spell it.
// Headers: CTCCreatureNavigation.h + FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE (Agents typed vector<const CSteeringAgent*>, the
//   private inline GetParentCreature, the FrameUpdate override); CThingCreatureBase.h + FABLE_RETAIL_CTHINGCREATUREBASE_GETCURRENTSPEED.
//   No layout change.
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
    if (((__int64)GetParentThing().GetUniqueID() + CWorld::GetFrame()) % 5 == 0)
        PNavigator->UpdateLastPosition(GetParentCreature().PeekPos());
    if ((CWorld::GetFrame() >= NextUpdate && (NextUpdate == 0 || ((__int64)GetParentThing().GetUniqueID() + CWorld::GetFrame()) % CMainGameComponent::GetConstantFPS() == 0))
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
