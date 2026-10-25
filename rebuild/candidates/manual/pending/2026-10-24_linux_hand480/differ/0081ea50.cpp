// Grade: DIFFER(345v391) -- retail_asm candidate: retail rounds both coordinates with the inline-assembly pointer-out helper (`fld ; mov edx,ptr ; fistp dword ptr [edx]`, 12-byte frame); this C++ reference converts with (long) (two __ftol2 calls, 8-byte frame). The clamps, the cell addressing and the four-way triangle interpolation tail match instruction for instruction apart from the frame offsets.
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

// the engine's pointer-out float-to-long helper (retail: inline assembly `fistp dword ptr [edx]`)
inline void FloatToLong(long* result, float value)
{
    *result = (long)value;
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
    FloatToLong(&x, localPos.X + 0.5f);
    --x;
    long y;
    FloatToLong(&y, localPos.Y + 0.5f);
    --y;
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
