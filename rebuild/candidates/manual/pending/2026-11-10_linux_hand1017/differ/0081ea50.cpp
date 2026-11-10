// CMap::GetGroundSizeZAt(const C3DVector& localPos) const -- the ground height under a map-local position: round the position to its height-map cell (clamped inside the extra-cell border) and interpolate the cell's four corner heights over the triangle the fraction falls in (the diagonal alternates with the cell's parity)
// Grade: DIFFER(382v391) -- frame / CSE residue (40 difflines): retail keeps `this` in ecx, caches HeightMap in ebx for the two clamp bounds and reloads +1A7Ch for the cell, and homes result1, result2 and y in the dead localPos argument slot (frame 0xC); ours CSEs HeightMap into ecx once and gives result2 / y their own frame words (frame 0x10). The float tree, clamps and GFFastFloatToLong expansions match. Tried: x/y adjusted in place (--x / --y), assignment form. 3 grades.
// Leaf: ?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081EA50, 391 bytes (+ int3 padding to 0x0081EBE0, where the C2DCoordI overload starts),
//   thiscall `ret 4`.
// Identity: Ego_r.pdb ?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z (protected const; the C2DCoordI overload
//   ?GetGroundSizeZAt@CMap@@IBEMABVC2DCoordI@@@Z follows it at 0x0081EBE0); callers CWorldMap::GetGroundSizeZAt 0x0063A300 and the
//   CMap neighbours 0x008200B0 / 0x00820140. The catalog's "Terrain_InterpolateHeightAtPosition" was descriptive.
// Retail-asm: 0x0081EA6E `fld dword ptr [esp+0Ch] ; mov edx,[esp+10h] ; fistp dword ptr [edx]` and 0x0081EA96 `fld dword ptr
//   [esp+14h] ; mov edx,[esp+0Ch] ; fistp dword ptr [edx]` -- the engine's pointer-out float-to-long (shared engine/GFFastFloatToLong.h,
//   FableWin.pdb ?GFFastFloatToLong@@YIJM@Z); VC7.1 compiles `(long)x` to __ftol2, so no C++ spelling gives the bare dword fistp.
// Data: 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Layout: CMap::HeightMap scoped_ptr<CHeightMap> +0x1A7C (engine/CMap.h); CHeightMap (new engine/CHeightMap.h): Cells +0x10,
//   SizeX +0x204, SizeY +0x208, SizeXIncludingExtraCells +0x20C, SizeYIncludingExtraCells +0x210; CHeightMapCell 12 bytes, Height +0.
// Graded with FABLE_HDR_OVERLAY=$A/hdr (engine/CHeightMap.h new, engine/CMap.h FABLE_RETAIL_CMAP_GETGROUNDSIZEZAT).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_GETGROUNDSIZEZAT
#define FABLE_RETAIL_CHEIGHTMAP
#include "engine/C3DVector.h"
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
#include "engine/GFFastFloatToLong.h"

float CMap::GetGroundSizeZAt(const C3DVector& localPos) const
{
    long x = GFFastFloatToLong(localPos.X + 0.5f) - 1;
    long y = GFFastFloatToLong(localPos.Y + 0.5f) - 1;
    long maxX = HeightMap->GetSizeXIncludingExtraCells() - 2;
    if (x < 0)
        x = 0;
    else if (x > maxX)
        x = maxX;
    long maxY = HeightMap->GetSizeYIncludingExtraCells() - 2;
    if (y < 0)
        y = 0;
    else if (y > maxY)
        y = maxY;
    float fx = localPos.X - x;
    float fy = localPos.Y - y;
    const CHeightMap* map = HeightMap.get();
    const CHeightMapCell* cell = &map->Cells[(map->GetSizeY() - y) * (map->GetSizeX() + 1) + x];
    float h00 = cell[0].Height;
    float h10 = cell[1].Height;
    const CHeightMapCell* above = cell - map->GetSizeXIncludingExtraCells();
    float h01 = above[0].Height;
    float h11 = above[1].Height;
    if ((x + y) & 1) {
        if (fx + fy > 1.0f)
            return (h10 - h11) * (1.0f - fy) + (h01 - h11) * (1.0f - fx) + h11;
        return (h01 - h00) * fy + (h10 - h00) * fx + h00;
    }
    if (fx > fy)
        return (h11 - h10) * fy + (h10 - h00) * fx + h00;
    return (h11 - h01) * fx + (h01 - h00) * fy + h00;
}
