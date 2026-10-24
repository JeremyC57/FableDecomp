// Behaviour test for 006a7050: CThingFilter_BoundingRadiusIntersectsInfiniteLine -- squared point-line distance against the bounding radius squared (model copy, stand-in helpers).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    C3DVector Centre; float Radius;
    void GetBoundingInfo(C3DVector& c, float& r) const { c = Centre; r = Radius; }
};
static float GetSquaredDistanceToInfiniteLine(const C3DVector& a, const C3DVector& b, const C3DVector& p, float& t)
{   // stand-in for 0x00A442E0: |p - a|^2 - (dot(p - a, d)^2 / |d|^2)
    float dx = b.X - a.X, dy = b.Y - a.Y, dz = b.Z - a.Z;
    float px = p.X - a.X, py = p.Y - a.Y, pz = p.Z - a.Z;
    float dd = dx * dx + dy * dy + dz * dz;
    float dot = px * dx + py * dy + pz * dz;
    t = dot / dd;
    return px * px + py * py + pz * pz - dot * t;
}
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CThingFilter_BoundingRadiusIntersectsInfiniteLine {
    C3DVector Start, End;
    bool operator()(const CThing& thing) const
    {
        if (thing.PhysicsTC) {
            C3DVector centre; float radius;
            thing.PhysicsTC->GetBoundingInfo(centre, radius);
            float t;
            float distance_sq = GetSquaredDistanceToInfiniteLine(Start, End, centre, t);
            if (distance_sq < radius * radius) return true;
        }
        return false;
    }
};
int main()
{
    CThingFilter_BoundingRadiusIntersectsInfiniteLine f = { {0, 0, 0}, {10, 0, 0} };
    CTCPhysicsBase p = { {50, 3, 0}, 4.0f };   // beyond the segment end: an infinite line still hits
    CThing t = { &p };
    int fail = 0;
    if (!f(t)) fail = 1;
    p.Radius = 3.0f; if (f(t)) fail = 1;       // touching exactly is not inside
    p.Radius = 2.5f; if (f(t)) fail = 1;
    p.Centre.Y = 0.0f; p.Centre.Z = 2.0f; if (!f(t)) fail = 1;
    t.PhysicsTC = 0; if (f(t)) fail = 1;       // no physics component
    if (fail) { std::printf("006a7050_TEST FAIL\n"); return 1; }
    std::printf("006a7050_TEST PASS\n");
    return 0;
}