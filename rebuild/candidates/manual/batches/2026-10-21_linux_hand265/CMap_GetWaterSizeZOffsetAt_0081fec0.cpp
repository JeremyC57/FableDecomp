// CMap::GetWaterSizeZOffsetAt(const C3DVector& pos) -- the average water surface height over the four map cells around a world position: for each of (x,y) (x+1,y) (x,y+1) (x+1,y+1) whose water size offset exceeds 0.05, add that offset plus the cell's ground height; the sum over the count, or 0 when no cell has water
// Leaf: ?GetWaterSizeZOffsetAt@CMap@@QAEMABVC3DVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FEC0, length 448 bytes (`ret 4`).
// Callees: _ftol @ 0x00BFEA70 (the two float -> long casts), CMap::GetWaterSizeZOffsetAt(const C2DCoordI&) @ 0x0081FCC0.
//   Floats: 0x0125A15C 0.05f, 0x0122DEDC 0.0f.
// Layout: as the sibling CMap rows 0x0081FB00/0x0081FCC0 (height map +0x1a7c: cells +0x10, SizeX +0x204, SizeY +0x208; cell height +0).
// Shape: four straight-line probes through C2DCoordI temporaries; x+1 and y+1 are NAMED locals (`long x1`, `long y1`: x1 spills to
//   the dead first coordinate, y1 takes the pos/y register -- `y++` swaps esi/edi); the running sum is homed in the dead `pos`
//   argument slot and the count is kept in ebp with a frame copy for the final `fild`.
struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
};
struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMap {                                // retail layout
    unsigned char                  _pad00[0x10];
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _pad1c[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
struct CMap {
    unsigned char                            _pad00[0x1c];
    void*                                    DefinitionManager;   // +0x1c
    unsigned char                            _pad20[0x1a7c - 0x20];
    CHeightMap*                              HeightMap;           // +0x1a7c
    bool HasWaterAt(const C2DCoordI& pos);       // 0x0081FB00
    float GetWaterSizeZOffsetAt(const C2DCoordI& pos);   // 0x0081FCC0
    float GetWaterSizeZOffsetAt(const C3DVector& pos);
};

float CMap::GetWaterSizeZOffsetAt(const C3DVector& pos)
{
    long x = (long)pos.X;
    long y = (long)pos.Y;
    float total = 0.0f;
    long count = 0;
    float h = GetWaterSizeZOffsetAt(C2DCoordI(x, y));
    if (h > 0.05f) {
        total += h + HeightMap->GetCellIncludingExtraCells(x, y).Height;
        count++;
    }
    long x1 = x + 1;
    h = GetWaterSizeZOffsetAt(C2DCoordI(x1, y));
    if (h > 0.05f) {
        total += h + HeightMap->GetCellIncludingExtraCells(x1, y).Height;
        count++;
    }
    long y1 = y + 1;
    h = GetWaterSizeZOffsetAt(C2DCoordI(x, y1));
    if (h > 0.05f) {
        total += h + HeightMap->GetCellIncludingExtraCells(x, y1).Height;
        count++;
    }
    h = GetWaterSizeZOffsetAt(C2DCoordI(x1, y1));
    if (h > 0.05f) {
        total += h + HeightMap->GetCellIncludingExtraCells(x1, y1).Height;
        count++;
    }
    if (count > 0)
        return total / count;
    return 0.0f;
}
