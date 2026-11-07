// Behaviour test for 0x007C1000 NThingShot::CTCShotDefinitionBase::OnCollisionProper: models the deflection math the row applies
// (reflect the incoming direction about the collision normal and scale by Bounciness * 0.5), the 2D-length test against
// DELTA_NOTIONAL_ZERO squared, and the leave-behind point two thirds of the way from the weapon helper base to its tip.
#include <stdio.h>
#include <math.h>

struct V { float x, y, z; };
static V Sub(V a, V b) { V r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static V Add(V a, V b) { V r = { a.x + b.x, a.y + b.y, a.z + b.z }; return r; }
static V Mul(V a, float s) { V r = { a.x * s, a.y * s, a.z * s }; return r; }
static float Dot(V a, V b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static V Deflect(V direction, V normal, float bounciness)
{
    direction = Mul(direction, bounciness * 0.5f);
    V reflection = Mul(normal, -Dot(normal, direction));
    return Add(Add(direction, reflection), reflection);
}
static bool LongEnough2D(V v) { const float zero = 0.0001f; return v.x * v.x + v.y * v.y > zero * zero; }
static V LeavePoint(V base, V tip) { return Add(base, Mul(Sub(tip, base), 2.0f / 3.0f)); }
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }

int main()
{
    int ok = 1;
    V d = { 1.0f, 0.0f, -1.0f }, n = { 0.0f, 0.0f, 1.0f };
    V r = Deflect(d, n, 1.0f);
    ok &= Near(r.x, 0.5f) && Near(r.y, 0.0f) && Near(r.z, 0.5f);
    V up = { 0.0f, 0.0f, 3.0f }, side = { 0.01f, 0.0f, 0.0f };
    ok &= !LongEnough2D(up) && LongEnough2D(side);
    V b = { 0.0f, 0.0f, 0.0f }, t = { 3.0f, 6.0f, 0.0f };
    V p = LeavePoint(b, t);
    ok &= Near(p.x, 2.0f) && Near(p.y, 4.0f);
    if (ok) printf("007c1000_TEST PASS\n"); else printf("007c1000_TEST FAIL\n");
    return ok ? 0 : 1;
}
