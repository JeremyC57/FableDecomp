#include <cstdio>
struct Game { int updates, hud, bars, lastValue; long Add() { ++bars; return 77; } };
struct Script {
    Game* game; long chunks; long BarID; int doneAfter; int checks;
    bool IsDone() { return ++checks > doneAfter; }
    void Main()
    {
        game->updates++;
        if (IsDone()) return;
        game->hud = 1;
        BarID = game->Add();
        while (!IsDone()) { game->lastValue = (int)chunks; game->updates++; }
    }
};
int main()
{
    Game g = { 0, 0, 0, 0 };
    Script s = { &g, 5, 0, 3, 0 };
    s.Main();
    Game g2 = { 0, 0, 0, 0 };
    Script t = { &g2, 5, 0, 0, 0 };
    t.Main();
    if (s.BarID != 77 || g.hud != 1 || g.updates != 3 || g.lastValue != 5 || g2.hud != 0 || g2.updates != 1 || t.BarID != 0) { std::printf("00cfc990_TEST FAIL\n"); return 1; }
    std::printf("00cfc990_TEST PASS\n");
    return 0;
}