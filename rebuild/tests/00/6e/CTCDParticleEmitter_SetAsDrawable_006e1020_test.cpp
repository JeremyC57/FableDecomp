// Behaviour test for CTCDParticleEmitter::SetAsDrawable(bool) @ 0x006E1020: only while attached to a live thing, the
// creation switch follows IsParticleCreationDisabled(); a missing or dead attachment does nothing.
#include <cstdio>
static int g_sets = 0;
static bool g_should = false;
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    CThing* GetPItem() const { return static_cast<CThing*>(PData); }
};
class CTCDParticleEmitter { public:
    virtual void v00() { }
    unsigned char _base[0x8]; void* EmitterName; long ParticleTypeID;
    CBaseIntelligentPointer PEmitter; CBaseIntelligentPointer PAttachedToThing;
    unsigned char _body[0x44 - 0x24];
    bool WaitingForParticleToLoad : 1; bool ForceParticleToLoad : 1; bool IndependantObject : 1; bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1; bool ResetEmitterOrientation : 1; bool AttachedToCamera : 1; bool Moving : 1;
    bool IsParticleCreationDisabled() const { return g_should; }
    void SetParticleCreationDisabled(bool disabled) { ++g_sets; ParticleCreationDisabled = disabled; }
    void SetAsDrawable(bool drawable);
};
void CTCDParticleEmitter::SetAsDrawable(bool drawable)
{
    if (PAttachedToThing.GetPItem() != 0 && !PAttachedToThing.GetPItem()->IsDead()) {
        if (IsParticleCreationDisabled()) {
            if (!ParticleCreationDisabled)
                SetParticleCreationDisabled(true);
        } else if (ParticleCreationDisabled) {
            SetParticleCreationDisabled(false);
        }
    }
}
int main()
{
    CThing host; host.m_flags91 = 0;
    CTCDParticleEmitter c; c.ParticleCreationDisabled = false; c.PAttachedToThing.PData = 0;
    g_should = true; c.SetAsDrawable(false);                  // no attachment
    if (g_sets != 0) { std::printf("006e1020_TEST FAIL\n"); return 1; }
    c.PAttachedToThing.PData = &host; host.m_flags91 = 1; c.SetAsDrawable(false);   // dead attachment
    if (g_sets != 0) { std::printf("006e1020_TEST FAIL\n"); return 1; }
    host.m_flags91 = 0; c.SetAsDrawable(false);                // disable
    if (g_sets != 1 || !c.ParticleCreationDisabled) { std::printf("006e1020_TEST FAIL\n"); return 1; }
    g_should = false; c.SetAsDrawable(true);                   // re-enable
    if (g_sets != 2 || c.ParticleCreationDisabled) { std::printf("006e1020_TEST FAIL\n"); return 1; }
    c.SetAsDrawable(true);                                     // already enabled
    if (g_sets != 2) { std::printf("006e1020_TEST FAIL\n"); return 1; }
    std::printf("006e1020_TEST PASS\n");
    return 0;
}