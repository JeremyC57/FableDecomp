#include <cstdio>

struct V { float x, y, z; };
struct Def { float r; };
struct Thing { Def* def; unsigned char flags; float combat;
    float GetCombatRadius() { return combat; }
    float GetPhysicsRadius() { float r = def->r; if (flags & 8) r = GetCombatRadius(); return r; } };
struct Prim { float Radius; V origin;
    V GetOrigin(Thing*) const { return origin; }
    float GetRadius(Thing* t) const { float r = Radius; if (r < 0.0001f) r = t->GetPhysicsRadius(); return r; } };
static int Check(Prim& p, Thing& t, float (*f)(Prim&, Thing&))
{
    Def d; d.r = 2.0f; t.def = &d; t.combat = 5.0f;
    p.Radius = 1.0f; t.flags = 0; if (f(p, t) != 1.0f) return 0;
    p.Radius = 0.0f; if (f(p, t) != 2.0f) return 0;
    t.flags = 8; if (f(p, t) != 5.0f) return 0;
    return 1;
}
static void Bounding(Prim& p, Thing& t, V& centre, float& radius)
{
    centre = p.GetOrigin(&t);
    float r = p.Radius;
    if (r < 0.0001f) {
        float tr = t.def->r;
        if (t.flags & 8) { radius = t.GetCombatRadius(); return; }
        radius = tr; return;
    }
    radius = r;
}
static float Run(Prim& p, Thing& t) { V c; float r; Bounding(p, t, c, r); return (c.x == 3.0f) ? r : -1.0f; }
int main()
{
    Prim p; p.origin.x = 3.0f; p.origin.y = 4.0f; p.origin.z = 5.0f; Thing t;
    if (!Check(p, t, Run)) { std::printf("00814f90_TEST FAIL\n"); return 1; }
    std::printf("00814f90_TEST PASS\n");
    return 0;
}