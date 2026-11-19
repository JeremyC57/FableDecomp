// CMiniMapDisplay::GetDrawPosition(const C3DVector& position, const C2DBoxF& renderBox) const -- a world position's pixel on the minimap
// Leaf: ?GetDrawPosition@CMiniMapDisplay@@QBE?AVC2DVector@@ABVC3DVector@@ABVC2DBoxF@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A020, 341 bytes, frame pointer, `ret 0Ch` (hidden return + 2 refs). Callees:
//   CRegionMinimap::GetRegionMapExtents 0x0066C6A0 (`lea ecx,[edi+54h] ; call` at 0x0082A035, the inline GetRelativePosOnMap),
//   CPlayerGui::PeekGuiDef 0x00438570, CWorld::PeekWorldMap (vtable slot 13, `call [eax+34h]`), CWorldMap::GetLoadedRegion 0x004FB150,
//   CWorldMap::GetRegion const 0x004FC180, CMiniMapDisplay::GetRelativeMapSize 0x00829C70. Globals GDummyInt1 0x013B83B8 and
//   GDummyInt2 0x013B83BC (FableWin publics ?GDummyInt1@@3JA / ?GDummyInt2@@3JA).
// Retail-asm: 0x0082A0D8 `fistp dword ptr [ebp+10h]` and 0x0082A118 `fistp dword ptr [ebp+0Ch]` (each with the `fsub` / `fild ; fadd ;
//   cmp` tail): the engine inline-assembly floor, expanded twice through the shared engine/GFFloatToLongFloor.h helper. FableWin calls
//   ?GFFloatToLongFloor@@YIJM@Z twice. No asm in this source.
// Shape: FableWin.pdb 0x027D3990; Ego_d locals position, renderBox, renderPos, player_gui_def, local_render_box, world_map, region,
//   mapSize, thingPos (player_gui_def is fetched and unused, as in FableWin). The divisor is the float 256.0f: retail loads the float
//   constant at 0x0125B240 (`fld dword ptr` at 0x0082A145 / 0x0082A15C); FableWin divides by the double __real@4070000000000000, and the
//   double spelling gives a qword fdivr here.
// Bracket: fixes 0x0082A12F `fild dword ptr [ebp+0Ch]` / 0x0082A132 `fild dword ptr [ebp+10h]` / 0x0082A135 `mov eax,[ebp+8]` /
//   0x0082A138 `pop edi` (the order after the second floor). Five natural brackets: the two `((BR - TL) * scale) + TL` sums, the X floor
//   argument `(mapSize.X * thingPos.X * extent) + TLX`, and the Y floor argument `(TLY + extent) - (mapSize.Y * thingPos.Y * extent)`.
//   Their PAREN tuples move c2's 83-node region cut to between the two `fild`s, which is retail's `fild ; fild ; mov ; pop ; fstp`.
//   Measured: 0 brackets, cut after `fild fild fstp fild fadd`; 2 (box sums only), cut 2 nodes later; 4 (without the Y product), cut
//   after both filds (`fstp mov fild pop`); 6 (with a redundant Y-argument wrap), `fild mov fild pop`; the `(mapSize.X * thingPos.X)`
//   grouping changes code (349v343); wrapping the C2DVector ctor arguments adds its PAREN after the filds. All DIFFER.
// Lever: FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY makes the GetRelativeMapSize body visible and __declspec(noinline) (still
//   called at 0x00829C70). VC7.1 then knows mapSize's address is not kept, which gives retail's frame homes for renderPos.Y (without it,
//   DIFFER in the frame slots). Body from FableWin 0x027D3850, Ego_d locals size / mapOrigin / mapExtents.
//   FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE keeps the existing inline GetRegionMapExtents body out of line, as retail calls it.
// Headers: CMiniMapDisplay.h + FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION (inside DRAWMARKERS: the +0x54 bytes typed as CRegionMinimap
//   RegionMinimap, the inline GetRelativePosOnMap, GetRelativeMapSize, GetDrawPosition); CRegionMinimap.h +
//   FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE. No layout change (sizeof(CRegionMinimap) is 0x64, as the bytes it replaces).
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
    local_render_box.BRX = ((local_render_box.BRX - local_render_box.TLX) * region.GetMiniMapScale()) + local_render_box.TLX;
    local_render_box.BRY = ((local_render_box.BRY - local_render_box.TLY) * region.GetMiniMapScale()) + local_render_box.TLY;
    C2DVector renderPos((float)GFFloatToLongFloor((mapSize.X * thingPos.X * local_render_box.GetExtents().X) + local_render_box.TLX),
        (float)GFFloatToLongFloor((local_render_box.TLY + local_render_box.GetExtents().Y) - (mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y)));
    renderPos.X += (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth());
    renderPos.Y += (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight());
    return renderPos;
}