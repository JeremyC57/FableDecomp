// Behaviour test for NUISystem::CParticleEffect::~CParticleEffect (0x00543CF0): an owned emitter is killed and the pointer
// cleared before the CChangingStateComponent base destructor runs; a null PHandle is skipped.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CParticleEmitter { public: int killed; void Kill() { ++killed; g_log[g_n++] = 20; } };
struct CChangingStateComponent { virtual ~CChangingStateComponent() { g_log[g_n++] = 1; } };
static CParticleEmitter** g_seen = 0;
struct CParticleEffect : CChangingStateComponent {
    CParticleEmitter* PHandle; unsigned long EffectIndex;
    CParticleEffect() : PHandle(0), EffectIndex(0) {}
    virtual ~CParticleEffect()
    {
        if (PHandle) {
            PHandle->Kill();
            PHandle = 0;
        }
        if (g_seen) *g_seen = PHandle;
    }
};
int main()
{
    CParticleEmitter e; e.killed = 0;
    CParticleEmitter* after = &e;
    g_seen = &after;
    { CParticleEffect fx; fx.PHandle = &e; }
    { CParticleEffect none; }
    if (e.killed != 1 || after != 0 || g_n != 3 || g_log[0] != 20 || g_log[1] != 1 || g_log[2] != 1) {
        std::printf("00543cf0_TEST FAIL\n"); return 1;
    }
    std::printf("00543cf0_TEST PASS\n");
    return 0;
}