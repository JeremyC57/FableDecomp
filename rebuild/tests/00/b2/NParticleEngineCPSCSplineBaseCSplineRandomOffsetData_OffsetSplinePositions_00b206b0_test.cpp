// Behaviour test for 0x00B206B0 CSplineRandomOffsetData::OffsetSplinePositions: a copy of the model with plain arrays and a
// deterministic GFFloatRandom stand-in for the test only (returns a fixed fraction of the range).
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static float g_frac = 0.75f;
static float TestRandom(float range, unsigned long* seed) { ++*seed; return range * g_frac; }
static float Sqr(float v) { return v * v; }
static void Offset(V* thisPts, const V* initPts, const V* limits, const V* speeds, long count, unsigned long* seed)
{
    for (long i = 0; i < count; ++i) {
        const V& limit = limits[i];
        if (!(limit.X * limit.X + limit.Y * limit.Y + limit.Z * limit.Z > 1e-4f * 1e-4f)) continue;
        V offset = { thisPts[i].X - initPts[i].X, thisPts[i].Y - initPts[i].Y, thisPts[i].Z - initPts[i].Z };
        const V& speed = speeds[i];
        V m;
        m.X = speed.X != 0.0f ? TestRandom(speed.X * 2.0f, seed) - speed.X : 0.0f;
        m.Y = speed.Y != 0.0f ? TestRandom(speed.Y * 2.0f, seed) - speed.Y : 0.0f;
        m.Z = speed.Z != 0.0f ? TestRandom(speed.Z * 2.0f, seed) - speed.Z : 0.0f;
        m.X += offset.X; m.Y += offset.Y; m.Z += offset.Z;
        float lx = Sqr(limit.X), ly = Sqr(limit.Y), lz = Sqr(limit.Z);
        if (lx < 0.0001f) { m.X = 0.0f; float d = (m.Y * m.Y) / ly; d += (m.Z * m.Z) / lz; if (d > 1.0f) { float s = 1.0f / d; m.Y *= s; m.Z *= s; } }
        else if (ly < 0.0001f) { m.Y = 0.0f; float d = (m.X * m.X) / lx; d += (m.Z * m.Z) / lz; if (d > 1.0f) { float s = 1.0f / d; m.X *= s; m.Z *= s; } }
        else if (lz < 0.0001f) { m.Z = 0.0f; float d = (m.X * m.X) / lx; d += (m.Y * m.Y) / ly; if (d > 1.0f) { float s = 1.0f / d; m.X *= s; m.Y *= s; } }
        else { float d = (m.X * m.X) / lx; d += (m.Y * m.Y) / ly; d += (m.Z * m.Z) / lz; if (d > 1.0f) { float s = 1.0f / d; m.X *= s; m.Y *= s; m.Z *= s; } }
        thisPts[i].X = m.X + initPts[i].X; thisPts[i].Y = m.Y + initPts[i].Y; thisPts[i].Z = m.Z + initPts[i].Z;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    V init[3]   = { {0,0,0}, {10,10,10}, {5,5,5} };
    V cur[3]    = { {0.1f,0,0}, {10,10,10}, {7,5,5} };
    V limits[3] = { {1,1,1}, {0,0,0}, {0,2,2} };
    V speeds[3] = { {0.2f,0,0}, {1,1,1}, {1,1,1} };
    unsigned long seed = 42;
    Offset(cur, init, limits, speeds, 3, &seed);
    // point 0: offset.X 0.1 + (0.4*0.75 - 0.2 = 0.1) = 0.2, inside the unit ellipsoid
    if (!Near(cur[0].X, 0.2f) || !Near(cur[0].Y, 0.0f) || !Near(cur[0].Z, 0.0f)) { std::printf("00b206b0_TEST FAIL\n"); return 1; }
    // point 1: zero limit -> untouched, no random draw
    if (!Near(cur[1].X, 10.0f) || !Near(cur[1].Y, 10.0f) || !Near(cur[1].Z, 10.0f)) { std::printf("00b206b0_TEST FAIL\n"); return 1; }
    // point 2: X pinned to initial; Y,Z offsets 0.5 each, d = 0.25/4*2 = 0.125 -> kept
    if (!Near(cur[2].X, 5.0f) || !Near(cur[2].Y, 5.5f) || !Near(cur[2].Z, 5.5f)) { std::printf("00b206b0_TEST FAIL\n"); return 1; }
    if (seed != 42 + 1 + 3) { std::printf("00b206b0_TEST FAIL\n"); return 1; }
    // outside the ellipsoid: offset 3 on a limit of 1 in all axes -> d = 9+0+0 -> scaled by 1/9
    V i2[1] = { {0,0,0} }, c2[1] = { {3,0,0} }, l2[1] = { {1,1,1} }, s2[1] = { {0,0,0} };
    Offset(c2, i2, l2, s2, 1, &seed);
    if (!Near(c2[0].X, 3.0f / 9.0f)) { std::printf("00b206b0_TEST FAIL\n"); return 1; }
    std::printf("00b206b0_TEST PASS\n");
    return 0;
}