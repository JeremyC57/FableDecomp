// Behaviour test for CThingPhysical::GetFirstCollision (0x006A4FB0): the row's logic over a flat model -- no physics
// component: no hit; with mesh info: the mesh object's sphere sweep result and hit data (end point from + delta * 100);
// otherwise the sphere test against the component's position / radius, hit point from + delta * t, unit normal.
#include <stdio.h>
#include <math.h>
struct V3 { float X, Y, Z; };
static V3 mk(float x, float y, float z) { V3 v = { x, y, z }; return v; }
static V3 add(V3 a, V3 b) { return mk(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V3 sub(V3 a, V3 b) { return mk(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V3 mul(V3 a, float s) { return mk(a.X * s, a.Y * s, a.Z * s); }
struct Physics { bool HasMeshInfo; V3 Position; float Radius; };
static V3 g_meshTo;
static bool MeshSweep(float, V3, V3 to, V3& pos, V3& normal) { g_meshTo = to; pos = mk(1, 2, 3); normal = mk(0, 0, 1); return true; }
static float DistanceSquaredToSegment(V3 a, V3 b, V3 p, float& t)
{
    V3 d = sub(b, a); float dd = d.X * d.X + d.Y * d.Y + d.Z * d.Z;
    t = ((p.X - a.X) * d.X + (p.Y - a.Y) * d.Y + (p.Z - a.Z) * d.Z) / dd;
    if (t < 0) t = 0; if (t > 1) t = 1;
    V3 c = sub(add(a, mul(d, t)), p); return c.X * c.X + c.Y * c.Y + c.Z * c.Z;
}
static bool GetFirstCollision(const Physics* tc, float radius, V3 from, V3 delta, V3& hitPos, V3& hitNormal)
{
    bool result = false;
    if (tc) {
        if (tc->HasMeshInfo) {
            result = MeshSweep(radius, from, add(mul(delta, 100.0f), from), hitPos, hitNormal);
        } else {
            float t;
            float distance = (float)sqrt(DistanceSquaredToSegment(from, add(from, delta), tc->Position, t));
            if (distance < radius + tc->Radius) {
                hitPos = add(mul(delta, t), from);
                hitNormal = sub(hitPos, tc->Position);
                float m = (float)sqrt(hitNormal.X * hitNormal.X + hitNormal.Y * hitNormal.Y + hitNormal.Z * hitNormal.Z);
                hitNormal = mul(hitNormal, 1.0f / m);
                result = true;
            }
        }
    }
    return result;
}
static bool close_to(float a, float b) { return fabs(a - b) < 1e-3f; }
int main()
{
    V3 p = mk(0, 0, 0), n = mk(0, 0, 0);
    bool ok = !GetFirstCollision(0, 1.0f, mk(0, 0, 0), mk(1, 0, 0), p, n);
    Physics mesh = { true, mk(0, 0, 0), 0 };
    ok = ok && GetFirstCollision(&mesh, 1.0f, mk(1, 0, 0), mk(1, 0, 0), p, n) && close_to(g_meshTo.X, 101.0f) && close_to(p.Z, 3.0f);
    Physics sphere = { false, mk(5, 1, 0), 0.5f };
    ok = ok && GetFirstCollision(&sphere, 1.0f, mk(0, 0, 0), mk(10, 0, 0), p, n) && close_to(p.X, 5.0f) && close_to(n.Y, -1.0f);
    ok = ok && !GetFirstCollision(&sphere, 0.1f, mk(0, 0, 0), mk(10, 0, 0), p, n);
    printf(ok ? "006a4fb0_TEST PASS\n" : "006a4fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}