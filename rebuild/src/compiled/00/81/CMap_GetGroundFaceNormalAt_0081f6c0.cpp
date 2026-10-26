// CMap::GetGroundFaceNormalAt(const C3DVector& localPos) const -- unit normal of the height-map triangle under a local
//   position: sample the four corner heights of the cell, pick the triangle by the cell's diagonal parity and the
//   fractional position, cross the two edge vectors (1,0,a) x (0,1,b) and normalise with the fast inverse square root
// Leaf: ?GetGroundFaceNormalAt@CMap
// Retail: Fable.exe @ 0x0081F6C0, 550 bytes, `ret 8` (hidden result + localPos). Caller CWorldMap::GetGroundFaceNormalAt
//   0x0063AB10 (landed) forwards the hidden result.
// Callees: _ftol 0x00BFEA70. 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 3.0f 0x0122DED4, 0.5f 0x0122F59C, inverse-sqrt mantissa
//   table 0x013CE558 (FastInvSqrt as landed in 0x008C2550).
// Layout: CMap PHeightMap +0x1a7c (retail); CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208 and the 12-byte
//   CHeightMapCell (Height +0) as in the landed CMap::GetThemeHeightAt 0x0081FA20.
// Shape: PHeightMap is re-read through `this` at every cell (a cached local lets VC7.1 CSE SizeX/SizeY); the even-parity
//   test is written dy > dx so both arms start with the same (long)localPos.Y, which VC7.1 hoists above the parity
//   compare; the cross product is a named local normalised in place (member Normalise) and returned by copy (user copy
//   ctor), as retail keeps it in the frame.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    __forceinline void Normalise() { float r = FastInvSqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

class CHeightMapCell { public:                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char _pad04[8];
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CHeightMap { public:
    virtual ~CHeightMap();
    void*                          World;          // +0x04
    void*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) const { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
class CMap { public:
    unsigned char _pad000[0x1a7c];
    CHeightMap*   PHeightMap;                      // +0x1a7c
    C3DVector GetGroundFaceNormalAt(const C3DVector& localPos) const;
};

C3DVector CMap::GetGroundFaceNormalAt(const C3DVector& localPos) const
{
    long x = (long)localPos.X;
    long y = (long)localPos.Y;
    float h00 = PHeightMap->GetCellIncludingExtraCells(x, y).Height;
    long x1 = (long)(localPos.X + 1.0f);
    float h10 = PHeightMap->GetCellIncludingExtraCells(x1, y).Height;
    long y1 = (long)(localPos.Y + 1.0f);
    float h01 = PHeightMap->GetCellIncludingExtraCells(x, y1).Height;
    float h11 = PHeightMap->GetCellIncludingExtraCells(x1, y1).Height;
    float a, b;
    if ((x + y) % 2 == 1) {
        if ((localPos.Y - (long)localPos.Y) + (localPos.X - x) > 1.0f) {
            a = h11 - h01;
            b = h11 - h10;
        } else {
            a = h10 - h00;
            b = h01 - h00;
        }
    } else {
        if (localPos.Y - (long)localPos.Y > localPos.X - x) {
            a = h11 - h01;
            b = h01 - h00;
        } else {
            a = h10 - h00;
            b = h11 - h10;
        }
    }
    C3DVector n = CrossProduct(C3DVector(1.0f, 0.0f, a), C3DVector(0.0f, 1.0f, b));
    n.Normalise();
    return n;
}