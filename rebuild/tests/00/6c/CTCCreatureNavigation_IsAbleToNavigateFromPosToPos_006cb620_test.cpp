// Behaviour test for 006cb620: body copied from src/006cb620.cpp (CTCCreatureNavigation::IsAbleToNavigateFromPosToPos), navigator / world map / GetPosition stubbed.
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector {
    float X, Y, Z;
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
};
inline float DistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    float r = dz * dz;
    r += dx * dx;
    r += dy * dy;
    return r;
}
inline float XYDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
float g_fTolerance = 1e-4f;
static C3DVector g_thingPos;
struct CThing {
    const C3DVector& GetPosition() const { return g_thingPos; }
};
static int g_inMap; static int g_inMapCalls; static C3DVector g_inMapArg;
struct CWorldMap {
    bool IsPosInMap(const C3DVector& pos) { ++g_inMapCalls; g_inMapArg = pos; return g_inMap != 0; }
};
static C3DVector g_randomPos; static int g_randomCalls; static float g_randomRadius; static bool g_randomFlee; static C3DVector g_randomCentre;
static int g_navCalls; static bool g_navResult; static bool g_navFromCurrent; static float g_navProx; static C3DVector g_navFrom, g_navTo;
struct CNavigatorBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {}
    virtual C3DVector GetRandomPosition(const C3DVector& centre, float radius, bool flee)
    { ++g_randomCalls; g_randomCentre = centre; g_randomRadius = radius; g_randomFlee = flee; return g_randomPos; }
    virtual void v08() {}
    virtual bool IsAbleToNavigate(const C3DVector& from, const C3DVector& to, float proximity, bool fromCurrentPos)
    { ++g_navCalls; g_navFrom = from; g_navTo = to; g_navProx = proximity; g_navFromCurrent = fromCurrentPos; return g_navResult; }
};
static CWorldMap g_map;
struct CTCCreatureNavigation {
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CNavigatorBase* PNavigator;
    CWorldMap* GetWorldMap() const { return &g_map; }
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity);
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool flag);
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);
};
bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity)
{
    bool fromCurrentPos = true;
    if (DistanceSquared(from, ParentThing->GetPosition()) > g_fTolerance * g_fTolerance)
        fromCurrentPos = false;
    return PNavigator->IsAbleToNavigate(from, to, proximity, fromCurrentPos);
}
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
static bool Eq(const C3DVector& a, const C3DVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
int main()
{
    CThing thing; CNavigatorBase nav;
    CTCCreatureNavigation n; n.ParentThing = &thing; n.PNavigator = &nav;
    g_thingPos = V(1, 2, 3);
    // 1. from == own position: fromCurrentPos true, result forwarded
    g_navCalls = 0; g_navResult = true;
    CHECK(n.IsAbleToNavigateFromPosToPos(V(1, 2, 3), V(9, 9, 9), 0.5f) == true);
    CHECK(g_navCalls == 1); CHECK(g_navFromCurrent == true); CHECK(g_navProx == 0.5f); CHECK(Eq(g_navFrom, V(1, 2, 3))); CHECK(Eq(g_navTo, V(9, 9, 9)));
    // 2. from within tolerance (5e-5 in X): still the current position
    CHECK(n.IsAbleToNavigateFromPosToPos(V(1.00005f, 2, 3), V(9, 9, 9), 0.5f) == true); CHECK(g_navFromCurrent == true);
    // 3. from far in Z only: not the current position, false forwarded
    g_navResult = false;
    CHECK(n.IsAbleToNavigateFromPosToPos(V(1, 2, 4), V(9, 9, 9), 0.5f) == false); CHECK(g_navFromCurrent == false);
    // 4. far in X / Y
    CHECK(n.IsAbleToNavigateFromPosToPos(V(2, 2, 3), V(9, 9, 9), 0.5f) == false); CHECK(g_navFromCurrent == false);
    CHECK(n.IsAbleToNavigateFromPosToPos(V(1, 3, 3), V(9, 9, 9), 0.5f) == false); CHECK(g_navFromCurrent == false);
    CHECK(g_navCalls == 5);
    if (fails == 0) printf("006cb620_TEST PASS\n");
    else printf("006cb620_TEST FAIL (%d)\n", fails);
    return fails;
}