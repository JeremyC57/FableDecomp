// Behaviour test for NBBBMaths::IsPosInFOVWithWeight (0x00A4B670): links the REAL row object. The test defines the two tables the
// row reads (GFastCosTable, 1025 cosines of k/1024 turns, 0x013CD550; GFastOneOverSqrtTable, the 128 reciprocal-square-root mantissa
// seeds, 0x013CE558, built here from the exponent / mantissa split the seed formula uses) and checks against exact double math:
// cos(half the view angle, in turns), the normalised direction to the test position, its dot with the view direction, true with
// weight (dot - cos) / (1 - cos) inside the cone, false with weight 0 outside; the inputs are never written.
#include <stdio.h>
#include <math.h>
#include <string.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
namespace NBBBMaths { bool __fastcall IsPosInFOVWithWeight(const C3DVector& origin_pos, const C3DVector& view_direction, float view_angle, const C3DVector& test_pos, float& weight); }
float GFastCosTable[1025];
unsigned long GFastOneOverSqrtTable[128];
static const double TWO_PI = 6.283185307179586;

static void build_tables()
{
    for (int k = 0; k <= 1024; ++k) GFastCosTable[k] = (float)cos(TWO_PI * k / 1024.0);
    // index = (exponent bit 0) << 6 | top 6 mantissa bits; seed = ((0x17C - exponent) >> 1) << 23 | entry << 15
    for (int i = 0; i < 128; ++i) {
        unsigned long e = 126 + ((i >> 6) & 1);            // exponent with the right parity
        unsigned long bits = (e << 23) | ((unsigned long)(i & 63) << 17) | (1ul << 16);   // middle of the bucket
        float x; memcpy(&x, &bits, 4);
        double y = 1.0 / sqrt((double)x);
        unsigned long se = ((0x17c - e) >> 1);
        double m = (y / pow(2.0, (double)se - 127.0) - 1.0) * 256.0;
        long mi = (long)floor(m + 0.5); if (mi < 0) mi = 0; if (mi > 255) mi = 255;
        GFastOneOverSqrtTable[i] = (unsigned long)mi;
    }
}

static void run(int c, C3DVector o, C3DVector d, float angle, C3DVector t, bool want_in)
{
    C3DVector o0 = o, d0 = d, t0 = t; float w = -7.0f;
    bool r = NBBBMaths::IsPosInFOVWithWeight(o, d, angle, t, w);
    double tx = (double)t.X - o.X, ty = (double)t.Y - o.Y, tz = (double)t.Z - o.Z, m = sqrt(tx * tx + ty * ty + tz * tz);
    double dot = (tx * d.X + ty * d.Y + tz * d.Z) / m, cs = cos(TWO_PI * angle * 0.5);
    bool in = dot > cs;
    check(in == want_in, "fixture on the intended side of the cone", c);
    check(r == in, "result", c);
    if (in) check(fabs(w - (dot - cs) / (1.0 - cs)) < 2e-3, "weight = (dot - cos) / (1 - cos)", c);
    else check(w == 0.0f, "weight 0 outside", c);
    check(memcmp(&o, &o0, 12) == 0 && memcmp(&d, &d0, 12) == 0 && memcmp(&t, &t0, 12) == 0, "inputs untouched", c);
}

int main()
{
    build_tables();
    run(1, C3DVector(0, 0, 0), C3DVector(1, 0, 0), 0.25f, C3DVector(10, 1, 0), true);          // 90-degree cone, near the axis
    run(2, C3DVector(0, 0, 0), C3DVector(1, 0, 0), 0.25f, C3DVector(1, 3, 0), false);          // outside
    run(3, C3DVector(5, -2, 1), C3DVector(0, 0.6f, 0.8f), 0.125f, C3DVector(5.5f, 4, 9), true); // 45-degree cone, offset origin
    run(4, C3DVector(5, -2, 1), C3DVector(0, 0.6f, 0.8f), 0.125f, C3DVector(9, -1, 2), false);
    run(5, C3DVector(-3, 7, 2), C3DVector(0, -1, 0), 0.5f, C3DVector(-1, 0, 6), true);         // 180-degree cone: any forward point
    run(6, C3DVector(-3, 7, 2), C3DVector(0, -1, 0), 0.5f, C3DVector(-3, 9, 2), false);        // straight behind
    run(7, C3DVector(1, 1, 1), C3DVector(0.267261f, 0.534522f, 0.801784f), 0.3f, C3DVector(4, 7, 11), true);
    run(8, C3DVector(100, 200, 0), C3DVector(-1, 0, 0), 0.1f, C3DVector(70, 205, 2), true);    // narrow cone, far target
    if (failures) { printf("00a4b670_TEST FAIL (%d)\n", failures); return 1; }
    printf("00a4b670_TEST PASS\n");
    return 0;
}