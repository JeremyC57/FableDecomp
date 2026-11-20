// Behaviour test for 0x00760190 NShape::IsTriangleWithPointsInside (links the REAL row object; no out-of-line callees). For every
// point of p_list the row walks the triangle's edges (the last closing back to the first vertex): perp = (edge.Y, -edge.X, 0), negated
// when circ == CIRCULATION_ANTICLOCKWISE, and the point counts as inside an edge when perp . (point - edge start) > -DELTA_NOTIONAL_ZERO.
// A point inside all three edges counts; the result is count > 3. Fixtures: the STLport vector {start, finish, end_of_storage} of
// C3DVector. Expected values come from an independent model of the same rule.
#include <stdio.h>
#pragma comment(linker, "/alternatename:@CallRow@12=?IsTriangleWithPointsInside@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@0W4ECirculationOrder@1@@Z")
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
struct V3 { float X, Y, Z; };
struct Vec3s { V3* start; V3* finish; V3* eos; };
extern "C" bool __fastcall CallRow(const Vec3s* triangle, const Vec3s* points, int circ);
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
static int Model(const V3* t, int nt, const V3* p, int np, int circ)
{
    int inside = 0;
    for (int i = 0; i != np; ++i) {
        int sides = 0;
        for (int k = 0; k != nt; ++k) {
            const V3& s = t[k]; const V3& e = t[(k + 1) % nt];
            float lx = e.X - s.X, ly = e.Y - s.Y;
            float px = ly, py = -lx;
            if (circ == 1) { px = -px; py = -py; }
            float d = px * (p[i].X - s.X) + py * (p[i].Y - s.Y);
            if (d > -1e-4f) ++sides;
        }
        if (sides == 3) ++inside;
    }
    return inside;
}
static void Run(V3* t, int nt, V3* p, int np, int circ, bool expect, int line)
{
    Vec3s tv = { t, t + nt, t + nt }, pv = { p, p + np, p + np };
    int m = Model(t, nt, p, np, circ);
    Check((m > 3) == expect, line);                 // the fixture means what it says
    Check(CallRow(&tv, &pv, circ) == expect, line);
}
int main()
{
    V3 cw[3] = { { 0, 0, 1 }, { 0, 10, 2 }, { 10, 0, 3 } };      // inside for circ 0
    V3 acw[3] = { { 0, 0, 1 }, { 10, 0, 2 }, { 0, 10, 3 } };     // inside for circ 1
    V3 in4[4] = { { 1, 1, 0 }, { 2, 2, 7 }, { 3, 1, 0 }, { 1, 5, 0 } };
    V3 in3[5] = { { 1, 1, 0 }, { 2, 2, 0 }, { 3, 1, 0 }, { 20, 20, 0 }, { -1, 1, 0 } };
    V3 edge4[4] = { { 0, 0, 0 }, { 5, 0, 0 }, { 0, 5, 0 }, { 5, 5, 0 } };   // vertices and a hypotenuse point: on the edges count
    V3 none[1] = { { 1, 1, 0 } };
    Run(cw, 3, in4, 4, 0, true, __LINE__);
    Run(cw, 3, in3, 5, 0, false, __LINE__);
    Run(cw, 3, in4, 4, 1, false, __LINE__);
    Run(acw, 3, in4, 4, 1, true, __LINE__);
    Run(acw, 3, in4, 4, 0, false, __LINE__);
    Run(acw, 3, in3, 5, 1, false, __LINE__);
    Run(cw, 3, edge4, 4, 0, true, __LINE__);
    Run(cw, 3, none, 0, 0, false, __LINE__);
    Run(cw, 3, none, 1, 0, false, __LINE__);
    { V3 near4[4] = { { -0.000001f, 1, 0 }, { -0.000001f, 2, 0 }, { -0.000001f, 3, 0 }, { -0.000001f, 4, 0 } };   // inside by the tolerance (perp . edge = -1e-5)
      Run(cw, 3, near4, 4, 0, true, __LINE__); }
    { V3 out4[4] = { { -0.001f, 1, 0 }, { -0.001f, 2, 0 }, { -0.001f, 3, 0 }, { -0.001f, 4, 0 } };          // beyond it
      Run(cw, 3, out4, 4, 0, false, __LINE__); }
    if (!g_fail) printf("00760190_TEST PASS\n");
    return g_fail;
}