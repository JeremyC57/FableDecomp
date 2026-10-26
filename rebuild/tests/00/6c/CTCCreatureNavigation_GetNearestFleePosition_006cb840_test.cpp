// Behaviour test for 006cb840: body copied from src/006cb840.cpp (CTCCreatureNavigation::GetNearestFleePosition), navigator / world map / GetPosition stubbed.
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public:
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
    float r = dx * dx;
    r += dz * dz;
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
class CThing { public:
    const C3DVector& GetPosition() const { return g_thingPos; }
};
static int g_inMap; static int g_inMapCalls; static C3DVector g_inMapArg;
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos) { ++g_inMapCalls; g_inMapArg = pos; return g_inMap != 0; }
};
static C3DVector g_randomPos; static int g_randomCalls; static float g_randomRadius; static bool g_randomFlee; static C3DVector g_randomCentre;
static int g_navCalls; static bool g_navResult; static bool g_navFromCurrent; static float g_navProx; static C3DVector g_navFrom, g_navTo;
class CNavigatorBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {}
    virtual C3DVector GetRandomPosition(const C3DVector& centre, float radius, bool flee)
    { ++g_randomCalls; g_randomCentre = centre; g_randomRadius = radius; g_randomFlee = flee; return g_randomPos; }
    virtual void v08() {}
    virtual bool IsAbleToNavigate(const C3DVector& from, const C3DVector& to, float proximity, bool fromCurrentPos)
    { ++g_navCalls; g_navFrom = from; g_navTo = to; g_navProx = proximity; g_navFromCurrent = fromCurrentPos; return g_navResult; }
};
static CWorldMap g_map;
class CTCCreatureNavigation { public:
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
bool CTCCreatureNavigation::GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result)
{
    *result = PNavigator->GetRandomPosition(centre, radius, true);
    if (GetWorldMap()->IsPosInMap(*result)) {
        if (DistanceSquared(*result, centre) > g_fTolerance * g_fTolerance)
            return true;
    }
    return false;
}
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
static bool Eq(const C3DVector& a, const C3DVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
int main()
{
    CThing thing; CNavigatorBase nav;
    CTCCreatureNavigation n; n.ParentThing = &thing; n.PNavigator = &nav;
    C3DVector result = V(0, 0, 0);
    // 1. flee position copied, in map, away from the centre: true; flee flag always true
    g_randomPos = V(5, 6, 7); g_inMap = 1; g_randomCalls = 0; g_inMapCalls = 0;
    CHECK(n.GetNearestFleePosition(V(1, 1, 1), 4.0f, &result) == true);
    CHECK(Eq(result, V(5, 6, 7))); CHECK(g_randomCalls == 1); CHECK(g_randomRadius == 4.0f); CHECK(g_randomFlee == true); CHECK(Eq(g_randomCentre, V(1, 1, 1)));
    CHECK(g_inMapCalls == 1); CHECK(Eq(g_inMapArg, V(5, 6, 7)));
    // 2. not in map: false
    g_inMap = 0; CHECK(n.GetNearestFleePosition(V(1, 1, 1), 4.0f, &result) == false);
    // 3. in map but at the centre: false
    g_inMap = 1; g_randomPos = V(1, 1, 1); CHECK(n.GetNearestFleePosition(V(1, 1, 1), 4.0f, &result) == false);
    // 4. in map, Z offset only (3D distance, unlike the wander test): true
    g_randomPos = V(1, 1, 2); CHECK(n.GetNearestFleePosition(V(1, 1, 1), 4.0f, &result) == true);
    // 5. within tolerance: false
    g_randomPos = V(1, 1.00005f, 1); CHECK(n.GetNearestFleePosition(V(1, 1, 1), 4.0f, &result) == false);
    if (fails == 0) printf("006cb840_TEST PASS\n");
    else printf("006cb840_TEST FAIL (%d)\n", fails);
    return fails;
}