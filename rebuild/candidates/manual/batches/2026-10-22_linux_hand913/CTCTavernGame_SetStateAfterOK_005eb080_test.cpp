#include <cstdio>

static bool g_quit;
static int g_sounds, g_texts;
struct CTCTavernGame {
    long YesState, NoState, AltState, TavernGameState, PreviousTavernGameState; int leaves;
    CTCTavernGame() : YesState(9), NoState(9), AltState(9), TavernGameState(30), PreviousTavernGameState(0), leaves(0) {}
    void LeaveGame() { ++leaves; }
    void PlaySound() { ++g_sounds; }
    void SetState(long s)
    {
        if (g_quit) { g_quit = false; LeaveGame(); return; }
        PreviousTavernGameState = TavernGameState;
        TavernGameState = s;
    }
    void SetStateAfterOK(long state, bool quitDuringPrompt)
    {
        if (g_quit) { g_quit = false; LeaveGame(); return; }
        PlaySound();
        ++g_texts;
        if (quitDuringPrompt) g_quit = true;
        YesState = state; NoState = 0; AltState = 0;
        SetState(44);
    }
};
static bool Run()
{
    CTCTavernGame a; a.SetStateAfterOK(12, false);
    bool ok = a.YesState == 12 && a.NoState == 0 && a.AltState == 0 && a.PreviousTavernGameState == 30 && a.TavernGameState == 44 && a.leaves == 0;
    CTCTavernGame b; g_quit = true; b.SetStateAfterOK(12, false);
    ok = ok && b.leaves == 1 && !g_quit && b.YesState == 9 && b.TavernGameState == 30 && g_sounds == 1;
    CTCTavernGame c; c.SetStateAfterOK(13, true);
    ok = ok && c.leaves == 1 && c.YesState == 13 && c.TavernGameState == 30 && g_sounds == 2 && g_texts == 2;
    return ok;
}

int main()
{
    if (!Run()) { std::printf("005eb080_TEST FAIL\n"); return 1; }
    std::printf("005eb080_TEST PASS\n");
    return 0;
}
