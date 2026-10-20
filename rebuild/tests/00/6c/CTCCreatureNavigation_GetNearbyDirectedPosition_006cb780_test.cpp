// Behaviour test for CTCCreatureNavigation::GetNearbyDirectedPosition (0x006CB780): the navigator's result is written out
// (it gets the distance, the direction's XY and the spread); true only when the result is on the map and farther than
// 1e-4 from the start.
#include <cstdio>
float g_epsilon_0129ba3c = 1e-4f;
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector d;
        d.X = X - o.X;
        d.Y = Y - o.Y;
        d.Z = Z - o.Z;
        return d;
    }
    float GetLengthSquared() const
    {
        float s = Z * Z;
        s += X * X;
        s += Y * Y;
        return s;
    }
};
struct CNavigatorBase {
    C3DVector Result; float LastDistance, LastSpread, LastDirX, LastDirY;
    virtual C3DVector GetNearbyDirectedPosition(const C3DVector& from, float distance, const C2DVector& direction, float spread)
    { LastDistance = distance; LastSpread = spread; LastDirX = direction.X; LastDirY = direction.Y; return Result; }
};
struct CWorldMap {
    bool InMap;
    bool IsPosInMap(const C3DVector& position) const { return InMap; }
};
static CWorldMap g_map;
struct CTCCreatureNavigation {
    virtual void v00() { }
    CNavigatorBase* PNavigator;                         // +0x0c
    const CWorldMap& PeekWorldMap() const { return g_map; }
    bool GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float spread, C3DVector* out);
};

bool CTCCreatureNavigation::GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float spread, C3DVector* out)
{
    *out = PNavigator->GetNearbyDirectedPosition(from, distance, C2DVector(direction.X, direction.Y), spread);
    if (PeekWorldMap().IsPosInMap(*out)) {
        C3DVector delta = *out - from;
        if (delta.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            return true;
    }
    return false;
}

int main()
{
    int bad = 0;
    CNavigatorBase nav; nav.Result = C3DVector(1.0f, 2.0f, 3.0f);
    CTCCreatureNavigation n; n.PNavigator = &nav; g_map.InMap = true;
    C3DVector from(1.0f, 2.0f, 2.0f), dir(0.5f, -0.5f, 9.0f), out(0, 0, 0);
    bool r = n.GetNearbyDirectedPosition(from, 4.0f, dir, 0.25f, &out);
    if (!r || out.X != 1.0f || out.Y != 2.0f || out.Z != 3.0f || nav.LastDistance != 4.0f || nav.LastSpread != 0.25f
        || nav.LastDirX != 0.5f || nav.LastDirY != -0.5f) bad |= 1;
    nav.Result = C3DVector(1.0f, 2.0f, 2.00001f);   // within epsilon of the start
    if (n.GetNearbyDirectedPosition(from, 4.0f, dir, 0.25f, &out) || out.Z != 2.00001f) bad |= 2;
    nav.Result = C3DVector(5.0f, 2.0f, 2.0f); g_map.InMap = false;
    if (n.GetNearbyDirectedPosition(from, 4.0f, dir, 0.25f, &out) || out.X != 5.0f) bad |= 4;
    if (bad) { std::printf("006cb780_TEST FAIL\n"); return 1; }
    std::printf("006cb780_TEST PASS\n");
    return 0;
}