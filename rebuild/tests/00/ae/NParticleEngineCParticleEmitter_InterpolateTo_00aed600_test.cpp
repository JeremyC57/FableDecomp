// Behaviour test for 0x00AED600 NParticleEngine::CParticleEmitter::InterpolateTo(const CParticleEmitter&, float): host model of the
// retail body (systems blended pairwise; one-bit flags lerped and rounded to nearest even; distances lerped; Priority and
// Emitter2DLayer switch to the target only when t > 0.5; the untouched flags keep their values).
#include <cstdio>
#include <cmath>

static long NearestEven(float v)                      // GFFloatToLongNear under the default x87 rounding (nearest, ties to even)
{
    double f = std::floor((double)v);
    double d = (double)v - f;
    long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1) != 0)) ++r;
    return r;
}
static unsigned short GFInterpolate(unsigned short a, unsigned short b, float t) { return (unsigned short)NearestEven((1.0f - t) * a + b * t); }
static unsigned char GFInterpolate(unsigned char a, unsigned char b, float t) { return (unsigned char)NearestEven((1.0f - t) * a + b * t); }
static float GFInterpolate(float a, float b, float t) { return (1.0f - t) * a + b * t; }
static long GFInterpolateAsFlags(long a, long b, float t) { return t > 0.5f ? b : a; }

struct SystemModel {
    int Calls; const SystemModel* Last; float LastT;
    void InterpolateTo(const SystemModel& other, float t) { ++Calls; Last = &other; LastT = t; }
};

struct EmitterModel {
    SystemModel* Systems[4]; int SystemCount;
    unsigned short Priority:3; unsigned short IsScreenDisplacement:1; unsigned short ReadZBuffer:1; unsigned short Dead:1;
    unsigned short ParticleCreationDisabled:1; unsigned short ContinuousEmitter:1; unsigned short Initialised:1;
    unsigned short WaitingToSleep:1; unsigned short Sleeping:1; unsigned short Emitter2D:1; unsigned short Emitter2DLayer:7;
    unsigned char PreWaterEffect:1; unsigned char WaterEffect:1; unsigned char ZBufferWriteable:1; unsigned char Visible:1;
    unsigned char AlwaysUpdate:1; unsigned char DieIfOffsceen:1; unsigned char OffscreenUpdate:1; unsigned char ClipEffectToWeatherMask:1;
    unsigned char EnableDithering:1; unsigned char CalcBoundingSphereOnceOnly:1; unsigned char BoundingSphereValid:1;
    float MaxSpawnDistance, MaxDrawDistance, FadeOutStart, FadeInStart, FadeInEnd;

    void InterpolateTo(const EmitterModel& other, float t)        // 0x00AED600
    {
        for (int i = 0; i < SystemCount; ++i)
            Systems[i]->InterpolateTo(*other.Systems[i], t);
        ContinuousEmitter = GFInterpolate(ContinuousEmitter, other.ContinuousEmitter, t);
        MaxSpawnDistance = GFInterpolate(MaxSpawnDistance, other.MaxSpawnDistance, t);
        MaxDrawDistance = GFInterpolate(MaxDrawDistance, other.MaxDrawDistance, t);
        FadeOutStart = GFInterpolate(FadeOutStart, other.FadeOutStart, t);
        FadeInEnd = GFInterpolate(FadeInEnd, other.FadeInEnd, t);
        FadeInStart = GFInterpolate(FadeInStart, other.FadeInStart, t);
        Priority = GFInterpolateAsFlags(Priority, other.Priority, t);
        IsScreenDisplacement = GFInterpolate(IsScreenDisplacement, other.IsScreenDisplacement, t);
        ReadZBuffer = GFInterpolate(ReadZBuffer, other.ReadZBuffer, t);
        Emitter2D = GFInterpolate(Emitter2D, other.Emitter2D, t);
        Emitter2DLayer = GFInterpolateAsFlags(Emitter2DLayer, other.Emitter2DLayer, t);
        PreWaterEffect = GFInterpolate(PreWaterEffect, other.PreWaterEffect, t);
        WaterEffect = GFInterpolate(WaterEffect, other.WaterEffect, t);
        ZBufferWriteable = GFInterpolate(ZBufferWriteable, other.ZBufferWriteable, t);
        DieIfOffsceen = GFInterpolate(DieIfOffsceen, other.DieIfOffsceen, t);
        OffscreenUpdate = GFInterpolate(OffscreenUpdate, other.OffscreenUpdate, t);
        ClipEffectToWeatherMask = GFInterpolate(ClipEffectToWeatherMask, other.ClipEffectToWeatherMask, t);
        EnableDithering = GFInterpolate(EnableDithering, other.EnableDithering, t);
        CalcBoundingSphereOnceOnly = GFInterpolate(CalcBoundingSphereOnceOnly, other.CalcBoundingSphereOnceOnly, t);
    }
};

