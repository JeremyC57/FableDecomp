// Test: CThingObject::GetAlpha -- 0xFF without property bit 13; with it, (1 - 4d) * 40 for the unsigned heading difference d (in
//   turns) between the facing and the sun, 0 beyond a quarter turn.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static float AngleXY(const V& v) { return Wrap((float)std::atan2(v.X, v.Y) * 0.15915494f); }
static float UnsignedDifference(float a, float b) { float d = (float)std::fabs(Wrap(a) - Wrap(b)); if (d > 0.5f) d = 1.0f - d; return d; }
static unsigned char GetAlpha(unsigned long property, const V& facing, const V& sun)
{
    if (property & 0x2000) {
        float d = UnsignedDifference(AngleXY(sun), AngleXY(facing));
        if (d > 0.25f) return 0;
        return (unsigned char)((1.0f - d * 4.0f) * 40.0f);
    }
    return 0xff;
}
int main()
{
    V north = { 0, 1, 0 }, east = { 1, 0, 0 }, south = { 0, -1, 0 }, nne = { 0.41421356f, 1, 0 };
    bool ok = GetAlpha(0, north, north) == 0xff && GetAlpha(0xffffdfff, north, south) == 0xff;
    ok = ok && GetAlpha(0x2000, north, north) == 40;           // aligned
    ok = ok && GetAlpha(0x2000, north, south) == 0;            // half a turn
    ok = ok && GetAlpha(0x2000, north, east) == 0;             // exactly a quarter turn: (1 - 1) * 40
    unsigned char a = GetAlpha(0x2000, nne, north);         // 1/16 turn: (1 - 0.25) * 40 = 30 (29 after float truncation)
    ok = ok && (a == 30 || a == 29) && GetAlpha(0x2000, north, nne) == a;   // symmetric
    std::printf(ok ? "00702e00_TEST PASS\n" : "00702e00_TEST FAIL\n");
    return ok ? 0 : 1;
}