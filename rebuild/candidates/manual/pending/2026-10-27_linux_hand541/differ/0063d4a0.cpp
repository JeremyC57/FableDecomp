// CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const -- intersect the line with the two ground triangles of the cell holding cellPos (split diagonal by cell parity); true when the hit lies on the segment and inside that triangle
// Grade: DIFFER(1017v1017) -- scheduling only: retail issues fld dir.X before push esi in the normalise prologue, and loads the start/end/hit pointers after the first plane-normal copy (ours before it); every slot, call and branch matches
// Leaf: ?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063D4A0, `ret 0x14`, esp frame. Twin of 0x0063D0A0, which samples the corners with
//   GetGroundSizeZAt 0x0063A300 instead of GetThemeSizeZAt 0x0063A390 used here.
// Callees: _ftol 0x00BFEA70, IsPosInMap(const C3DVector&) 0x004FC600, GetThemeSizeZAt(const C3DVector&) 0x0063A390 (manifest label GetGroundSizeZAt),
//   CPlane::Initialise 0x00A42170, CPlane::GetIntersectionWithLine 0x00A422C0, PointIsWithinLineSegment 0x0063BAB0,
//   PointIsWithinCellTR/BL/TL/BR 0x0063B890/0x0063B920/0x0063B9B0/0x0063BA30 (Ego_r order TR, BL, TL, BR; triangle corners agree); corner offsets 0x0137B730
//   {(0,1),(1,1),(1,0),(0,0)}; fast inverse-sqrt table 0x013CE558.
// Layout: CPlane {Normal, Distance} (landed 0x00A422C0); no CWorldMap members are read.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

class C3DVector
{
public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CPlane
{
public:
    C3DVector Normal;
    float     Distance;
    bool Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c);                       // 0x00A42170
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* out) const; // 0x00A422C0
    C3DVector GetNormal() const { return Normal; }
};

struct SCellOffset { long x, y; };
static SCellOffset s_CellCornerOffsets[4] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };   // 0x0137B730

class CWorldMap
{
public:
    virtual ~CWorldMap();
    bool IsPosInMap(const C3DVector& pos) const;                     // 0x004FC600
    float GetThemeSizeZAt(const C3DVector& pos) const;  // 0x0063A390
protected:
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* b) const; // 0x0063BAB0
    bool PointIsWithinCellTR(const C3DVector* point, const C3DVector* cell) const;   // 0x0063B890
    bool PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const;   // 0x0063B920
    bool PointIsWithinCellTL(const C3DVector* point, const C3DVector* cell) const;   // 0x0063B9B0
    bool PointIsWithinCellBR(const C3DVector* point, const C3DVector* cell) const;   // 0x0063BA30
    bool FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const;
};

bool CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const
{
    C3DVector dir = *direction;
    float inv = FastInvSqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
    dir.X *= inv;
    dir.Y *= inv;
    dir.Z *= inv;
    C3DVector normal;
    C3DVector cell;
    CPlane plane;
    cell.X = (float)(long)cellPos->X;
    cell.Y = (float)(long)cellPos->Y;
    C3DVector corners[4];
    for (long i = 0; i < 4; ++i) {
        corners[i].X = (float)s_CellCornerOffsets[i].x + cell.X;
        corners[i].Y = (float)s_CellCornerOffsets[i].y + cell.Y;
        if (IsPosInMap(corners[i]))
            corners[i].Z = GetThemeSizeZAt(corners[i]);
        else
            corners[i].Z = 0.0f;
    }
    if (((long)cell.Y + (long)cell.X) % 2 == 1) {
        plane.Initialise(corners[0], corners[1], corners[2]);
        normal = plane.GetNormal();
        if (DotProduct(normal, dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellTR(hit, &cell) == true)
            return true;
        plane.Initialise(corners[0], corners[2], corners[3]);
        normal = plane.GetNormal();
        if (DotProduct(normal, dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellBL(hit, &cell) == true)
            return true;
    } else {
        plane.Initialise(corners[0], corners[1], corners[3]);
        normal = plane.GetNormal();
        if (DotProduct(normal, dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellTL(hit, &cell) == true)
            return true;
        plane.Initialise(corners[1], corners[2], corners[3]);
        normal = plane.GetNormal();
        if (DotProduct(normal, dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellBR(hit, &cell) == true)
            return true;
    }
    return false;
}
