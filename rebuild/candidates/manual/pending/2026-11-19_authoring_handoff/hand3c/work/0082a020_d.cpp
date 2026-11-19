// CMiniMapDisplay::GetDrawPosition(const C3DVector& position, const C2DBoxF& renderBox) const -- a world position's pixel on the minimap
// Leaf: ?GetDrawPosition@CMiniMapDisplay@@QBE?AVC2DVector@@ABVC3DVector@@ABVC2DBoxF@@@Z
// Flags: none
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS
#define FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION
#define FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_C2DBOXF_EXTENT_NAMED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CRegionMinimap.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CMiniMapDisplay.h"

extern long GDummyInt1;                                                    // 0x013B83B8 ?GDummyInt1@@3JA
extern long GDummyInt2;                                                    // 0x013B83BC ?GDummyInt2@@3JA

C2DVector CMiniMapDisplay::GetDrawPosition(const C3DVector& position, const C2DBoxF& renderBox) const
{
    C2DVector thingPos;
    GetRelativePosOnMap(position, thingPos);
    const CPlayerGuiDef& player_gui_def = CPlayerGui::PeekGuiDef();
    const CWorldMap& world_map = CMainGameComponent::Get()->GetWorld().PeekWorldMap();
    const CRegion& region = world_map.GetRegion(world_map.GetLoadedRegion());
    C2DVector mapSize;
    GetRelativeMapSize(mapSize);
    C2DBoxF local_render_box(renderBox);
    local_render_box.BRX = (local_render_box.BRX - local_render_box.TLX) * region.GetMiniMapScale() + local_render_box.TLX;
    local_render_box.BRY = (local_render_box.BRY - local_render_box.TLY) * region.GetMiniMapScale() + local_render_box.TLY;
    C2DVector renderPos((float)GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX),
        (float)GFFloatToLongFloor(local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y));
    renderPos.X += (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth());
    renderPos.Y += (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight());
    return renderPos;
}
