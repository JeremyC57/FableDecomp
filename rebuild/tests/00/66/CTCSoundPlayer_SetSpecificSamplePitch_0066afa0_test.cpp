// Behaviour test for CTCSoundPlayer::SetSpecificSamplePitch (0066afa0): forwards to a live emitter only with the sound system up.
#include <cstdio>

static int g_kills = 0, g_sets = 0;
static unsigned long g_sample = 0;
static float g_value = 0.0f;

struct CThingSoundEmitter {
    unsigned char m_thingFlags;
    void Kill(bool immediately) { if (immediately) ++g_kills; m_thingFlags |= 1; }
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch) { ++g_sets; g_sample = sampleID; g_value = pitch; }
    void SetSpecificSampleVolume(unsigned long sampleID, float volume) { ++g_sets; g_sample = sampleID; g_value = volume; }
};

struct CBaseIntelligentPointer {
    CThingSoundEmitter* m_pItem;
    CThingSoundEmitter* GetPItem() const { return m_pItem; }
};

void* g_pSoundSystem = 0;

struct CTCSoundPlayer {
    CBaseIntelligentPointer PAttachedEmitter;
    bool HasLiveAttachedEmitter() const
    {
        return PAttachedEmitter.GetPItem() && !(PAttachedEmitter.GetPItem()->m_thingFlags & 1);
    }
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);
};

void CTCSoundPlayer::SetSpecificSamplePitch(unsigned long sampleID, float pitch)
{
    if (g_pSoundSystem) {
        if (HasLiveAttachedEmitter())
            PAttachedEmitter.GetPItem()->SetSpecificSamplePitch(sampleID, pitch);
    }
}

int main()
{
    CThingSoundEmitter emitter; emitter.m_thingFlags = 0;
    CTCSoundPlayer p; p.PAttachedEmitter.m_pItem = &emitter;
    int system = 1;
    p.SetSpecificSamplePitch(3, 0.5f);                         // no sound system
    g_pSoundSystem = &system;
    p.SetSpecificSamplePitch(7, 1.5f);                         // forwarded
    emitter.m_thingFlags = 1;
    p.SetSpecificSamplePitch(9, 2.5f);                         // killed emitter
    if (g_sets != 1 || g_sample != 7 || g_value != 1.5f) { std::printf("0066afa0_TEST FAIL\n"); return 1; }
    std::printf("0066afa0_TEST PASS\n");
    return 0;
}