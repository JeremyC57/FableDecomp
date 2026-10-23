// Behaviour test for CTCHeroOnlineScoreboard::ShowScoreboard (00563130): resets the task, and with a GUI either reports missing login details or starts state 1 with no selected clan
#include <cstdio>

struct Board {
    int TaskState, ScoreboardState, SelectedClan, ErrorShown, Components, ConnectingVisible, Connecting1Visible;
    bool HasHttp, HasMenuEntry;
};

static void ShowScoreboard(Board& b, bool hasGui, bool hasUser, bool hasPassword)
{
    b.HasMenuEntry = false;
    b.HasHttp = false;
    b.TaskState = 100;
    if (hasGui) {
        ++b.Components;
        b.ConnectingVisible = 0;
        ++b.Components;
        b.Connecting1Visible = 1;
        bool notSignedIn = !hasUser || !hasPassword;
        if (notSignedIn) {
            b.ErrorShown = 1;
            return;
        }
        b.ScoreboardState = 1;
        b.SelectedClan = 0;
    }
}

int main()
{
    Board a = { 5, 9, 3, 0, 0, -1, -1, true, true };
    ShowScoreboard(a, true, true, true);
    if (a.TaskState != 100 || a.ScoreboardState != 1 || a.SelectedClan != 0 || a.Components != 2 || a.HasHttp || a.ErrorShown) {
        std::printf("00563130_TEST FAIL\n"); return 1;
    }
    Board b = { 5, 9, 3, 0, 0, -1, -1, true, true };
    ShowScoreboard(b, true, true, false);
    if (b.ErrorShown != 1 || b.ScoreboardState != 9 || b.Connecting1Visible != 1 || b.ConnectingVisible != 0) {
        std::printf("00563130_TEST FAIL\n"); return 1;
    }
    Board c = { 5, 9, 3, 0, 0, -1, -1, true, true };
    ShowScoreboard(c, false, true, true);
    if (c.TaskState != 100 || c.ScoreboardState != 9 || c.Components != 0 || c.HasMenuEntry) {
        std::printf("00563130_TEST FAIL\n"); return 1;
    }
    std::printf("00563130_TEST PASS\n");
    return 0;
}