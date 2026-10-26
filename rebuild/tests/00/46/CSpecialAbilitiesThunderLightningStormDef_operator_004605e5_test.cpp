// behaviour test for 0x004605E5 CSpecialAbilitiesThunderLightningStormDef::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
    long NumBeams;
    float StartRadius;
    float EndRadius;
    float SkyBeamStartTime;
    float MainBeamsStartTime;
    float BeamRotationStartSpeed;
    float BeamRotationEndSpeed;
    float TotalLifetime;
    float Damage;
    long InitSound;
    long LoopingSound;
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        NumBeams = o.NumBeams;
        StartRadius = o.StartRadius;
        EndRadius = o.EndRadius;
        SkyBeamStartTime = o.SkyBeamStartTime;
        MainBeamsStartTime = o.MainBeamsStartTime;
        BeamRotationStartSpeed = o.BeamRotationStartSpeed;
        BeamRotationEndSpeed = o.BeamRotationEndSpeed;
        TotalLifetime = o.TotalLifetime;
        Damage = o.Damage;
        InitSound = o.InitSound;
        LoopingSound = o.LoopingSound;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.NumBeams = 100L; d.NumBeams = 0L;
    s.StartRadius = 2.25f; d.StartRadius = 0.0f;
    s.EndRadius = 3.25f; d.EndRadius = 0.0f;
    s.SkyBeamStartTime = 4.25f; d.SkyBeamStartTime = 0.0f;
    s.MainBeamsStartTime = 5.25f; d.MainBeamsStartTime = 0.0f;
    s.BeamRotationStartSpeed = 6.25f; d.BeamRotationStartSpeed = 0.0f;
    s.BeamRotationEndSpeed = 7.25f; d.BeamRotationEndSpeed = 0.0f;
    s.TotalLifetime = 8.25f; d.TotalLifetime = 0.0f;
    s.Damage = 9.25f; d.Damage = 0.0f;
    s.InitSound = 109L; d.InitSound = 0L;
    s.LoopingSound = 110L; d.LoopingSound = 0L;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.NumBeams != s.NumBeams) fail |= 4;
    if (d.StartRadius != s.StartRadius) fail |= 4;
    if (d.EndRadius != s.EndRadius) fail |= 4;
    if (d.SkyBeamStartTime != s.SkyBeamStartTime) fail |= 4;
    if (d.MainBeamsStartTime != s.MainBeamsStartTime) fail |= 4;
    if (d.BeamRotationStartSpeed != s.BeamRotationStartSpeed) fail |= 4;
    if (d.BeamRotationEndSpeed != s.BeamRotationEndSpeed) fail |= 4;
    if (d.TotalLifetime != s.TotalLifetime) fail |= 4;
    if (d.Damage != s.Damage) fail |= 4;
    if (d.InitSound != s.InitSound) fail |= 4;
    if (d.LoopingSound != s.LoopingSound) fail |= 4;
    if (fail) { std::printf("004605e5_TEST FAIL %d\n", fail); return 1; }
    std::printf("004605e5_TEST PASS\n");
    return 0;
}