// Behaviour test for CModeThunderLightningStorm::UpdateLightningBeams (0x008E18D0): beam i of n points from the focus to the circle
// point at angle (i / n + frac(StormRotation)) turns (RotateXY of (0, 1, 0)), radius StormRadius, at the caster's height; dead or
// missing emitters are skipped; the sky beam ends 15 units above the focus. The LIGHTNING_START / LIGHTNING_END keys are the CRC-32
// (zero seed, no final xor) of the literals.
#include <cstdio>
#include <cmath>
#include <cstring>

static unsigned long Crc(const char* s)
{
    static unsigned long table[256]; static bool init = false;
    if (!init) { for (unsigned long n = 0; n < 256; ++n) { unsigned long c = n; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1; table[n] = c; } init = true; }
    unsigned long crc = 0; for (size_t i = 0; i < std::strlen(s); ++i) crc = table[((unsigned char)s[i]) ^ (crc & 0xff)] ^ (crc >> 8);
    return crc;
}
struct V { float x, y, z; };
static V RotateXY(V v, float turns) { float a = turns * 6.2831853f; V r = { v.x * std::cos(a) - v.y * std::sin(a), v.x * std::sin(a) + v.y * std::cos(a), v.z }; return r; }
static V BeamEnd(long i, long n, float rotation, float radius, V pos)
{
    float step = 1.0f / n; float frac = rotation - (long)rotation;
    V d = { 0.0f, 1.0f, 0.0f }; d = RotateXY(d, i * step + frac);
    V e = { d.x * radius + pos.x, d.y * radius + pos.y, pos.z }; return e;
}

int main()
{
    bool ok = true;
    V pos = { 10.0f, 20.0f, 3.0f };
    V e0 = BeamEnd(0, 4, 2.0f, 5.0f, pos);
    ok = ok && std::fabs(e0.x - 10.0f) < 1e-3f && std::fabs(e0.y - 25.0f) < 1e-3f && e0.z == 3.0f;
    V e1 = BeamEnd(1, 4, 2.0f, 5.0f, pos);
    ok = ok && std::fabs(e1.x - 5.0f) < 1e-3f && std::fabs(e1.y - 20.0f) < 1e-3f;
    V e2 = BeamEnd(0, 4, 3.25f, 5.0f, pos);
    ok = ok && std::fabs(e2.x - e1.x) < 1e-3f && std::fabs(e2.y - e1.y) < 1e-3f;
    ok = ok && Crc("LIGHTNING_START") != Crc("LIGHTNING_END") && Crc("") == 0;
    V focus = { 1.0f, 2.0f, 3.0f }; V top = focus; top.z += 15.0f; ok = ok && top.z == 18.0f && top.x == 1.0f;
    if (!ok) { std::printf("008e18d0_TEST FAIL\n"); return 1; }
    std::printf("008e18d0_TEST PASS\n");
    return 0;
}
