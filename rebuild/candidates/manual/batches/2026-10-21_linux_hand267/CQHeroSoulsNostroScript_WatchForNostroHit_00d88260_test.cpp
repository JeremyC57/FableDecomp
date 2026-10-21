// Behaviour test for 00d88260 (CQ_HeroSoulsNostroScript::WatchForNostroHit): wait while Nostro has lost < 100
// health; if the 80-second timer expires first, add the one-line Nostro conversation; the timer is always
// deregistered.
#include <cstdio>

struct Game {
    int frames; float health; int damageAt; int timerLeft; int registered; int conversations; int lines; int termAt;
    void NewScriptFrame() { ++frames; if (timerLeft > 0) --timerLeft; if (frames == damageAt) health -= 150.0f; }
    float GetHealth() const { return health; }
};
struct Timer {
    Game* g; Timer(Game* game) : g(game) { ++g->registered; }
    ~Timer() { --g->registered; }
    void SetTime(int s) { g->timerLeft = s; }
    int GetTime() const { return g->timerLeft; }
};
struct Script {
    Game* game;
    bool IsActiveThreadTerminating() { return game->frames >= game->termAt; }
    void WatchForNostroHit()
    {
        float threshold = game->GetHealth() - 100.0f;
        Timer timer(game);
        timer.SetTime(80);
        while (game->GetHealth() > threshold) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (timer.GetTime() == 0) {
                if (IsActiveThreadTerminating()) return;
                ++game->conversations; ++game->lines;
                break;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    int ok = 1;
    Game g = { 0, 500.0f, 10, 0, 0, 0, 0, 1000 }; Script s = { &g }; s.WatchForNostroHit();
    if (g.frames != 10 || g.conversations != 0 || g.registered != 0) ok = 0;
    Game h = { 0, 500.0f, 1000, 0, 0, 0, 0, 1000 }; Script t = { &h }; t.WatchForNostroHit();
    if (h.frames != 80 || h.conversations != 1 || h.lines != 1 || h.registered != 0) ok = 0;
    Game k = { 0, 500.0f, 1000, 0, 0, 0, 0, 5 }; Script u = { &k }; u.WatchForNostroHit();
    if (k.frames != 5 || k.conversations != 0 || k.registered != 0) ok = 0;
    if (!ok) { std::printf("00d88260_TEST FAIL\n"); return 1; }
    std::printf("00d88260_TEST PASS\n");
    return 0;
}
