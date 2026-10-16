#include <cstdio>
// Behaviour model of CTCOracleMinigame::SetText (0x007E6230): only a changed text with a live hero is shown and stored.
struct Game { unsigned long current; int shown; };
static void SetText(Game& g, unsigned long text, bool heroAlive)
{
    if (g.current != text && heroAlive) { g.shown++; g.current = text; }
}
int main()
{
    Game g = { 1, 0 };
    SetText(g, 1, true); SetText(g, 2, false); SetText(g, 3, true);
    if (g.current != 3 || g.shown != 1) { std::printf("007e6230_TEST FAIL\n"); return 1; }
    std::printf("007e6230_TEST PASS\n");
    return 0;
}
