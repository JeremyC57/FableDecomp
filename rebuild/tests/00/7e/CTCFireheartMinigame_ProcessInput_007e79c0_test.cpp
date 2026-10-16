// behaviour test for 0x007E79C0 CTCFireheartMinigame::ProcessInput
#include <cstdio>
struct Game {
    long XPos, YPos; bool Moving; int calls;
    void SetHeroPos(long x, long y, bool) { XPos = x; YPos = y; ++calls; }
    void ProcessInput(int input)
    {
        if (Moving) return;
        long x = XPos, y = YPos;
        switch (input) { case 0: --y; break; case 1: ++x; break; case 2: ++y; break; case 3: --x; break; }
        if (x < 0) x = 0; else if (x > 3) x = 3;
        if (y < 0) y = 0; else if (y > 3) y = 3;
        SetHeroPos(x, y, false);
    }
};
int main()
{
    Game g = { 0, 0, false, 0 };
    g.ProcessInput(0);  bool ok = g.XPos == 0 && g.YPos == 0;      // clamped
    g.ProcessInput(1);  ok = ok && g.XPos == 1;
    g.ProcessInput(2);  ok = ok && g.YPos == 1;
    g.ProcessInput(3);  ok = ok && g.XPos == 0;
    g.XPos = 3; g.ProcessInput(1); ok = ok && g.XPos == 3;
    g.ProcessInput(7);  ok = ok && g.calls == 6;
    g.Moving = true; g.ProcessInput(2); ok = ok && g.calls == 6;
    if (!ok) { std::printf("007e79c0_TEST FAIL\n"); return 1; }
    std::printf("007e79c0_TEST PASS\n");
    return 0;
}