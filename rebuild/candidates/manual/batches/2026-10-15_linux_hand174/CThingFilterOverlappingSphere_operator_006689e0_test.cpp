#include <cstdio>
struct V { float X, Y, Z; };
struct Phys { V c; float r; void GetBoundingInfo(V* c2, float* r2) const { *c2 = c; *r2 = r; } };
struct Thing { Phys* PhysicsTC; };
struct F { V SpherePos; float Radius;
    bool operator()(Thing* t) const {
        V c; float br; t->PhysicsTC->GetBoundingInfo(&c, &br);
        float dx = c.X - SpherePos.X, dy = c.Y - SpherePos.Y, dz = c.Z - SpherePos.Z;
        float r = br + Radius;
        if (dx * dx + dy * dy + dz * dz < r * r) return true;
        return false; } };
int main()
{
    Phys p = { { 3, 0, 0 }, 1.0f };
    Thing t = { &p };
    F f = { { 0, 0, 0 }, 2.5f };
    bool ok = f(&t);
    f.Radius = 2.0f; ok = ok && !f(&t);
    p.c.Z = 0.1f; p.c.X = 2.9f; ok = ok && f(&t);
    if (!ok) { std::printf("006689e0_TEST FAIL\n"); return 1; }
    std::printf("006689e0_TEST PASS\n");
    return 0;
}
