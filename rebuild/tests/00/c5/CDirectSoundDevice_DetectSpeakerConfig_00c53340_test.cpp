// Behaviour test for 0x00C53340 CDirectSoundDevice::DetectSpeakerConfig: the DSSPEAKER_CONFIG low byte picks the speaker
// mode bit (unknown layouts 0x80000000, high bytes ignored); a failed GetSpeakerConfig releases DirectSound and returns false.
#include <cstdio>

static int g_uninit = 0;
static void CoUninitialize() { ++g_uninit; }

struct IDirectSound {
    unsigned int m_config; long m_hr; int m_released;
    unsigned long Release() { ++m_released; return 0; }
    long GetSpeakerConfig(unsigned int* config) { *config = m_config; return m_hr; }
};

class CDirectSoundDevice {
public:
    bool DetectSpeakerConfig();
    void ReleaseDirectSound()
    {
        m_pDirectSound->Release();
        CoUninitialize();
        m_pDirectSound = 0;
    }
    unsigned int  m_speakerMode;
    IDirectSound* m_pDirectSound;
};

bool CDirectSoundDevice::DetectSpeakerConfig()
{
    unsigned int config;
    if (m_pDirectSound->GetSpeakerConfig(&config) < 0) {
        ReleaseDirectSound();
        return false;
    }
    switch ((unsigned char)config) {
    case 2:  m_speakerMode = 2;    break;
    case 1:  m_speakerMode = 1;    break;
    case 4:  m_speakerMode = 4;    break;
    case 3:  m_speakerMode = 8;    break;
    case 6:  m_speakerMode = 0x40; break;
    case 7:  m_speakerMode = 0x80; break;
    default: m_speakerMode = 0x80000000u; break;
    }
    return true;
}

static int fail() { std::printf("00c53340_TEST FAIL\n"); return 1; }

int main()
{
    static const unsigned int expect[9] = { 0x80000000u, 1, 2, 8, 4, 0x80000000u, 0x40, 0x80, 0x80000000u };
    for (unsigned int c = 0; c < 9; ++c)
    {
        IDirectSound ds = { c | 0x00050000u, 0, 0 };   // DSSPEAKER_COMBINED geometry in the high bytes is ignored
        CDirectSoundDevice d; d.m_speakerMode = 0x12345678; d.m_pDirectSound = &ds;
        if (!d.DetectSpeakerConfig() || d.m_speakerMode != expect[c] || ds.m_released || d.m_pDirectSound != &ds) return fail();
    }
    IDirectSound bad = { 4, -1, 0 };
    CDirectSoundDevice d; d.m_speakerMode = 7; d.m_pDirectSound = &bad;
    if (d.DetectSpeakerConfig() || bad.m_released != 1 || g_uninit != 1 || d.m_pDirectSound != 0 || d.m_speakerMode != 7) return fail();
    std::printf("00c53340_TEST PASS\n");
    return 0;
}