// Grade: DIFFER(382v391) -- 69 difflines; retail_asm candidate: the landed __forceinline FloatToLong (__asm fld/mov edx/fistp dword, as 0x00B1B5E0 / 0x0081E550) gives retail's conversion sequence (345->389v391 plain, 382 with block temps). Left: retail caches HeightMap in ebx for the two clamp bounds (push ebx, keeps this in ecx and re-reads +0x1a7c for the cell) and homes both conversions in the dead localPos argument slot. Tried: plain x/y outs (389v391, 78 lines), both in block temps (382, 69), x in a block temp (382, 73). Next: a named map local for the clamp bounds only.
// CMap::GetGroundSizeZAt(const C3DVector& localPos) const -- the ground height under a map-local position: round the position to its height-map cell (clamped inside the extra-cell border) and interpolate the cell's four corner heights over the triangle the fraction falls in (the diagonal alternates with the cell's parity)
// Leaf: ?GetGroundSizeZAt@CMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081EA50, 391 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb ?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z; callers CWorldMap::GetGroundSizeZAt 0x0063A300 and
//   two CMap neighbours at 0x008200B0 / 0x00820140 (the landed 0x0063A5F0 model lists this address as
//   CMap::GetGroundSizeZAt(const C3DVector&)). The catalog's "Terrain_InterpolateHeightAtPosition" is descriptive.
// Retail-asm: 0x0081EA6E `fld dword ptr [esp+0Ch] ; mov edx,[esp+10h] ; fistp dword ptr [edx]` (and again at
//   0x0081EA96) -- the engine's pointer-out inline-assembly float-to-long helper (the landed 0x00B1B5E0 model); this C++
//   reference body converts with `(long)` (__ftol2 calls), so the row is a retail_asm candidate, not reconstruction.
// Data: 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Layout: CMap::HeightMap scoped_ptr<CHeightMap> +0x1a7c (PDB +0x1a8c); CHeightMap: Cells vector<CHeightMapCell> +0x10
//   (12-byte cells, Height +0), SizeX +0x204, SizeY +0x208, SizeXIncludingExtraCells +0x20c,
//   SizeYIncludingExtraCells +0x210 (PDB +4 each: the 16-byte Dinkumware vector).
struct C3DVector { float X, Y, Z; };

struct CHeightMapCell {                                 // 12 bytes
    float         Height;                               // +0x00
    unsigned char _rest[8];
};

struct CHeightMap {
    unsigned char   _pad00[0x10];
    CHeightMapCell* Cells;                              // +0x10 vector<CHeightMapCell>::_M_start
    CHeightMapCell* CellsFinish;                        // +0x14
    CHeightMapCell* CellsEnd;                           // +0x18
    unsigned char   _pad1c[0x204 - 0x1c];
    long            SizeX;                              // +0x204
    long            SizeY;                              // +0x208
    long            SizeXIncludingExtraCells;           // +0x20c
    long            SizeYIncludingExtraCells;           // +0x210
};

// the engine's pointer-out float-to-long helper: expanded in place, the pointer parameter gets a frame copy
__forceinline void FloatToLong(long* result, float value)
{
    __asm fld value
    __asm mov edx, result
    __asm fistp dword ptr [edx]
}

class CMap {
public:
    unsigned char _pad0000[0x1a7c];
    CHeightMap*   HeightMap;                            // +0x1a7c
    float GetGroundSizeZAt(const C3DVector& localPos) const;
};

float CMap::GetGroundSizeZAt(const C3DVector& localPos) const
{
    long x;
    {
        long cellX;
        FloatToLong(&cellX, localPos.X + 0.5f);
        x = cellX - 1;
    }
    long y;
    {
        long cellY;
        FloatToLong(&cellY, localPos.Y + 0.5f);
        y = cellY - 1;
    }
    long maxX = HeightMap->SizeXIncludingExtraCells - 2;
    if (x < 0)
        x = 0;
    else if (x > maxX)
        x = maxX;
    long maxY = HeightMap->SizeYIncludingExtraCells - 2;
    if (y < 0)
        y = 0;
    else if (y > maxY)
        y = maxY;
    float fx = localPos.X - x;
    float fy = localPos.Y - y;
    const CHeightMap* map = HeightMap;
    const CHeightMapCell* cell = &map->Cells[(map->SizeY - y) * (map->SizeX + 1) + x];
    float h00 = cell[0].Height;
    float h10 = cell[1].Height;
    const CHeightMapCell* above = cell - map->SizeXIncludingExtraCells;
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
