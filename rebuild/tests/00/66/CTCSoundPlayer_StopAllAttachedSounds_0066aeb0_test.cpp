// Behaviour test for CTCSoundPlayer::StopAllAttachedSounds (0x0066AEB0): kills a live emitter once, only with the sound system up.
#include <cstdio>

static int g_kills = 0, g_sets = 0;
static unsigned long g_sample = 0;
static float g_value = 0.0f;

class CThingSoundEmitter { public:
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

class CTCSoundPlayer { public:
    CBaseIntelligentPointer PAttachedEmitter;
    bool HasLiveAttachedEmitter() const
    {
        return PAttachedEmitter.GetPItem() && !(PAttachedEmitter.GetPItem()->m_thingFlags & 1);
    }
    void StopAllAttachedSounds();
};

void CTCSoundPlayer::StopAllAttachedSounds()
{
    if (g_pSoundSystem) {
        if (HasLiveAttachedEmitter())
            PAttachedEmitter.GetPItem()->Kill(true);
    }
}

int main()
{
    CThingSoundEmitter emitter; emitter.m_thingFlags = 0;
    CTCSoundPlayer p; p.PAttachedEmitter.m_pItem = 0;
    int system = 1;
    p.StopAllAttachedSounds();                              // no sound system
    g_pSoundSystem = &system;
    p.StopAllAttachedSounds();                              // no emitter
    p.PAttachedEmitter.m_pItem = &emitter;
    p.StopAllAttachedSounds();                              // kills
    p.StopAllAttachedSounds();                              // already killed
    if (g_kills != 1) { std::printf("0066aeb0_TEST FAIL\n"); return 1; }
    std::printf("0066aeb0_TEST PASS\n");
    return 0;
}