// Behaviour test for CTCDParticleEmitter::SetEmitterUpdateParameters(const CParticleUpdateParameters&) @ 0x006E11A0: a live
// emitter receives the parameters; otherwise, while waiting for the particle, they are copied into the pending creation
// parameters and UpdateParametersSet is raised; with neither nothing changes.
#include <cstdio>
static int g_emitterSets = 0, g_assigns = 0;
static const void* g_emitterArg = 0;
namespace NParticleEngine {
template<class A, class B> struct CParticleUpdateParameters {
    long Value;
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other) { ++g_assigns; Value = other.Value; return *this; }
};
struct CParticleEmitter {
    void SetUpdateParameters(const CParticleUpdateParameters<unsigned long, unsigned long>& parameters) { ++g_emitterSets; g_emitterArg = &parameters; }
};
}
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> CEmitterUpdateParameters;
struct CParticleCreationParameters {
    bool InterpolatingParticle; bool ParticleCreationDisabled; bool UpdateParametersSet; bool AlwaysUpdateFlag;
    unsigned char _body[0x28 - 0x04];
    CEmitterUpdateParameters UpdateParameters;
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    NParticleEngine::CParticleEmitter* GetPItem() const { return static_cast<NParticleEngine::CParticleEmitter*>(PData); }
};
template<class T> struct CCountedPointer { T* Object; long* PCount; T* operator->() const { return Object; } };
struct CTCDParticleEmitter {
    virtual void v00() { }
    unsigned char _base[0x8]; void* EmitterName; long ParticleTypeID;
    CBaseIntelligentPointer PEmitter;
    unsigned char _body[0x3c - 0x1c];
    CCountedPointer<CParticleCreationParameters> PCreationParameters;
    bool WaitingForParticleToLoad : 1; bool ForceParticleToLoad : 1; bool IndependantObject : 1; bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1; bool ResetEmitterOrientation : 1; bool AttachedToCamera : 1; bool Moving : 1;
    void SetEmitterUpdateParameters(const CEmitterUpdateParameters& parameters);
};
void CTCDParticleEmitter::SetEmitterUpdateParameters(const CEmitterUpdateParameters& parameters)
{
    if (PEmitter.GetPItem() != 0) {
        PEmitter.GetPItem()->SetUpdateParameters(parameters);
    } else if (WaitingForParticleToLoad) {
        PCreationParameters->UpdateParameters = parameters;
        PCreationParameters->UpdateParametersSet = true;
    }
}
int main()
{
    NParticleEngine::CParticleEmitter emitter; CParticleCreationParameters pending; pending.UpdateParametersSet = false; pending.UpdateParameters.Value = 0;
    CEmitterUpdateParameters params; params.Value = 42;
    CTCDParticleEmitter c; c.PCreationParameters.Object = &pending; c.WaitingForParticleToLoad = true;
    c.PEmitter.PData = &emitter; c.SetEmitterUpdateParameters(params);
    if (g_emitterSets != 1 || g_emitterArg != &params || g_assigns != 0 || pending.UpdateParametersSet) { std::printf("006e11a0_TEST FAIL\n"); return 1; }
    c.PEmitter.PData = 0; c.WaitingForParticleToLoad = false; c.SetEmitterUpdateParameters(params);
    if (g_emitterSets != 1 || g_assigns != 0 || pending.UpdateParametersSet) { std::printf("006e11a0_TEST FAIL\n"); return 1; }
    c.WaitingForParticleToLoad = true; c.SetEmitterUpdateParameters(params);
    if (g_assigns != 1 || pending.UpdateParameters.Value != 42 || !pending.UpdateParametersSet) { std::printf("006e11a0_TEST FAIL\n"); return 1; }
    std::printf("006e11a0_TEST PASS\n");
    return 0;
}