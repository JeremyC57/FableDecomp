// CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative) const -- place a world position on a region's mini-map graphic
// Retail 0x005FBAA0, 634 bytes, thiscall, ret 0Ch, esp frame (speed TU, no pragmas).
// Callees: std::vector<long> copy ctor 0x00514160 (FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE), CTCBase::PeekWorld 0x00686A80,
// CWorld::PeekWorldMap vslot 13 (+0x34), CWorldMap::PeekMap vslot 19 (+0x4C), NInventory::CTCInventoryBase::GetMiniMapGraphic
// 0x005BEB57, CGraphicDataBank::GetGraphicFrameWidth/Height vslots 21/22 (+0x54/+0x58), free 0x00BFEA14 (vector dtor).
// Shape: copy region's map ids; bounds = first map's world rect, grown over the rest (unsigned index loop, size cached); relative =
// (position - topLeft) / extent, aspect-corrected by a two-float scale (scale[0] stays on the x87 stack, scale[1] lives in br's dead
// frame word [esp+24h]: the array is declared after the extents, so it packs into br's slot); then scaled by the region's mini-map
// scale, centred (-0.5) and sized by the mini-map graphic's frame size, plus the region's mini-map offset * scale.
// C2DCoordI user copy ctor (FABLE_RETAIL_C2DCOORDI_COPY) gives the dead `add eax,1D24h`. The scale array's 2-trip initialise loop
// (fully unrolled) is what puts VC7.1's X-line operand order at retail's `fld relative.X ; fmul scale`.
// Leaf: ?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DCOORDI_COPY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_WORLDACCESSORS
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#include "stlport/fable_containers.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CRegion.h"
#include "engine/C3DVector.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CGraphicDataBank.h"
#include "fable_game.h"
void CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative) const
{
    std::vector<long> maps = region.GetMaps();
    C2DCoordI tl = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldPosition();
    C2DCoordI br;
    br.X = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldSizeX() + tl.X;
    br.Y = PeekWorld().PeekWorldMap().PeekMap(maps[0]).GetWorldSizeY() + tl.Y;
    for (unsigned int i = 1; i < maps.size(); ++i) {
        const CMap& map = PeekWorld().PeekWorldMap().PeekMap(maps[i]);
        if (tl.X > map.GetWorldPosition().X)
            tl.X = map.GetWorldPosition().X;
        long top = map.GetWorldPosition().Y;
        if (tl.Y > top)
            tl.Y = top;
        if (br.X < map.GetWorldPosition().X + map.GetWorldSizeX())
            br.X = map.GetWorldPosition().X + map.GetWorldSizeX();
        if (br.Y < map.GetWorldSizeY() + top)
            br.Y = map.GetWorldSizeY() + top;
    }
    relative.X = position.X - tl.X;
    relative.Y = position.Y - tl.Y;
    long width = br.X - tl.X;
    long height = br.Y - tl.Y;
    relative.X /= (float)width;
    relative.Y /= (float)height;
    float scale[2];
    for (int k = 0; k < 2; ++k)
        scale[k] = 1.0f;
    if (width > height)
        scale[1] = (float)height / (float)width;
    else if (width < height)
        scale[0] = (float)width / (float)height;
    relative.X *= scale[0];
    relative.Y *= scale[1];
    float graphicWidth = (float)CMainGameComponent::Get()->PeekEngineGraphics().GetGraphicFrameWidth(GetMiniMapGraphic(region), 0);
    float graphicHeight = (float)CMainGameComponent::Get()->PeekEngineGraphics().GetGraphicFrameHeight(GetMiniMapGraphic(region), 0);
    relative.X = (relative.X * region.GetMiniMapScale() - 0.5f) * graphicWidth;
    relative.Y = ((1.0f - relative.Y) * region.GetMiniMapScale() - 0.5f) * graphicHeight;
    relative.X += region.GetMiniMapOffset().X * region.GetMiniMapScale();
    relative.Y += region.GetMiniMapOffset().Y * region.GetMiniMapScale();
}