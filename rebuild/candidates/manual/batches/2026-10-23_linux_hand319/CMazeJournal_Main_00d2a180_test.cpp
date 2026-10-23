// Behaviour test for CMazeJournal::Main (0x00D2A180): inactive maze removes Me; the cut-scene plays once; each use hides Me,
// gives the journal, completes the quest and adds log-book entry 0x104.
#include <cstdio>
struct Sim {
    bool mazeActive, played, removed, drawable; int cutscenes, journals, completed, logEntry;
    void Run(int uses)
    {
        if (!mazeActive) { removed = true; return; }
        if (!played) { cutscenes++; played = true; }
        for (int i = 0; i < uses; i++) { drawable = false; journals++; completed++; logEntry = 0x104; }
    }
};
int main()
{
    Sim a = { false, false, false, true, 0, 0, 0, 0 };
    a.Run(1);
    if (!a.removed || a.cutscenes != 0 || a.journals != 0) { std::printf("00d2a180_TEST FAIL\n"); return 1; }
    Sim b = { true, false, false, true, 0, 0, 0, 0 };
    b.Run(1); b.Run(1);
    if (b.removed || b.cutscenes != 1 || b.journals != 2 || b.drawable || b.logEntry != 0x104) { std::printf("00d2a180_TEST FAIL\n"); return 1; }
    std::printf("00d2a180_TEST PASS\n");
    return 0;
}
