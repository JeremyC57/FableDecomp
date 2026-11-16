// 0057df85 CTCHeroStats::KickHeroToMostRecentSafeRegion
// Leaf: ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ
// Flags: /GS
#pragma optimize("s",on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_KICK_PASS23
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_KICKREGION_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MOVEHEROTOREGION
#define FABLE_RETAIL_CWORLD_KICKREGION_PASS23
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/C3DVector.h"
#include "engine/CTCHeroStats.h"
#include "engine/CGameEvent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CPlayerGui.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasDefName.h"
#include "engine/CThingFilter_HasScriptData.h"
#include "engine/ThingAlive.h"
void CTCHeroStats::KickHeroToMostRecentSafeRegion()
{
    MirroredCriminalStatus best_data;
    long best = 0;
    CCharString current_region(PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName());
    CThingMarker* kick_marker;
    if (PeekThingSearchTools().GetPFirstLeafThing(
            GFFilter_Bind2(CThingFilter_HasDefName("REGION_KICK_TO_POINT"), CThingFilter_HasScriptData(current_region)),
            &kick_marker)) {
        if(IS_THING_ALIVE(&GetParentThing())) {
        GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
        CGameEvent game_event(0x2c);
        game_event.AddFloat(kick_marker->GetPos().X);
        game_event.AddFloat(kick_marker->GetPos().Y);
        game_event.AddFloat(kick_marker->GetPos().Z);
        game_event.AddFloat(kick_marker->GetPhysicsTC()->GetFacingAngleXY());
        game_event.AddUBYTE(false);
        game_event.AddUBYTE(false);
        game_event.AddUBYTE(false);
        game_event.SetPlayer(GetParentThing().GetPlayerNumber());
        long region_number = PeekWorldMap().GetRegionNumberMapIsIn(kick_marker->GetCurrentMapNumber());
        if (region_number > 0) {
            CPlayerGui* gui = &CPlayerGui::Get();
            gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region_number));
        }
        GetWorld().HandleMoveHeroToRegionGameEvent(game_event);
        }
        return;
    }
    for (std::map<CCharString, MirroredCriminalStatus>::iterator i = OutstandingCrimes.begin(); i != OutstandingCrimes.end(); ++i) {
        long fine;
        long left;
        GetOutstandingFineAndFramesLeft(i->first, fine, left);
        long exited;
        if (fine == 0 && i->first != current_region && (exited = i->second.LastTimeExited) > best) {
            best_data = i->second;
            best = exited;
        }
    }
    if(best!=0 && IS_THING_ALIVE(&GetParentThing())) {
    GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
    CGameEvent game_event(0x2c);
    game_event.AddFloat(best_data.EntrancePosX);
    game_event.AddFloat(best_data.EntrancePosY);
    game_event.AddFloat(best_data.EntrancePosZ);
    game_event.AddFloat(best_data.EntranceAngle);
    game_event.AddUBYTE(false);
    game_event.AddUBYTE(false);
    game_event.AddUBYTE(false);
    game_event.SetPlayer(GetParentThing().GetPlayerNumber());
    C3DVector entrance_coord(best_data.EntrancePosX, best_data.EntrancePosY, best_data.EntrancePosZ);
    long region_number = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(entrance_coord));
    if (region_number > 0) {
        CPlayerGui* gui = &CPlayerGui::Get();
        gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region_number));
    }
    GetWorld().HandleMoveHeroToRegionGameEvent(game_event);
    }
}