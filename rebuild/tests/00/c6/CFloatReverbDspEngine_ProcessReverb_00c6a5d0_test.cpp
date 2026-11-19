// Behaviour test for 00c6a5d0: CReverbProcessor::Process -- not ready: false and nothing runs; ready: the five stages in order, the active flag kept
#include <cstdio>
#include <cstring>
static char g_seq[16] = ""; static float* g_mixA = 0; static float* g_mixB = 0; static unsigned long g_count = 0;
struct CReverbProcessor {
    bool m_bWasActive; bool m_bReady;
    void WriteInput(const float*, bool, unsigned long count) { std::strcat(g_seq, "W"); g_count = count; }
    void ProcessCombs(const float* const*, unsigned long, float* output) { std::strcat(g_seq, "C"); g_mixA = output; }
    void ProcessMix(unsigned long, float* mix) { std::strcat(g_seq, "M"); g_mixB = mix; }
    void ProcessStage94(const float* const*, unsigned long) { std::strcat(g_seq, "9"); }
    void ProcessStage144(const float* const*, unsigned long) { std::strcat(g_seq, "4"); }
    bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count)
    {
        float mix[128];
        if (!m_bReady) return false;
        WriteInput(dry, active, count); ProcessCombs(inputs, count, mix); ProcessMix(count, mix);
        ProcessStage94(inputs, count); ProcessStage144(inputs, count); m_bWasActive = active; return true;
    }
};
int main()
{
    CReverbProcessor r; r.m_bReady = false; r.m_bWasActive = false; float dry[4] = { 0 }; const float* ch[2] = { dry, dry };
    int ok = !r.Process(dry, true, ch, 4) && g_seq[0] == 0;
    r.m_bReady = true; ok = ok && r.Process(dry, true, ch, 4) && !std::strcmp(g_seq, "WCM94") && r.m_bWasActive && g_count == 4 && g_mixA == g_mixB && g_mixA;
    g_seq[0] = 0; ok = ok && r.Process(dry, false, ch, 2) && !r.m_bWasActive;
    std::printf(ok ? "00c6a5d0_TEST PASS\n" : "00c6a5d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
