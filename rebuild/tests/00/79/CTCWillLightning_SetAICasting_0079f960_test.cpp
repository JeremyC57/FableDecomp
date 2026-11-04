// Behaviour test for CTCWillLightning::SetAICasting 0x0079F960: the will timer is flagged as AI-cast and the spell is set up for the
// requested level (SetLevel with the same level), nothing else of the timer changes.
#include <cstdio>
struct CWillButtonHelperTimer {
    bool IsAICasting, IsAIHoldingDown;
    long LevelToCast;
    void SetAICasting() { IsAICasting = true; }
};
struct CTCWillLightning {
    CWillButtonHelperTimer m_tTimer;
    long levelSet;
    int setLevelCalls;
    void SetLevel(long level) { levelSet = level; ++setLevelCalls; }
    void SetAICasting(long level) { m_tTimer.SetAICasting(); SetLevel(level); }
};
int main()
{
    CTCWillLightning w;
    w.m_tTimer.IsAICasting = false; w.m_tTimer.IsAIHoldingDown = false; w.m_tTimer.LevelToCast = 7;
    w.levelSet = -1; w.setLevelCalls = 0;
    w.SetAICasting(3);
    if (!w.m_tTimer.IsAICasting || w.m_tTimer.IsAIHoldingDown || w.m_tTimer.LevelToCast != 7 || w.levelSet != 3 || w.setLevelCalls != 1) {
        std::printf("0079f960_TEST FAIL\n"); return 1;
    }
    std::printf("0079f960_TEST PASS\n");
    return 0;
}