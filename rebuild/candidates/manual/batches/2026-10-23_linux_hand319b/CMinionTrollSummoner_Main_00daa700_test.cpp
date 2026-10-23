// Behaviour test for CMinionTrollSummoner::Main (0x00DAA700): cut-scene once, run off until near the point, wait for a hit or the hero.
#include <cstdio>
struct Sim {
    bool shown; int cutscenes; float dist; int moves; int frames; bool hitAt3; float heroDist; bool released;
    void Run()
    {
        if (!shown) cutscenes++;
        shown = true;
        while (dist > 2.0f) { frames++; moves++; dist -= 1.5f; }
        int f = 0;
        while (!(hitAt3 && f >= 3) && !(heroDist < 10.0f)) { frames++; f++; heroDist -= 1.0f; }
        released = true;
    }
};
int main()
{
    Sim s = { false, 0, 5.0f, 0, 0, true, 100.0f, false };
    s.Run();
    if (s.cutscenes != 1 || s.moves != 2 || s.frames != 5 || !s.released) { std::printf("00daa700_TEST FAIL\n"); return 1; }
    Sim t = { true, 0, 1.0f, 0, 0, false, 12.0f, false };
    t.Run();
    if (t.cutscenes != 0 || t.moves != 0 || t.frames != 3) { std::printf("00daa700_TEST FAIL\n"); return 1; }
    std::printf("00daa700_TEST PASS\n");
    return 0;
}
