// Behaviour test for NParticleEngine::CParticle::GetInterpolatedLifetimerInSeconds (0x00AF85E0).
#include <cstdio>

static float g_period = 0.5f;
float GetParticleUpdatePeriod() { return g_period; }

namespace NParticleEngine {

struct CParticle {
    virtual void v00() {}
    unsigned char _state[0x6e];
    short         Lifetimer;
    unsigned char CreationTime;
    float GetInterpolatedLifetimerInSeconds(float fraction) const;
};

float CParticle::GetInterpolatedLifetimerInSeconds(float fraction) const
{
    return GetParticleUpdatePeriod() * (Lifetimer - CreationTime * (1.0f / 255.0f) + fraction);
}

} // namespace NParticleEngine

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    NParticleEngine::CParticle p;
    p.Lifetimer = 10;
    p.CreationTime = 51;                 // 0.2 of a tick
    float a = p.GetInterpolatedLifetimerInSeconds(0.3f);       // (10 - 0.2 + 0.3) * 0.5
    p.Lifetimer = -4;
    p.CreationTime = 255;
    g_period = 2.0f;
    float b = p.GetInterpolatedLifetimerInSeconds(1.0f);       // (-4 - 1 + 1) * 2
    if (!Near(a, 5.05f) || !Near(b, -8.0f)) {
        std::printf("00af85e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00af85e0_TEST PASS\n");
    return 0;
}