static void Fill(EmitterModel& e, int bits, unsigned short priority, unsigned short layer, float d)
{
    e.SystemCount = 0;
    e.Priority = priority; e.Emitter2DLayer = layer;
    e.IsScreenDisplacement = bits & 1; e.ReadZBuffer = bits & 1; e.Dead = bits & 1; e.ParticleCreationDisabled = bits & 1;
    e.ContinuousEmitter = bits & 1; e.Initialised = bits & 1; e.WaitingToSleep = bits & 1; e.Sleeping = bits & 1; e.Emitter2D = bits & 1;
    e.PreWaterEffect = bits & 1; e.WaterEffect = bits & 1; e.ZBufferWriteable = bits & 1; e.Visible = bits & 1; e.AlwaysUpdate = bits & 1;
    e.DieIfOffsceen = bits & 1; e.OffscreenUpdate = bits & 1; e.ClipEffectToWeatherMask = bits & 1; e.EnableDithering = bits & 1;
    e.CalcBoundingSphereOnceOnly = bits & 1; e.BoundingSphereValid = bits & 1;
    e.MaxSpawnDistance = d; e.MaxDrawDistance = 2 * d; e.FadeOutStart = 3 * d; e.FadeInStart = 4 * d; e.FadeInEnd = 5 * d;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int fails = 0;
    SystemModel s0 = { 0, 0, 0.0f }, s1 = { 0, 0, 0.0f }, o0 = { 0, 0, 0.0f }, o1 = { 0, 0, 0.0f };
    EmitterModel a, b;
    // t = 0.25: lerped bits round to the current value (0.25 -> 0), selects keep the current values.
    Fill(a, 0, 2, 10, 10.0f); Fill(b, 1, 5, 99, 50.0f);
    a.Systems[0] = &s0; a.Systems[1] = &s1; a.SystemCount = 2; b.Systems[0] = &o0; b.Systems[1] = &o1; b.SystemCount = 2;
    a.InterpolateTo(b, 0.25f);
    if (s0.Calls != 1 || s0.Last != &o0 || s1.Calls != 1 || s1.Last != &o1 || !Near(s1.LastT, 0.25f)) ++fails;
    if (a.ContinuousEmitter != 0 || a.Emitter2D != 0 || a.CalcBoundingSphereOnceOnly != 0) ++fails;
    if (a.Priority != 2 || a.Emitter2DLayer != 10) ++fails;
    if (!Near(a.MaxSpawnDistance, 20.0f) || !Near(a.FadeInEnd, 100.0f) || !Near(a.FadeInStart, 80.0f)) ++fails;
    // t = 0.75: lerped bits become the target's, selects switch, untouched flags keep this emitter's values.
    Fill(a, 0, 2, 10, 10.0f); a.SystemCount = 0;
    a.InterpolateTo(b, 0.75f);
    if (a.ContinuousEmitter != 1 || a.IsScreenDisplacement != 1 || a.ReadZBuffer != 1 || a.Emitter2D != 1) ++fails;
    if (a.PreWaterEffect != 1 || a.WaterEffect != 1 || a.ZBufferWriteable != 1 || a.DieIfOffsceen != 1 || a.OffscreenUpdate != 1) ++fails;
    if (a.ClipEffectToWeatherMask != 1 || a.EnableDithering != 1 || a.CalcBoundingSphereOnceOnly != 1) ++fails;
    if (a.Dead != 0 || a.Initialised != 0 || a.Sleeping != 0 || a.Visible != 0 || a.AlwaysUpdate != 0 || a.BoundingSphereValid != 0) ++fails;
    if (a.Priority != 5 || a.Emitter2DLayer != 99) ++fails;
    if (!Near(a.MaxDrawDistance, 2 * 40.0f) || !Near(a.FadeOutStart, 3 * 40.0f)) ++fails;
    // t = 0.5 exactly: the select keeps the current value (strict >), the lerp 0.5 rounds to even (0).
    Fill(a, 0, 3, 7, 0.0f); a.SystemCount = 0;
    a.InterpolateTo(b, 0.5f);
    if (a.Priority != 3 || a.Emitter2DLayer != 7 || a.ContinuousEmitter != 0 || a.EnableDithering != 0) ++fails;
    // 1 -> 0 at 0.5 rounds to even (0) as well.
    Fill(a, 1, 3, 7, 0.0f); Fill(b, 0, 1, 1, 0.0f); a.SystemCount = 0;
    a.InterpolateTo(b, 0.5f);
    if (a.ContinuousEmitter != 0 || a.Dead != 1) ++fails;
    if (fails) { std::printf("00aed600_TEST FAIL\n"); return 1; }
    std::printf("00aed600_TEST PASS\n");
    return 0;
}