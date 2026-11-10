// CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative_pos) const -- place a world position on a region's mini-map graphic: normalise it inside the bounding box of the region's maps, keep the box aspect, scale to the mini-map graphic and add the region's mini-map offset
// Grade: DIFFER(634v634) -- 2 masked diff lines (10 grades): only the x87 operand order of the final `relative_pos.X * region.GetMiniMapScale()` remains -- retail `fld [esi] ; push ebx ; fmul [ebp+2Ch]` (0x005FBCC7), ours loads the scale first. The frame residue of chunk 1011 (scale_y in the loop-index slot) is fixed by the C2DVector scale(1,1) + `relative_pos *= scale` (frame-packer trace, hand886 phook.py: the 8-byte scale shares the bottom-right slot, scale.Y lands at [esp+24h] as in retail). c2prio: the scale read is CSE temp #45 (lines of the four final statements); operand swaps, outer swaps, parentheses, a named scale local, compound X ops and C2DVector forms of the offset/final lines all keep (or worsen) the order (probes in NOTES.md)
// Leaf: ?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FBAA0, 634 bytes (+ int3 padding), thiscall `ret 0Ch`. Ego_r.pdb 0x0064CBE0
//   ?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z (protected const).
// Callees: vector<long>::vector(const vector&) 0x00514160 (the copied map list), CTCBase::PeekWorld 0x00686A80, CWorld slot 13
//   PeekWorldMap (+0x34), CWorldMap slot 19 PeekMap(long) const (+0x4C), CTCInventoryBase::GetMiniMapGraphic 0x005BEB57,
//   CGraphicDataBank GetGraphicFrameWidth / GetGraphicFrameHeight (+0x54 / +0x58) on CMainGameComponent::PeekEngineGraphics (+0x161C8),
//   free 0x00BFEA14 (the copy's storage).
// Shape: the aspect correction is a C2DVector scale(1,1) applied with the component-wise C2DVector::operator*= (retail's frame puts
//   scale.Y at [esp+24h], sharing the bottom-right slot whose Y lives in ebp; separate float scale_x / scale_y share the loop index
//   slot instead).
// Layout (shared headers): CMap WorldSizeX +0x1D0C, WorldSizeY +0x1D10, WorldPos +0x1D24; CRegion MiniMapScale +0x2C, MiniMapOffset +0x30.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CMAP_WORLDACCESSORS
#define FABLE_RETAIL_C2DCOORDI_COPY
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CTCInventoryMap.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMap.h"
#include "engine/CRegion.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CGraphicDataBank.h"
#include "fable_game.h"

void CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative_pos) const
{
    std::vector<long> maps(region.GetMaps());
    C2DCoordI top_left = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldPosition();
    C2DCoordI bottom_right;
    bottom_right.X = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldSizeX() + top_left.X;
    bottom_right.Y = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldSizeY() + top_left.Y;
    for (unsigned long i = 1; i < maps.size(); ++i) {
        const CMap& map = PeekWorld().PeekWorldMap().PeekMap(maps[i]);
        if (top_left.X > map.GetWorldPosition().X)
            top_left.X = map.GetWorldPosition().X;
        long top = map.GetWorldPosition().Y;
        if (top_left.Y > top)
            top_left.Y = top;
        if (bottom_right.X < map.GetWorldPosition().X + map.GetWorldSizeX())
            bottom_right.X = map.GetWorldPosition().X + map.GetWorldSizeX();
        if (bottom_right.Y < map.GetWorldSizeY() + top)
            bottom_right.Y = map.GetWorldSizeY() + top;
    }
    relative_pos.X = position.X - top_left.X;
    relative_pos.Y = position.Y - top_left.Y;
    long width = bottom_right.X - top_left.X;
    long height = bottom_right.Y - top_left.Y;
    relative_pos.X /= (float)width;
    relative_pos.Y /= (float)height;
    C2DVector scale(1.0f, 1.0f);
    if (width > height)
        scale.Y = (float)height / (float)width;
    else if (width < height)
        scale.X = (float)width / (float)height;
    relative_pos *= scale;
    float graphic_width = (float)CMainGameComponent::Get()->PeekEngineGraphics().GetGraphicFrameWidth(GetMiniMapGraphic(region), 0);
    float graphic_height = (float)CMainGameComponent::Get()->PeekEngineGraphics().GetGraphicFrameHeight(GetMiniMapGraphic(region), 0);
    relative_pos.X = (relative_pos.X * region.GetMiniMapScale() - 0.5f) * graphic_width;
    relative_pos.Y = ((1.0f - relative_pos.Y) * region.GetMiniMapScale() - 0.5f) * graphic_height;
    relative_pos.X += region.GetMiniMapOffset().X * region.GetMiniMapScale();
    relative_pos.Y += region.GetMiniMapOffset().Y * region.GetMiniMapScale();
}
