// Behaviour test for 0x00B0F8E0 NParticleEngine::CPSCUpdateNormal::GetParticleLifeSecs
#include <cstdio>
const float Zero_0122dedc = 0.0f;
struct CPSCUpdateNormal {
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;
    unsigned long _flags26 : 6;
    float SystemLifeSecs;
    float ParticleLifeSecs;
    bool GetParticleLifeSecs(float& lifeSecs)
    {
        lifeSecs = UseParticleLifeSecs ? ParticleLifeSecs
                 : (UseSystemLifeSecs ? SystemLifeSecs : Zero_0122dedc);
        return true;
    }
};
int main()
{
    CPSCUpdateNormal c;
    c.FadeInEndInteger = 0; c.FadeOutBeginInteger = 0; c.UseRandomRotationAxis = 0;
    c.UseRandomInitialRotation = 0; c.UseAccelerationParam = 0; c.StayWithEmitter = 0; c._flags26 = 0;
    c.SystemLifeSecs = 2.5f; c.ParticleLifeSecs = 1.5f;
    float out = 9.0f;
    int ok = 1;
    c.UseParticleLifeSecs = 1; c.UseSystemLifeSecs = 1;
    if (!c.GetParticleLifeSecs(out) || out != 1.5f) ok = 0;
    c.UseParticleLifeSecs = 0;
    if (!c.GetParticleLifeSecs(out) || out != 2.5f) ok = 0;
    c.UseSystemLifeSecs = 0;
    if (!c.GetParticleLifeSecs(out) || out != 0.0f) ok = 0;
    if (!ok) { std::printf("00b0f8e0_TEST FAIL\n"); return 1; }
    std::printf("00b0f8e0_TEST PASS\n");
    return 0;
}