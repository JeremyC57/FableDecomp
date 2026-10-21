// Behaviour test for 00d26a50 (CQ_DragonBossFightScript::JackTaunts): a 20-second timer gates five taunts
// TEXT_QST_B05_JACK_TAUNT_10 .. _50, each a new conversation with one line; the timer is re-armed after each taunt
// and always deregistered on exit, also when the thread terminates early.
#include <cstdio>
#include <cstring>

static int g_registered = 0, g_deregistered = 0;
struct Game { long timerValue; int frames, termAt, conversations; char lines[8][40]; int nLines; };
static Game* g_game = 0;
struct CTimer {
    long Index;
    CTimer() : Index(++g_registered) {}
    ~CTimer() { ++g_deregistered; }
    void SetTime(long s) { g_game->timerValue = s; }
    long GetTime() { return g_game->timerValue; }
};
struct Script {
    bool Term() { return g_game->frames >= g_game->termAt; }
    void Frame() { ++g_game->frames; if (g_game->timerValue > 0) g_game->timerValue -= 10; }
    void JackTaunts()
    {
        CTimer timer;
        long taunt = 10;
        timer.SetTime(20);
        while (taunt < 60) {
            Frame();
            if (Term()) return;
            if (timer.GetTime() == 0) {
                if (Term()) return;
                ++g_game->conversations;
                std::sprintf(g_game->lines[g_game->nLines++], "TEXT_QST_B05_JACK_TAUNT_%ld", taunt);
                timer.SetTime(20);
                taunt += 10;
            }
        }
        if (Term()) return;
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 1000; g_game = &g;
    Script s; s.JackTaunts();
    int ok = g.conversations == 5 && g.nLines == 5 && std::strcmp(g.lines[0], "TEXT_QST_B05_JACK_TAUNT_10") == 0 &&
             std::strcmp(g.lines[4], "TEXT_QST_B05_JACK_TAUNT_50") == 0 && g.frames == 10 && g_registered == 1 && g_deregistered == 1;
    Game h; std::memset(&h, 0, sizeof h); h.termAt = 3; g_game = &h;
    s.JackTaunts();
    if (h.conversations != 1 || g_deregistered != 2) ok = 0;
    if (!ok) { std::printf("00d26a50_TEST FAIL\n"); return 1; }
    std::printf("00d26a50_TEST PASS\n");
    return 0;
}