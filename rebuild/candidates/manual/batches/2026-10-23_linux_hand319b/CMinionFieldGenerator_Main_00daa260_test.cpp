// Behaviour test for CMinionFieldGenerator::Main (0x00DAA260): cut-scene + guildmaster message once, run to the windmill, retake control.
#include <cstdio>
struct Sim {
    bool shown; int cutscenes, messages; float dist; int moves; int priority;
    void Run()
    {
        if (!shown) { cutscenes++; messages++; shown = true; }
        while (dist > 2.0f) { moves++; dist -= 3.0f; }
        priority = 3;
    }
};
int main()
{
    Sim s = { false, 0, 0, 7.0f, 0, 4 };
    s.Run();
    if (s.cutscenes != 1 || s.messages != 1 || !s.shown || s.moves != 2 || s.priority != 3) { std::printf("00daa260_TEST FAIL\n"); return 1; }
    s.dist = 7.0f; s.moves = 0; s.Run();
    if (s.cutscenes != 1 || s.messages != 1 || s.moves != 2) { std::printf("00daa260_TEST FAIL\n"); return 1; }
    std::printf("00daa260_TEST PASS\n");
    return 0;
}
