#include <cstdio>
static int g_calls = 0; static bool g_arg = true;
struct CSubtitleRenderer { unsigned char _pad[0xe0]; bool m_visible, m_fading, m_queued; unsigned char _pade3[3]; bool m_forced;
    void SetAsInCutscene(bool b) { ++g_calls; g_arg = b; } void ResetCutsceneState(); };
void CSubtitleRenderer::ResetCutsceneState() { SetAsInCutscene(false); m_visible = true; m_fading = false; m_queued = false; m_forced = false; }
int main()
{
    CSubtitleRenderer r; r.m_visible = false; r.m_fading = r.m_queued = r.m_forced = true; r.ResetCutsceneState();
    if (g_calls != 1 || g_arg || !r.m_visible || r.m_fading || r.m_queued || r.m_forced) { std::printf("004933e6_TEST FAIL\n"); return 1; }
    std::printf("004933e6_TEST PASS\n"); return 0;
}
