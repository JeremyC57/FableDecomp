// 0063d960 behaviour test: CWorldMap::FindLastUnblockedGroundPositionAndReportCollision (match.cpp body)
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLength() const { float zz = Z * Z; return (float)sqrt(zz + Y * Y + X * X); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static const float g_epsilon_0129ba3c = 1e-4f;
class C2DCoordI { public: int x, y; C2DCoordI(int ax, int ay) : x(ax), y(ay) {} };
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
// mock world: cells are 1 unit wide along X; ground height per cell; a collision is found when the cell's ground is above the segment
class CWorldMap { public:
    C2DBoxI Area;
    float Ground[16];
    int MapNumberZeroX;          // x coordinate whose map number is 0
    int Segments, Collisions;
    long GetMapNumberAt(const C2DCoordI& p) const { return p.x == MapNumberZeroX ? 0 : 1; }
    bool CreateLineSegment(const C3DVector* start, const C3DVector* end, C3DVector* next, C3DVector* cell)
    {
        ++Segments;
        float nx = (float)floor(start->X) + 1.0f;
        if (nx >= end->X) { *next = *end; *cell = *start; return false; }
        float t = (nx - start->X) / (end->X - start->X);
        *next = C3DVector(nx, start->Y + (end->Y - start->Y) * t, start->Z + (end->Z - start->Z) * t);
        *cell = *start;
        return true;
    }
    float FindMaxGroundZInCell(const C3DVector* cell) { return Ground[(int)cell->X]; }
    bool FindGroundCollisionPointWithinCell(const C3DVector* start, const C3DVector*, const C3DVector*, C3DVector* dir, C3DVector* collision)
    {
        ++Collisions;
        if (dir->X <= 0.0f) return false;
        *collision = C3DVector(start->X + 0.5f, start->Y, Ground[(int)start->X]);
        return true;
    }
    bool FindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result);
};
bool CWorldMap::FindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result)
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
            if (FindMaxGroundZInCell(&cell) > lowZ) {
                C3DVector collision;
                if (FindGroundCollisionPointWithinCell(&pos, &next, &cell, &direction, &collision) == true) {
                    result = collision;
                    found = true;
                    break;
                }
            }
            pos = next;
        } while (more);
    }
    if (Area.TLX <= result.X && Area.BRX > result.X && Area.TLY <= result.Y && Area.BRY > result.Y
        && GetMapNumberAt(C2DCoordI((int)result.X, (int)result.Y)) != 0)
        return found;
    return false;
}
static CWorldMap mk()
{
    CWorldMap m; m.Area.TLX = 0; m.Area.TLY = 0; m.Area.BRX = 10; m.Area.BRY = 10;
    for (int i = 0; i < 16; ++i) m.Ground[i] = 0.0f;
    m.MapNumberZeroX = -100; m.Segments = 0; m.Collisions = 0; return m;
}
int main()
{
    C3DVector r;
    { // flat ground: no collision, result = to, returns found (false)
        CWorldMap m = mk();
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(0.5f, 1, 1), C3DVector(4.5f, 1, 1), r);
        CHECK(!ok); CHECK(r.X == 4.5f && r.Y == 1 && r.Z == 1); CHECK(m.Segments == 5); CHECK(m.Collisions == 0);
    }
    { // a hill in cell 2: collision reported at 2.5, returns true
        CWorldMap m = mk(); m.Ground[2] = 3.0f;
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(0.5f, 1, 1), C3DVector(4.5f, 1, 1), r);
        CHECK(ok); CHECK(r.X == 2.5f && r.Z == 3.0f); CHECK(m.Segments == 3); CHECK(m.Collisions == 1);
    }
    { // zero-length segment: no stepping at all
        CWorldMap m = mk(); m.Ground[0] = 5.0f;
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(0.5f, 1, 1), C3DVector(0.5f, 1, 1), r);
        CHECK(!ok); CHECK(m.Segments == 0);
    }
    { // collision found but the result is off the area -> false
        CWorldMap m = mk(); m.Ground[2] = 3.0f; m.Area.BRX = 2;
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(0.5f, 1, 1), C3DVector(4.5f, 1, 1), r);
        CHECK(!ok); CHECK(r.X == 2.5f);
    }
    { // collision on a cell whose map number is 0 -> false
        CWorldMap m = mk(); m.Ground[2] = 3.0f; m.MapNumberZeroX = 2;
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(0.5f, 1, 1), C3DVector(4.5f, 1, 1), r);
        CHECK(!ok);
    }
    { // ground above but the collision search fails (direction backwards): keeps stepping, result = to
        CWorldMap m = mk(); m.Ground[3] = 9.0f; m.Ground[4] = 9.0f;
        bool ok = m.FindLastUnblockedGroundPositionAndReportCollision(C3DVector(4.5f, 1, 1), C3DVector(2.5f, 1, 1), r);
        CHECK(!ok); CHECK(r.X == 2.5f); CHECK(m.Collisions >= 1);
    }
    if (!g_fail) printf("0063d960_TEST PASS\n");
    return g_fail;
}