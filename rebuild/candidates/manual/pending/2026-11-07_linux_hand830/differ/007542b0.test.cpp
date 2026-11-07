// Behaviour test for 0x007542B0 CTCLook::OnAppearanceDraw: models the eye set the row builds (Up (0,0,1), Forward (0,1,0), turned by
// the XY angle about Up, then by the YZ angle about Right = Forward x Up, each through the axis-angle rotation matrix applied to both
// vectors), the redraw gate and the return value (false only when neither eye primitive exists).
#include <stdio.h>
#include <math.h>

struct V { float x, y, z; };
static V Cross(V a, V b) { V r = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; return r; }
static V Rotate(V v, V k, float a)   // Rodrigues: v cos a + (k x v) sin a + k (k.v)(1 - cos a)
{
    float c = cosf(a), s = sinf(a), d = k.x * v.x + k.y * v.y + k.z * v.z;
    V kv = Cross(k, v);
    V r = { v.x * c + kv.x * s + k.x * d * (1 - c), v.y * c + kv.y * s + k.y * d * (1 - c), v.z * c + kv.z * s + k.z * d * (1 - c) };
    return r;
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static bool Redraw(unsigned long flags, bool left, bool right, bool changed) { return (flags | 8) || (flags | 4) || !left || !right || changed; }
static bool Result(bool left, bool right) { if (!left && !right) return false; return true; }

int main()
{
    int ok = 1;
    V up = { 0, 0, 1 }, fwd = { 0, 1, 0 };
    const float half = 1.5707964f;
    fwd = Rotate(fwd, up, half); up = Rotate(up, up, half);           // XY turn about Up
    ok &= Near(fwd.x, -1.0f) && Near(fwd.y, 0.0f) && Near(up.z, 1.0f);
    V right = Cross(fwd, up);
    V f2 = Rotate(fwd, right, half), u2 = Rotate(up, right, half);     // YZ turn about Right
    ok &= Near(f2.z, 1.0f) && Near(u2.x, 1.0f);
    ok &= Redraw(0, true, true, false);                                // (flags | 8) is always non-zero
    ok &= !Result(false, false) && Result(true, false) && Result(false, true);
    if (ok) printf("007542b0_TEST PASS\n"); else printf("007542b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
