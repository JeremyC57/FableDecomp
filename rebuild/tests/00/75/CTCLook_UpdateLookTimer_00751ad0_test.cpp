#include <cstdio>
struct CFrameSource { long m_frame; long GetFrameNumber() { return m_frame; } };
static CFrameSource g_source;
class CTCLook { public:
    long LookTimer;
    CFrameSource* GetFrameSource() { return &g_source; }
    void UpdateLookTimer() { LookTimer = GetFrameSource()->GetFrameNumber() + 90; }
};
int main()
{
    CTCLook l; l.LookTimer = 0; g_source.m_frame = 1000;
    l.UpdateLookTimer();
    if (l.LookTimer != 1090) { std::printf("00751ad0_TEST FAIL\n"); return 1; }
    g_source.m_frame = -5; l.UpdateLookTimer();
    if (l.LookTimer != 85) { std::printf("00751ad0_TEST FAIL\n"); return 1; }
    std::printf("00751ad0_TEST PASS\n");
    return 0;
}