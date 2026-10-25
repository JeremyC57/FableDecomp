#include <cstdio>
#include <cstring>
static int g_resets = 0, g_writes = 0; static float g_lastFirst = -1.0f; static float* g_lastBlock = 0;
static void ResetDecoder(void* decoder) { (void)decoder; ++g_resets; }
struct CStreamProgressMonitor {
    bool accept; bool lastQuiet; long lastAmount; int zeroed;
    bool UpdateProgress(void* source, bool quiet, void* context, long amount)
    {
        (void)source;
        float** ch = (float**)context;
        zeroed = 1;
        for (int c = 0; c < 4; ++c)
            for (int i = 0; i < 128; ++i)
                if (ch[c][i] != 0.0f) zeroed = 0;
        if (ch[1] != ch[0] + 128 || ch[3] != ch[0] + 384) zeroed = 0;
        ch[0][0] = 0.5f;
        lastQuiet = quiet; lastAmount = amount;
        return accept;
    }
};
struct CSampleSink { void Write(float* samples) { ++g_writes; g_lastFirst = samples[0]; g_lastBlock = samples; } };
struct CDecodedStream {
    bool m_restartPending; void* m_pDecoder; CSampleSink* m_pSink; CStreamProgressMonitor m_monitor;
    void PumpBlock()
    {
        float samples[4][128];
        float* channels[4] = { samples[0], samples[1], samples[2], samples[3] };
        for (int i = 0; i < 128; ++i) samples[2][i] = 9.0f;     // stale data the memset must clear
        if (m_restartPending)
            ResetDecoder(m_pDecoder);
        std::memset(samples, 0, sizeof(samples));
        if (m_monitor.UpdateProgress(m_pDecoder, m_restartPending, channels, 128))
            m_pSink->Write(samples[0]);
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c546e0_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    CSampleSink sink; CDecodedStream s; s.m_pDecoder = 0; s.m_pSink = &sink; s.m_restartPending = true; s.m_monitor.accept = true;
    s.PumpBlock();
    CHECK(g_resets == 1 && g_writes == 1 && g_lastFirst == 0.5f && s.m_monitor.zeroed == 1 && s.m_monitor.lastQuiet && s.m_monitor.lastAmount == 128);
    s.m_restartPending = false; s.m_monitor.accept = false;
    s.PumpBlock();
    CHECK(g_resets == 1 && g_writes == 1 && !s.m_monitor.lastQuiet);
    std::printf("00c546e0_TEST PASS\n");
    return 0;
}