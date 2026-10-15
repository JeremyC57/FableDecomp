// Behaviour test for 0x006E71F0 CGameScriptInterface::Rest: yields frames until the end frame or a stop request.
#include <cstdio>
static int g_frame = 100;
static int g_FramesPerSecond = 30;
static int GetGameFrame() { return g_frame; }
struct CScriptRunner {
    bool m_bStopRequested; int yields; int stopAfter;
    void YieldFrame() { ++yields; ++g_frame; if (yields == stopAfter) m_bStopRequested = true; }
};
static CScriptRunner* g_pScriptRunner = 0;
static void Rest(float seconds)
{
    if (g_pScriptRunner) {
        int fps = g_FramesPerSecond;
        float endFrame = (float)GetGameFrame() + (float)fps * seconds;
        while ((float)GetGameFrame() < endFrame) {
            if (g_pScriptRunner->m_bStopRequested) break;
            g_pScriptRunner->YieldFrame();
        }
    }
}
static int Fail() { std::printf("006e71f0_TEST FAIL\n"); return 1; }
int main()
{
    Rest(1.0f);
    if (g_frame != 100) return Fail();
    CScriptRunner r; r.m_bStopRequested = false; r.yields = 0; r.stopAfter = -1;
    g_pScriptRunner = &r;
    Rest(0.5f);
    if (r.yields != 15 || g_frame != 115) return Fail();
    r.yields = 0; r.stopAfter = 4;
    Rest(2.0f);
    if (r.yields != 4 || g_frame != 119) return Fail();
    std::printf("006e71f0_TEST PASS\n");
    return 0;
}
