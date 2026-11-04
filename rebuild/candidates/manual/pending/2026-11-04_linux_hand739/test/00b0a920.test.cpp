// Behaviour test for NParticleEngine::CPSCUpdateNormal::InterpolateTo (0x00B0A920): a standalone model of the blend -- the packed
// unsigned bit-fields (two 10-bit integers, the 1-bit Use* / fade flags) lerp as unsigned longs rounded to nearest and are truncated
// back into their field width, floats and C3DVectors lerp, the bool bit-fields, the two parameter keys and the decal emitter name
// take the target's value only when t > 0.5, LifeTimer is left alone.
#include <cstdio>
#include <cmath>
#include <string>
static float Lerp(float a, float b, float t) { return a * (1.0f - t) + b * t; }
static unsigned long LerpU(unsigned long a, unsigned long b, float t) { return (unsigned long)std::floor((1.0f - t) * a + b * t + 0.5f); }
struct Vec { float X, Y, Z; };
static Vec LerpV(const Vec& a, const Vec& b, float t) { Vec r = { Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t), Lerp(a.Z, b.Z, t) }; return r; }
struct Update {
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned int UseParticleLifeSecs : 1;
    unsigned int StayWithEmitter : 1;
    bool SetOrientationFromGame : 1;
    std::string DecalEmitterName;
    float ParticleLifeSecs;
    Vec RotationAxis;
    unsigned int AccelerationParam;
    long LifeTimer;
    void InterpolateTo(const Update& o, float t) {
        UseParticleLifeSecs = LerpU(UseParticleLifeSecs, o.UseParticleLifeSecs, t);
        ParticleLifeSecs = Lerp(ParticleLifeSecs, o.ParticleLifeSecs, t);
        AccelerationParam = t > 0.5f ? o.AccelerationParam : AccelerationParam;
        RotationAxis = LerpV(RotationAxis, o.RotationAxis, t);
        FadeInEndInteger = LerpU(FadeInEndInteger, o.FadeInEndInteger, t);
        FadeOutBeginInteger = LerpU(FadeOutBeginInteger, o.FadeOutBeginInteger, t);
        StayWithEmitter = LerpU(StayWithEmitter, o.StayWithEmitter, t);
        SetOrientationFromGame = t > 0.5f ? o.SetOrientationFromGame : SetOrientationFromGame;
        DecalEmitterName = t > 0.5f ? o.DecalEmitterName : DecalEmitterName;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Update a; a.FadeInEndInteger = 0; a.FadeOutBeginInteger = 1000; a.UseParticleLifeSecs = 1; a.StayWithEmitter = 0;
    a.SetOrientationFromGame = false; a.DecalEmitterName = "a"; a.ParticleLifeSecs = 5.0f; Vec ra = { 0, 0, 1 }; a.RotationAxis = ra;
    a.AccelerationParam = 7; a.LifeTimer = 3;
    Update b; b.FadeInEndInteger = 100; b.FadeOutBeginInteger = 600; b.UseParticleLifeSecs = 0; b.StayWithEmitter = 1;
    b.SetOrientationFromGame = true; b.DecalEmitterName = "b"; b.ParticleLifeSecs = 1.0f; Vec rb = { 1, 0, 0 }; b.RotationAxis = rb;
    b.AccelerationParam = 9; b.LifeTimer = 99;
    Update c = a; c.InterpolateTo(b, 0.25f);
    bool ok = c.FadeInEndInteger == 25 && c.FadeOutBeginInteger == 900 && c.UseParticleLifeSecs == 1 && c.StayWithEmitter == 0
        && !c.SetOrientationFromGame && c.DecalEmitterName == "a" && near(c.ParticleLifeSecs, 4.0f) && near(c.RotationAxis.X, 0.25f)
        && near(c.RotationAxis.Z, 0.75f) && c.AccelerationParam == 7 && c.LifeTimer == 3;
    Update d = a; d.InterpolateTo(b, 0.75f);
    ok = ok && d.FadeInEndInteger == 75 && d.FadeOutBeginInteger == 700 && d.UseParticleLifeSecs == 0 && d.StayWithEmitter == 1
        && d.SetOrientationFromGame && d.DecalEmitterName == "b" && d.AccelerationParam == 9 && d.LifeTimer == 3;
    printf(ok ? "00b0a920_TEST PASS\n" : "00b0a920_TEST FAIL\n");
    return ok ? 0 : 1;
}
