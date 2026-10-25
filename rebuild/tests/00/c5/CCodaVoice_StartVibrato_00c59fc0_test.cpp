// Behaviour test for 00c59fc0: CCodaVoice::StartVibrato -- SetVibrato always, then the delay counter from the seconds
#include <cstdio>
static int g_set = 0; static const void* g_setArg = 0; static float g_seconds = 0.0f;
struct SVibratoParams { float delay; float rate; float depth; };
static long SecondsToMixBlocks(float seconds) { g_seconds = seconds; return (long)(seconds * 100.0f); }
class CCodaVoice {
public:
    long m_vibratoCounter;
    void SetVibrato(const SVibratoParams* vib) { ++g_set; g_setArg = vib; }
    void StartVibrato(const SVibratoParams* vib) { SetVibrato(vib); if (vib) m_vibratoCounter = SecondsToMixBlocks(vib->delay); }
};
int main()
{
    CCodaVoice v; v.m_vibratoCounter = 7;
    SVibratoParams p = { 0.25f, 5.0f, 0.1f };
    v.StartVibrato(&p);
    int ok = g_set == 1 && g_setArg == &p && g_seconds == 0.25f && v.m_vibratoCounter == 25;
    v.StartVibrato(0);
    ok = ok && g_set == 2 && g_setArg == 0 && v.m_vibratoCounter == 25;
    std::printf(ok ? "00c59fc0_TEST PASS\n" : "00c59fc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
