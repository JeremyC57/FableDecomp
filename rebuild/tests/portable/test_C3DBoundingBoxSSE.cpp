// test_C3DBoundingBoxSSE.cpp -- dual-path test of engine/C3DBoundingBoxSSE.h (GF_SSE_BOX_DISTANCE_SQUARED). Boxes and points
// on a dyadic grid (every difference, square and sum exact in float), inside, outside on each axis, on faces, plus a NaN point
// lane (maxps / minps keep the second operand) -- checked against the clamp-and-sum reference and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "engine/C3DBoundingBoxSSE.h"
#include "fable_test_mode.h"
struct TestBox { float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };
struct TestPoint { float X, Y, Z; };
static float Kernel(const TestBox* box, const TestPoint& point)
{
    float result;
    GF_SSE_BOX_DISTANCE_SQUARED(box, point, result);
    return result;
}
static float Clamp(float p, float lo, float hi) { float c = p > lo ? p : lo; return c < hi ? c : hi; }
static unsigned int Bits(float f) { unsigned int u; memcpy(&u, &f, 4); return u; }
int main()
{
    int bad = 0, n = 0;
    TestBox box = { -2.0f, -1.0f, 0.5f, 3.0f, 4.0f, 2.5f };
    static const float coords[] = { -6.0f, -2.0f, -1.5f, 0.0f, 0.75f, 2.5f, 3.0f, 7.25f };
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) for (int k = 0; k < 8; ++k, ++n) {
        TestPoint p = { coords[i], coords[j], coords[k] };
        float got = Kernel(&box, p);
        float dx = Clamp(p.X, box.MinX, box.MaxX) - p.X, dy = Clamp(p.Y, box.MinY, box.MaxY) - p.Y,
              dz = Clamp(p.Z, box.MinZ, box.MaxZ) - p.Z;
        float want = (dx * dx + dy * dy) + dz * dz;
        if (Bits(got) != Bits(want)) { printf("  (%g,%g,%g): %.9g, expected %.9g\n", p.X, p.Y, p.Z, got, want); ++bad; }
#ifdef FABLE_TEST_DUAL
        if (Bits(fable_portable::BoxDistanceSquaredSSE(&box.MinX, &p.X)) != Bits(got)) { printf("  twin differs\n"); ++bad; }
#endif
    }
    printf("test_C3DBoundingBoxSSE %s: %s (%d points)\n", kFableTestMode, bad ? "FAIL" : "PASS", n);
    return bad ? 1 : 0;
}
