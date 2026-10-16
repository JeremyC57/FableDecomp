// Behaviour test for CTCTrophy::EndMinigame (0x006DDA50): only an in-progress minigame opens the aborted dialog (0x12a)
// and waits for termination.
#include <cstdio>
#include <cstring>
static char g_text[64]; static unsigned long g_dialog; static int g_calls;
struct Trophy {
    bool MinigameInProgress, WaitingForTerminate;
    void EndMinigame()
    {
        if (MinigameInProgress) {
            std::strcpy(g_text, "TEXT_GUI_TROPHY_GAME_ABORTED"); g_dialog = 0x12a; ++g_calls;
            WaitingForTerminate = true;
        }
    }
};
int main()
{
    Trophy a = { false, false }; a.EndMinigame();
    Trophy b = { true, false }; b.EndMinigame();
    if (a.WaitingForTerminate || !b.WaitingForTerminate || g_calls != 1 || g_dialog != 0x12a ||
        std::strcmp(g_text, "TEXT_GUI_TROPHY_GAME_ABORTED") != 0) { std::printf("006dda50_TEST FAIL\n"); return 1; }
    std::printf("006dda50_TEST PASS\n");
    return 0;
}
