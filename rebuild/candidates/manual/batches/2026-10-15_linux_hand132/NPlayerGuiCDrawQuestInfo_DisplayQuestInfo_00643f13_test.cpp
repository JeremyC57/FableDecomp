#include <cstdio>
// Behaviour test for 00643f13.

// NPlayerGui::CDrawQuestInfo::DisplayQuestInfo: opening from closing/opening resets the timers and arms the mini-mode countdown
// (frames per second * def delay); closing from open/hidden sets mode 1.
struct CPlayerGuiDef { float QuestInfoMiniModeDelaySeconds; };
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
static long g_framesPerSecond = 30;
struct CDrawQuestInfo {
    long GuiMode; float FadeInTimer; float MiniModeTimer; bool MiniMode; long FramesUntilMiniMode;
    void DisplayQuestInfo(bool display)
    {
        if (display) {
            if (GuiMode == 3 || GuiMode == 1) {
                GuiMode = 2; FadeInTimer = 0.0f; MiniMode = false; MiniModeTimer = 0.0f;
                long framesPerSecond = g_framesPerSecond;
                FramesUntilMiniMode = (long)(framesPerSecond * PeekGuiDef()->QuestInfoMiniModeDelaySeconds);
            }
        }
        else if (GuiMode == 2 || GuiMode == 0) { GuiMode = 1; MiniMode = false; }
    }
};
int main()
{
    g_def.QuestInfoMiniModeDelaySeconds = 2.5f;
    CDrawQuestInfo q; q.GuiMode = 1; q.FadeInTimer = 3; q.MiniModeTimer = 4; q.MiniMode = true; q.FramesUntilMiniMode = 0;
    int ok = 1;
    q.DisplayQuestInfo(true);
    if (q.GuiMode != 2 || q.FadeInTimer != 0.0f || q.MiniModeTimer != 0.0f || q.MiniMode || q.FramesUntilMiniMode != 75) ok = 0;
    q.FramesUntilMiniMode = 1; q.DisplayQuestInfo(true);
    if (q.FramesUntilMiniMode != 1) ok = 0;
    q.MiniMode = true; q.DisplayQuestInfo(false);
    if (q.GuiMode != 1 || q.MiniMode) ok = 0;
    q.GuiMode = 3; q.DisplayQuestInfo(false);
    if (q.GuiMode != 3) ok = 0;
    if (!ok) { std::printf("00643f13_TEST FAIL\n"); return 1; }
    std::printf("00643f13_TEST PASS\n");
    return 0;
}
