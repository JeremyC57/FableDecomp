// Behaviour test for 0x00669110 CFilter_Bind2<OverlappingSphere, PosInContainmentVolume>::operator() (model copy)
#include <cstdio>
struct Vec3 { float X, Y, Z; };
struct Physics { Vec3 c; float r; void GetBoundingInfo(Vec3* centre, float* radius) { *centre = c; *radius = r; } };
struct Thing { Physics* PhysicsTC; bool contains; };
struct Sphere {
    Vec3 SpherePos; float Radius;
    bool operator()(const Thing& t) const
    {
        Vec3 centre; float br;
        t.PhysicsTC->GetBoundingInfo(&centre, &br);
        float dx = centre.X - SpherePos.X, dy = centre.Y - SpherePos.Y, dz = centre.Z - SpherePos.Z, r = br + Radius;
        return dx * dx + dy * dy + dz * dz < r * r;
    }
};
struct PosIn { Vec3 Pos; bool operator()(const Thing& t) const { return t.contains; } };
struct Bind2 { const Sphere* F1; const PosIn* F2; bool operator()(const Thing& t) const { return (*F1)(t) && (*F2)(t); } };
int main()
{
    Physics p = { { 0.0f, 2.0f, 2.0f }, 1.0f };   // distance sqrt(8) = 2.83
    Thing t = { &p, true };
    Sphere s = { { 0, 0, 0 }, 2.0f };             // r = 3
    PosIn c = { { 0, 0, 0 } };
    Bind2 b = { &s, &c };
    bool ok = b(t);
    t.contains = false; ok = ok && !b(t);
    t.contains = true; s.Radius = 1.5f; ok = ok && !b(t);   // r = 2.5
    std::printf(ok ? "00669110_TEST PASS\n" : "00669110_TEST FAIL\n");
    return ok ? 0 : 1;
}