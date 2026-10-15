// Behaviour test for CAudioMixer::CloseAndCleanup (0x00C715D0): restore-on-close sends one control
// details block (id, one channel, 4-byte value), then the mixer is closed and the handle cleared.
#include <cstdio>

struct MIXERCONTROLDETAILS { unsigned int cbStruct, dwControlID, cChannels, cMultipleItems, cbDetails; void* paDetails; };
static int g_sets = 0, g_closes = 0; static unsigned int g_lastId = 0, g_lastValue = 0, g_lastFlags = 0;
static void StubSetControlDetails(void*, MIXERCONTROLDETAILS* d, unsigned int flags)
{
    ++g_sets; g_lastId = d->dwControlID; g_lastValue = *(unsigned int*)d->paDetails; g_lastFlags = flags;
    if (d->cbStruct != 0x18 || d->cChannels != 1 || d->cMultipleItems != 0 || d->cbDetails != 4) g_lastId = 0xdead;
}
static void StubClose(void*) { ++g_closes; }

struct CAudioMixer {
    void* m_hMixer; unsigned int m_volumeControlID; bool m_restoreOnClose; unsigned int m_savedVolume;
    void CloseAndCleanup()
    {
        if (m_hMixer) {
            if (m_restoreOnClose) {
                MIXERCONTROLDETAILS details;
                details.cbStruct = 0x18; details.dwControlID = m_volumeControlID; details.cChannels = 1;
                details.cMultipleItems = 0; details.cbDetails = 4; details.paDetails = &m_savedVolume;
                StubSetControlDetails(m_hMixer, &details, 0x80000000u);
            }
            StubClose(m_hMixer);
            m_hMixer = 0;
        }
    }
};

int main()
{
    int dummy;
    CAudioMixer m = { &dummy, 77, true, 1234 };
    m.CloseAndCleanup();
    if (g_sets != 1 || g_closes != 1 || g_lastId != 77 || g_lastValue != 1234 || g_lastFlags != 0x80000000u || m.m_hMixer) {
        std::printf("00c715d0_TEST FAIL\n"); return 1;
    }
    m.CloseAndCleanup();
    CAudioMixer n = { &dummy, 5, false, 0 };
    n.CloseAndCleanup();
    if (g_sets != 1 || g_closes != 2) { std::printf("00c715d0_TEST FAIL\n"); return 1; }
    std::printf("00c715d0_TEST PASS\n");
    return 0;
}
