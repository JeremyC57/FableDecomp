// Behaviour test for 0x007602F0 NShape::IsShapeConvex (links the REAL row object). The row walks the closed outline pts[0..n-1], takes
// each XY edge direction (Z of every point is cleared before the subtract), normalises it with the fast reciprocal square root, adds
// acos(dot(edge, previous edge)) / (2 pi) for every corner (the loop corners, then the two closing corners), and returns
// (sum - 1)^2 < 0.0025: a convex outline turns exactly once, a concave one more. Fixtures: the STLport vector {start, finish,
// end_of_storage} of C3DVector, the 128-entry mantissa table GFastOneOverSqrtTable (0x013CE558). Expected values come from an
// independent double-precision model of the same rule on the XY coordinates (the Z values are noise and must not matter).
#include <stdio.h>
#include <math.h>
#pragma comment(linker, "/alternatename:@CallRow@4=?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z")
unsigned long GFastOneOverSqrtTable[128]={106,103,100,98,95,92,90,87,85,83,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,51,49,47,46,44,42,41,39,38,36,35,33,32,31,29,28,26,25,24,22,21,20,19,17,16,15,14,13,12,10,9,8,7,6,5,4,3,2,1,255,252,248,244,240,237,233,230,226,223,220,217,214,211,208,205,202,199,196,193,191,188,185,183,180,178,176,173,171,168,166,164,162,160,158,155,153,151,149,147,145,143,142,140,138,136,134,133,131,129,127,126,124,122,121,119,118,116,115,113,112,110,109,107};
struct V3 { float X, Y, Z; };
struct Vec3s { V3* start; V3* finish; V3* eos; };
extern "C" bool __fastcall CallRow(const Vec3s* pts);
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
static double Turn(const V3& a0, const V3& a1, const V3& b0, const V3& b1)   // unsigned angle between edges a and b, in turns
{
    double ax = a1.X - a0.X, ay = a1.Y - a0.Y, bx = b1.X - b0.X, by = b1.Y - b0.Y;
    double la = sqrt(ax * ax + ay * ay), lb = sqrt(bx * bx + by * by);
    double d = (ax * bx + ay * by) / (la * lb);
    if (d > 1) d = 1; if (d < -1) d = -1;
    return acos(d) / 6.283185307179586;
}
static bool Model(const V3* p, int n, double& sum)
{
    sum = 0;
    for (int i = 2; i < n; ++i) sum += Turn(p[i - 2], p[i - 1], p[i - 1], p[i]);
    sum += Turn(p[n - 2], p[n - 1], p[n - 1], p[0]);                         // closing corner at the last point
    sum += Turn(p[n - 1], p[0], p[0], p[1]);                                 // closing corner at the first point
    return (sum - 1) * (sum - 1) < 0.0025;
}
static void Run(V3* p, int n, bool want, int line)
{
    double sum; bool model = Model(p, n, sum);
    Check(model == want, line);                                             // the fixture is what the comment says it is
    Vec3s v = { p, p + n, p + n + 2 };
    bool got = CallRow(&v);
    Check(got == want, line);
}
int main()
{
    V3 square[4] = { { 0, 0, 0 }, { 4, 0, 0 }, { 4, 4, 0 }, { 0, 4, 0 } };
    Run(square, 4, true, __LINE__);                                         // convex, anticlockwise
    V3 squarecw[4] = { { 0, 0, 0 }, { 0, 4, 0 }, { 4, 4, 0 }, { 4, 0, 0 } };
    Run(squarecw, 4, true, __LINE__);                                       // convex, clockwise (unsigned angles)
    V3 tri[3] = { { 0, 0, 0 }, { 10, 1, 0 }, { 3, 7, 0 } };
    Run(tri, 3, true, __LINE__);
    V3 hex[6] = { { 2, 0, 0 }, { 1, 1.7f, 0 }, { -1, 1.7f, 0 }, { -2, 0, 0 }, { -1, -1.7f, 0 }, { 1, -1.7f, 0 } };
    Run(hex, 6, true, __LINE__);
    V3 arrow[5] = { { 0, 0, 0 }, { 6, 0, 0 }, { 6, 6, 0 }, { 3, 2, 0 }, { 0, 6, 0 } };
    Run(arrow, 5, false, __LINE__);                                         // concave notch: turns add to more than one
    V3 ell[6] = { { 0, 0, 0 }, { 6, 0, 0 }, { 6, 2, 0 }, { 2, 2, 0 }, { 2, 6, 0 }, { 0, 6, 0 } };
    Run(ell, 6, false, __LINE__);
    // Z noise: the same XY outlines with wild Z values; the row clears Z on every read, so only XY decides.
    V3 squarez[4] = { { 0, 0, 50 }, { 4, 0, -30 }, { 4, 4, 90 }, { 0, 4, 7 } };
    Run(squarez, 4, true, __LINE__);
    V3 arrowz[5] = { { 0, 0, 3 }, { 6, 0, -8 }, { 6, 6, 40 }, { 3, 2, -1 }, { 0, 6, 12 } };
    Run(arrowz, 5, false, __LINE__);
    if (g_fail) { printf("007602f0_TEST FAIL\n"); return 1; }
    printf("007602f0_TEST PASS\n");
    return 0;
}
