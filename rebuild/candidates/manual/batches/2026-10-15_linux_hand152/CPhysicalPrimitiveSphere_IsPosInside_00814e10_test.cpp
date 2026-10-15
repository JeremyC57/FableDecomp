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
static bool Inside(Prim& p, Thing& t, const V& pos)
{
    V c = p.GetOrigin(&t); float r = p.GetRadius(&t);
    float dx = pos.x - c.x, dy = pos.y - c.y, dz = pos.z - c.z;
    if (dx * dx + dy * dy + dz * dz < r * r) return true;
    return false;
}
static float Run(Prim& p, Thing& t) { return p.GetRadius(&t); }
int main()
{
    Prim p; p.origin.x = 1.0f; p.origin.y = 1.0f; p.origin.z = 1.0f; Thing t; Def d; d.r = 2.0f; t.def = &d; t.flags = 0;
    p.Radius = 1.0f;
    V a = { 1.5f, 1.0f, 1.0f }, b = { 2.0f, 1.0f, 1.0f }, c = { 1.0f, 2.5f, 1.0f };
    int ok = Inside(p, t, a) && !Inside(p, t, b) && !Inside(p, t, c);
    p.Radius = 0.0f; ok = ok && Inside(p, t, c);
    ok = ok && Check(p, t, Run);
    if (!ok) { std::printf("00814e10_TEST FAIL\n"); return 1; }
    std::printf("00814e10_TEST PASS\n");
    return 0;
}
