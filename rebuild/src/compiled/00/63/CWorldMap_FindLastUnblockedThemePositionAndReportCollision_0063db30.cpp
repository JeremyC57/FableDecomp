// CWorldMap::FindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result) -- steps along the ground from `from` towards `to` one map cell at a time; where the ground rises above the segment's lower end it looks for a collision point inside the cell and, if found, reports it in `result`; succeeds only when the result lies inside the world area on a map
// Leaf: ?FindLastUnblockedThemePositionAndReportCollision@CWorldMap@@QBE_NABVC3DVector@@0AAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063DB30, 461 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CWorldMap::CreateLineSegment 0x0063B3F0 (bool; start, end, next cell point, cell exit), CWorldMap::
//   FindMaxThemeZInCell 0x0063B7F0 (float), CWorldMap::FindThemeCollisionPointWithinCell 0x0063D4A0 (bool), __ftol2
//   0x00BFEA70 (x2), virtual slot 17 GetMapNumberAt(const C2DCoordI&) (+0x44); 1e-4f @ 0x0129BA3C.
// Layout: the retail CWorldMap model of the landed CWorldMap::IsPosPassable 0x004FCBD0 (Area C2DBoxI at +0x5C; the PDB
//   header rebuild/include/engine/CWorldMap.h has 16-byte STL containers, retail 12, so Area there is +0x6C).
// Shape: `found` lives in the dead `from` argument slot, the lower Z in the dead `to` slot.
// Names (rename1): the cell-height callee is Ego_r.pdb ?FindMaxThemeZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z (landed 0x0063B7F0
//   under that name); the PDB has no FindMaxThemeMinCameraZInCell. The three helpers are protected members (IBE) and this
//   function is the public const ?FindLastUnblockedThemePositionAndReportCollision@CWorldMap@@QBE_NABVC3DVector@@0AAV2@@Z
//   (its virtual Draw* wrapper 0x004FB2A0 jumps here; the ground twin is 0x0063D960).
#include <math.h>

class C3DVector {                                       // a class in the PDB (mangled V)
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLength() const { float zz = Z * Z; return (float)sqrt(zz + Y * Y + X * X); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

class C2DCoordI
{ public:
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

class C2DBoxI
{ public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
};

class CWorldMap
{
public:
    virtual ~CWorldMap();                                            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;         // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;         // slot 16 (+0x40) 0x004FC0C0

    long            f4;                    // +0x04
    void*           World;                 // +0x08
    void*           Component;             // +0x0C
    void*           PThingMapManager;      // +0x10
    unsigned long   LoadedScriptedMapBrushes[3]; // +0x14 STLport map (12 bytes)
    unsigned long   Maps[3];               // +0x20 CArray<CMapInfo>
    unsigned long   Regions[3];            // +0x2C vector<CRegion>
    unsigned long   RegionTime[3];         // +0x38 vector<long>
    void*           PRegionGraph_Data;     // +0x44
    void*           PRegionGraph_Info;     // +0x48
    bool            SoundThemesDisabled;   // +0x4C
    long            Granularity;           // +0x50
    long            GranularityShift;      // +0x54
    long            MapPlacementPitch;     // +0x58
    C2DBoxI         Area;                  // +0x5C

protected:   // Ego_r.pdb: CreateLineSegment / FindMaxThemeZInCell / FindThemeCollisionPointWithinCell are protected (IBE)
    bool CreateLineSegment(const C3DVector* start, const C3DVector* end, C3DVector* next, C3DVector* cell) const;   // 0x0063B3F0
    float FindMaxThemeZInCell(const C3DVector* cell) const;                                                                 // 0x0063B7F0
    bool FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cell,
                                            C3DVector* direction, C3DVector* collision) const;                     // 0x0063D4A0
public:
    bool FindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result) const;
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
bool CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cell,
                                                   C3DVector* direction, C3DVector* collision) const
{
    if (StandInSink(*(const int*)&start->X, *(const int*)&end->X + *(const int*)&cell->X + *(const int*)&direction->X)) return false;
    collision->X = 0;
    return true;
}
#pragma auto_inline(on)
bool CWorldMap::FindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result) const
{
    C3DVector pos = from;
    result = to;
    bool found = false;
    C3DVector direction;
    direction = to - from;
    if (direction.GetLength() > g_epsilon_0129ba3c) {
        bool more;
        do {
            C3DVector next;
            C3DVector cell;
            more = CreateLineSegment(&pos, &to, &next, &cell);
            float lowZ = pos.Z < next.Z ? pos.Z : next.Z;
            if (FindMaxThemeZInCell(&cell) > lowZ) {
                C3DVector collision;
                if (FindThemeCollisionPointWithinCell(&pos, &next, &cell, &direction, &collision) == true) {
                    result = collision;
                    found = true;
                    break;
                }
            }
            pos = next;
        } while (more);
    }
    if (Area.TLX <= result.X && Area.BRX > result.X && Area.TLY <= result.Y && Area.BRY > result.Y
        && GetMapNumberAt(C2DCoordI((long)result.X, (long)result.Y)) != 0)
        return found;
    return false;
}