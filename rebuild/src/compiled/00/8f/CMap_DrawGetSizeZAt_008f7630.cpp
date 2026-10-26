// CMap::DrawGetSizeZAt(const C3DVector* pos, float radius) const -- the ground height at a world position: clamp the four grid corners around it to the map, and interpolate across the cell's triangle (the cell is split along the diagonal that alternates with the parity of x + y)
// Leaf: DrawGetSizeZAt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F7630, length 459 bytes (`ret 8`, Ego_r.pdb `?DrawGetSizeZAt@CMap@@UBEMPBVC3DVector@@M@Z`).
// Callees: _ftol @ 0x00BFEA70. Float 0x0122DED8 = 1.0f.
// Layout: as this chunk's CMap rows 0x0081FB00/0x0081FEC0 (height map +0x1a7c: cells +0x10, SizeX +0x204, SizeY +0x208; cell
//   height +0) and the landed CMap::DrawBlockGetSizeZAt 0x008F7800; the map's size in cells +0x1d14 / +0x1d18 as the landed
//   CHeightMap ctor 0x008F63C0 passes them.
// Shape: the clamps are a macro (the upper bound is re-read inside each else-arm, so Width/Height load once per clamp, never
//   CSE'd); one `float z` assigned in an if/else tree with a single return (four early returns cross-jump the shared
//   `(..)*dy + (h10-h00)*dx + h00` tail the other way round).
class C3DVector { public:
    float X, Y, Z;
};
class CHeightMapCell { public:                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CHeightMap { public:
    unsigned char                  _pad00[0x10];
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _pad1c[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
#define CLAMP_TO_MAP(v, high) ((v) < 0 ? 0 : ((v) > (high) ? (high) : (v)))
class CMap {
public:
    virtual float DrawGetSizeZAt(const C3DVector* pos, float radius) const;
    unsigned char _pad04[0x1a7c - 0x04];
    CHeightMap*   HeightMap;                       // +0x1a7c
    unsigned char _pad1a80[0x1d14 - 0x1a80];
    long          Width;                           // +0x1d14
    long          Height;                          // +0x1d18
};

float CMap::DrawGetSizeZAt(const C3DVector* pos, float radius) const
{
    float fx = pos->X;
    float fy = pos->Y;
    long x = (long)fx;
    float dx = fx - x;
    long y = (long)fy;
    float dy = fy - y;
    long x0 = CLAMP_TO_MAP(x, Width - 1);
    long y1 = CLAMP_TO_MAP(y + 1, Height - 1);
    long x1 = CLAMP_TO_MAP(x + 1, Width - 1);
    long y0 = CLAMP_TO_MAP(y, Height - 1);
    float h01 = HeightMap->GetCellIncludingExtraCells(x0, y1).Height;
    float h11 = HeightMap->GetCellIncludingExtraCells(x1, y1).Height;
    float h10 = HeightMap->GetCellIncludingExtraCells(x1, y0).Height;
    float h00 = HeightMap->GetCellIncludingExtraCells(x0, y0).Height;
    float z;
    if ((x + y) & 1) {
        if (dx + dy > 1.0f)
            z = (h10 - h11) * (1.0f - dy) + (h01 - h11) * (1.0f - dx) + h11;
        else
            z = (h01 - h00) * dy + (h10 - h00) * dx + h00;
    } else if (dx > dy)
        z = (h11 - h10) * dy + (h10 - h00) * dx + h00;
    else
        z = (h01 - h00) * dy + (h11 - h01) * dx + h00;
    return z;
}