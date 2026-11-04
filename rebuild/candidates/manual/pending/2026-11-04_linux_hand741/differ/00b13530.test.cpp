// Behaviour test for NParticleEngine::CPSCEmitterGeneric::DatabaseSerialise (0x00B13530): a standalone model of the quantised-field
// round trip (decode unit = Field / max, value = RoundToDecimals(unit * range - offset, 2); encode Field = round-half-even((
// RoundToDecimals(value, 2) + offset) / range * max)), the legacy spline point count clamp (at most 20) and the "at least two control
// points" padding with zero vectors.
#include <cstdio>
#include <cmath>
#include <vector>
static long RoundNear(float value)                     // the x87 default rounding of the bare fistp: round half to even
{
    float floorValue = std::floor(value);
    float fraction = value - floorValue;
    long result = (long)floorValue;
    if (fraction > 0.5f || (fraction == 0.5f && (result & 1) != 0))
        ++result;
    return result;
}
static float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    return RoundNear(value * scale) / scale;
}
static float Decode(unsigned long field, float max, float range, float offset)
{
    float unit = (float)field * (1.0f / max);
    return RoundToDecimals(unit * range - offset, 2);
}
static unsigned long Encode(float value, float max, float range, float offset)
{
    float scaled = (RoundToDecimals(value, 2) + offset) * (1.0f / range);
    return (unsigned long)RoundNear(scaled * max);
}
struct Vec { float X, Y, Z; };
static std::vector<Vec> LoadLegacyPoints(long count)
{
    if (count > 20)
        count = 20;
    std::vector<Vec> points(count);
    for (long i = 0; i < count; ++i) {
        points[i].X = (float)i; points[i].Y = 0.0f; points[i].Z = 0.0f;
    }
    Vec zero = { 0.0f, 0.0f, 0.0f };
    while (points.size() < 2)
        points.push_back(zero);
    return points;
}
int main()
{
    bool ok = Decode(1023, 1023.0f, 10.0f, 0.0f) == 10.0f;                 // EmitterSize: full scale
    ok = ok && Encode(10.0f, 1023.0f, 10.0f, 0.0f) == 1023;
    ok = ok && Decode(0, 1023.0f, 10.0f, 5.0f) == -5.0f;                      // RadialBias: -5 .. 5
    ok = ok && Encode(-5.0f, 1023.0f, 10.0f, 5.0f) == 0;
    ok = ok && Encode(0.0f, 1023.0f, 10.0f, 5.0f) == 512;                     // 511.5 rounds half to even
    ok = ok && Decode(Encode(1.0f, 2047.0f, 20.0f, 10.0f), 2047.0f, 20.0f, 10.0f) == 1.0f;   // NonUniformScaling default
    ok = ok && Decode(Encode(3.0f, 16383.0f, 100.0f, 0.0f), 16383.0f, 100.0f, 0.0f) == 3.0f; // ParticlesPerSecond default
    ok = ok && LoadLegacyPoints(25).size() == 20 && LoadLegacyPoints(0).size() == 2 && LoadLegacyPoints(1)[1].X == 0.0f;
    printf(ok ? "00b13530_TEST PASS\n" : "00b13530_TEST FAIL\n");
    return ok ? 0 : 1;
}
