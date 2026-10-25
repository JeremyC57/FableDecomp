#include <cstdio>
static int g_ramps = 0, g_scales = 0; static unsigned short g_lastVol = 0; static int g_lastStep = 0; static void* g_lastDest = 0;
struct CVolumeScaler {
    void Ramp(void* dest, unsigned short volume, int step) { ++g_ramps; g_lastDest = dest; g_lastVol = volume; g_lastStep = step; }
    void Scale(void* dest, unsigned short volume) { ++g_scales; g_lastDest = dest; g_lastVol = volume; }
};
struct CAudioMaster { CVolumeScaler* m_pScaler; };
static CAudioMaster g_master; static CAudioMaster* g_pAudioMaster = &g_master;
struct CAudioSource {
    unsigned short m_volume, m_targetVolume; bool m_ramping;
    void ApplyVolume(void* dest)
    {
        if (!g_pAudioMaster->m_pScaler)
            return;
        if (m_ramping) {
            m_ramping = false;
            g_pAudioMaster->m_pScaler->Ramp(dest, m_volume, (m_targetVolume - m_volume) / 128);
            m_volume = m_targetVolume;
        } else {
            g_pAudioMaster->m_pScaler->Scale(dest, m_volume);
        }
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c54ea0_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CVolumeScaler scaler; int buf[4];
    CAudioSource v; v.m_volume = 1000; v.m_targetVolume = 3000; v.m_ramping = true;
    g_master.m_pScaler = 0;
    v.ApplyVolume(buf);
    CHECK(g_ramps == 0 && g_scales == 0 && v.m_ramping && v.m_volume == 1000);
    g_master.m_pScaler = &scaler;
    v.ApplyVolume(buf);
    CHECK(g_ramps == 1 && g_lastDest == buf && g_lastVol == 1000 && g_lastStep == 15 && !v.m_ramping && v.m_volume == 3000);
    v.ApplyVolume(buf);
    CHECK(g_scales == 1 && g_lastVol == 3000);
    v.m_targetVolume = 1000; v.m_ramping = true;                 // downward ramp: signed division truncates toward zero
    v.ApplyVolume(buf);
    CHECK(g_ramps == 2 && g_lastStep == -15 && v.m_volume == 1000);
    std::printf("00c54ea0_TEST PASS\n");
    return 0;
}