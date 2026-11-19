// CEngineMap::PeekWaterHeight(long x, long y) const -- the water surface height at map cell (x, y): the landscape height under it plus the water depth where the depth exceeds 0.001, else 0
// Leaf: ?PeekWaterHeight@CEngineMap@@QBEMJJ@Z
// Flags: /d2inlT576
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B41A80, 424 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb 0x009E19B0 / FableWin.pdb 0x02D5DD80 ?PeekWaterHeight@CEngineMap@@QBEMJJ@Z.
// Shape: FableWin 0x02D5DD80: depth = PeekWaterDepth(x, y); depth > 0.001 ? PeekLandscapeHeight(x, y) + depth : 0. PeekLandscapeHeight is
//   the header inline body (FABLE_RETAIL_CENGINEMAP_PEEK_BODIES), expanded here as in the landed rows of this TU; its neighbour branch
//   calls the COMDAT copy 0x00B439C0. Retail's literals are floats (FableWin: the 0 is a double).
// Callees: CEngineMap::PeekWaterDepth 0x00B40BD0, CEngineMap::PeekLandscapeHeight 0x00B439C0, CIDrawMap::DrawBlockGetSizeZAt (slot 5).
// Retail-asm: 0x00B41BEA `fistp dword ptr [esp+1Ch]` -- engine/GFFloatToLongNear.h inside the inline CEngineMap::QuantiseLandscapeHeight.
//   This row carries no asm of its own.
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CENGINEMAP_PEEKWATERHEIGHT
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

float CEngineMap::PeekWaterHeight(long x, long y) const
{
    float depth = PeekWaterDepth(x, y);
    return depth > 0.001f ? PeekLandscapeHeight(x, y) + depth : 0.0f;
}
