// Behaviour test for CTCDParticleEmitter::SetAlwaysUpdate(bool) @ 0x006E0590: with a live emitter its AlwaysUpdate bit
// follows the argument; without one the pending creation parameters take it only while waiting for the particle to
// load; the component's own AlwaysUpdate bit always follows; neighbouring bits are preserved.
#include <cstdio>
struct CParticleCreationParameters { bool InterpolatingParticle; bool ParticleCreationDisabled; bool UpdateParametersSet; bool AlwaysUpdateFlag; };
namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char _pad[0x44];
    bool PreWaterEffect : 1; bool WaterEffect : 1; bool ZBufferWriteable : 1; bool Visible : 1;
    bool AlwaysUpdate : 1;
    void SetAlwaysUpdate(bool alwaysUpdate) { AlwaysUpdate = alwaysUpdate; }
};
}
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    NParticleEngine::CParticleEmitter* GetPItem() const { return static_cast<NParticleEngine::CParticleEmitter*>(PData); }
};
template<class T> struct CCountedPointer { T* Object; long* PCount; T* operator->() const { return Object; } };
struct CTCDParticleEmitter {
    virtual void v00() { }
    unsigned char _base[0x8]; void* EmitterName; long ParticleTypeID;
    CBaseIntelligentPointer PEmitter; CBaseIntelligentPointer PAttachedToThing;
    unsigned long AttachmentFlags; void* AttachmentPosName; long AttachmentPosIndex; float FloatOffset;
    long DieOffTimer; long FramesLeftToWaitForParticleToCreate;
    CCountedPointer<CParticleCreationParameters> PCreationParameters;
    bool WaitingForParticleToLoad : 1; bool ForceParticleToLoad : 1; bool IndependantObject : 1; bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1; bool ResetEmitterOrientation : 1; bool AttachedToCamera : 1; bool Moving : 1;
    bool AttachedToThing : 1; bool AlwaysUpdate : 1; bool Asleep : 1;
    void SetAlwaysUpdate(bool alwaysUpdate);
};
void CTCDParticleEmitter::SetAlwaysUpdate(bool alwaysUpdate)
{
    if (PEmitter.GetPItem() != 0)
        PEmitter.GetPItem()->SetAlwaysUpdate(alwaysUpdate);
    else if (WaitingForParticleToLoad)
        PCreationParameters->AlwaysUpdateFlag = alwaysUpdate;
    AlwaysUpdate = alwaysUpdate;
}
int main()
{
    NParticleEngine::CParticleEmitter emitter;
    emitter.PreWaterEffect = true; emitter.WaterEffect = false; emitter.ZBufferWriteable = true; emitter.Visible = true; emitter.AlwaysUpdate = false;
    CParticleCreationParameters params; params.AlwaysUpdateFlag = false;
    CTCDParticleEmitter c; c.PCreationParameters.Object = &params;
    c.WaitingForParticleToLoad = true; c.AttachedToThing = true; c.AlwaysUpdate = false; c.Asleep = true;
    c.PEmitter.PData = &emitter;
    c.SetAlwaysUpdate(true);
    if (!emitter.AlwaysUpdate || !emitter.Visible || !emitter.PreWaterEffect || params.AlwaysUpdateFlag || !c.AlwaysUpdate
        || !c.AttachedToThing || !c.Asleep) { std::printf("006e0590_TEST FAIL\n"); return 1; }
    c.PEmitter.PData = 0;
    c.SetAlwaysUpdate(true);
    if (!params.AlwaysUpdateFlag || !c.AlwaysUpdate) { std::printf("006e0590_TEST FAIL\n"); return 1; }
    c.WaitingForParticleToLoad = false;
    c.SetAlwaysUpdate(false);
    if (!params.AlwaysUpdateFlag || c.AlwaysUpdate || !c.AttachedToThing) { std::printf("006e0590_TEST FAIL\n"); return 1; }
    std::printf("006e0590_TEST PASS\n");
    return 0;
}