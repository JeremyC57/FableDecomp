// Behaviour test for CTCDParticleEmitter::EnterTimelineEnd() @ 0x006E05F0: a live emitter enters its timeline end;
// without one the parent thing is killed.
#include <cstdio>
static int g_kills = 0, g_ends = 0;
class CThing { public: void Kill(bool immediately) { if (immediately) ++g_kills; } };
namespace NParticleEngine { class CParticleEmitter { public: void EnterTimelineEnd() { ++g_ends; } }; }
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    NParticleEngine::CParticleEmitter* GetPItem() const { return static_cast<NParticleEngine::CParticleEmitter*>(PData); }
};
class CTCDParticleEmitter { public:
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; void* EmitterName; long ParticleTypeID;
    CBaseIntelligentPointer PEmitter;
    void EnterTimelineEnd();
};
void CTCDParticleEmitter::EnterTimelineEnd()
{
    if (PEmitter.GetPItem() != 0)
        PEmitter.GetPItem()->EnterTimelineEnd();
    else
        ParentThing->Kill(true);
}
int main()
{
    CThing thing; NParticleEngine::CParticleEmitter emitter;
    CTCDParticleEmitter c; c.ParentThing = &thing;
    c.PEmitter.PData = &emitter; c.EnterTimelineEnd();
    if (g_ends != 1 || g_kills != 0) { std::printf("006e05f0_TEST FAIL\n"); return 1; }
    c.PEmitter.PData = 0; c.EnterTimelineEnd();
    if (g_ends != 1 || g_kills != 1) { std::printf("006e05f0_TEST FAIL\n"); return 1; }
    std::printf("006e05f0_TEST PASS\n");
    return 0;
}