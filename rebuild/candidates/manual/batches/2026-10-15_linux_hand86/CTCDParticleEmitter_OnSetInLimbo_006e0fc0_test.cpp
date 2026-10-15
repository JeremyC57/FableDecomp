// Behaviour test for CTCDParticleEmitter::OnSetInLimbo(bool) @ 0x006E0FC0: when creation should be disabled and is not,
// disable it and clear the live emitter's particles and decals (null engine); when it should be enabled and is
// disabled, re-enable it; otherwise nothing.
#include <cstdio>
struct CIEngine;
static int g_particles = 0, g_decals = 0, g_sets = 0;
static bool g_should = false;
namespace NParticleEngine {
struct CParticleEmitter {
    void ClearAllParticles(CIEngine* engine) { if (engine == 0) ++g_particles; }
    void ClearAllDecals(CIEngine* engine) { if (engine == 0) ++g_decals; }
};
}
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    NParticleEngine::CParticleEmitter* GetPItem() const { return static_cast<NParticleEngine::CParticleEmitter*>(PData); }
};
struct CTCDParticleEmitter {
    virtual void v00() { }
    unsigned char _base[0x8]; void* EmitterName; long ParticleTypeID;
    CBaseIntelligentPointer PEmitter;
    unsigned char _body[0x44 - 0x1c];
    bool WaitingForParticleToLoad : 1; bool ForceParticleToLoad : 1; bool IndependantObject : 1; bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1; bool ResetEmitterOrientation : 1; bool AttachedToCamera : 1; bool Moving : 1;
    bool IsParticleCreationDisabled() const { return g_should; }
    void SetParticleCreationDisabled(bool disabled) { ++g_sets; ParticleCreationDisabled = disabled; }
    void OnSetInLimbo(bool inLimbo);
};
void CTCDParticleEmitter::OnSetInLimbo(bool inLimbo)
{
    if (IsParticleCreationDisabled()) {
        if (!ParticleCreationDisabled) {
            SetParticleCreationDisabled(true);
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            if (emitter != 0) {
                emitter->ClearAllParticles(0);
                emitter->ClearAllDecals(0);
            }
        }
    } else if (ParticleCreationDisabled) {
        SetParticleCreationDisabled(false);
    }
}
int main()
{
    NParticleEngine::CParticleEmitter emitter;
    CTCDParticleEmitter c; c.PEmitter.PData = &emitter; c.ParticleCreationDisabled = false;
    g_should = true; c.OnSetInLimbo(true);
    if (!c.ParticleCreationDisabled || g_sets != 1 || g_particles != 1 || g_decals != 1) { std::printf("006e0fc0_TEST FAIL\n"); return 1; }
    c.OnSetInLimbo(true);                                    // already disabled: nothing
    if (g_sets != 1 || g_particles != 1) { std::printf("006e0fc0_TEST FAIL\n"); return 1; }
    g_should = false; c.OnSetInLimbo(false);                 // re-enable
    if (c.ParticleCreationDisabled || g_sets != 2 || g_particles != 1) { std::printf("006e0fc0_TEST FAIL\n"); return 1; }
    c.OnSetInLimbo(false);                                   // already enabled: nothing
    if (g_sets != 2) { std::printf("006e0fc0_TEST FAIL\n"); return 1; }
    c.PEmitter.PData = 0; g_should = true; c.OnSetInLimbo(true);   // no emitter: only the switch
    if (!c.ParticleCreationDisabled || g_sets != 3 || g_particles != 1 || g_decals != 1) { std::printf("006e0fc0_TEST FAIL\n"); return 1; }
    std::printf("006e0fc0_TEST PASS\n");
    return 0;
}
