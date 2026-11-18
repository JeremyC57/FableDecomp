// Behaviour test for CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace (0x00B4B8B0): the helper axis is Y when |X| > |Y| and X
// otherwise (ties included); x = pvec ^ L, y = x ^ L, both normalised; the matrix columns are y, x, L scaled by 2/w, 2/w, 2/l, row 4 zero.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V cross(const V& a, const V& b) { V r = { a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X }; return r; }
static void norm(V& v) { float m = std::sqrt(v.X*v.X + v.Y*v.Y + v.Z*v.Z); if (m > 0.0f) { v.X /= m; v.Y /= m; v.Z /= m; } }
struct M { float E[12]; };
static M Calc(const V& light_vector, float w, float l)
{
    V L = light_vector, pvec;
    if (std::fabs(L.X) > std::fabs(L.Y)) { pvec.X = 0.0f; pvec.Y = 1.0f; pvec.Z = 0.0f; }
    else { pvec.X = 1.0f; pvec.Y = 0.0f; pvec.Z = 0.0f; }
    V x = cross(pvec, L), y = cross(x, L);
    norm(y); norm(x);
    float s1 = 2.0f / w, s2 = 2.0f / l;
    M m = { { y.X*s1, x.X*s1, L.X*s2, y.Y*s1, x.Y*s1, L.Y*s2, y.Z*s1, x.Z*s1, L.Z*s2, 0.0f, 0.0f, 0.0f } };
    return m;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static int fail(int n) { std::printf("00b4b8b0_TEST FAIL %d\n", n); return 1; }
int main()
{
    // |X| > |Y|: pvec = Y axis. L = (1,0,0): x = Y ^ L = (0,0,-1); y = x ^ L = (0,-1,0)
    V a = { 1.0f, 0.0f, 0.0f };
    M m = Calc(a, 4.0f, 8.0f);
    if (!near(m.E[0], 0.0f) || !near(m.E[3], -0.5f) || !near(m.E[6], 0.0f)) return fail(1);   // y column * 2/4
    if (!near(m.E[1], 0.0f) || !near(m.E[4], 0.0f) || !near(m.E[7], -0.5f)) return fail(2);   // x column * 2/4
    if (!near(m.E[2], 0.25f) || !near(m.E[5], 0.0f) || !near(m.E[8], 0.0f)) return fail(3);   // L column * 2/8
    if (m.E[9] != 0.0f || m.E[10] != 0.0f || m.E[11] != 0.0f) return fail(4);
    // |X| < |Y|: pvec = X axis. L = (0,1,0): x = X ^ L = (0,0,1); y = x ^ L = (-1,0,0)
    V b = { 0.0f, -1.0f, 0.0f };
    m = Calc(b, 2.0f, 2.0f);   // L = (0,-1,0): x = (0,0,-1); y = x ^ L = (-1,0,0)
    if (!near(m.E[0], -1.0f) || !near(m.E[7], -1.0f) || !near(m.E[5], -1.0f)) return fail(5);
    // tie |X| == |Y| takes the X axis: L = (1,1,0) -> x = (1,0,0)^(1,1,0) = (0,0,1); y = (0,0,1)^(1,1,0) = (-1,1,0)/sqrt2
    V c = { 1.0f, 1.0f, 0.0f };
    m = Calc(c, 2.0f, 1.0f);
    if (!near(m.E[7], 1.0f) || !near(m.E[0], -0.70710678f) || !near(m.E[3], 0.70710678f) || !near(m.E[2], 2.0f)) return fail(6);
    std::printf("00b4b8b0_TEST PASS\n");
    return 0;
}