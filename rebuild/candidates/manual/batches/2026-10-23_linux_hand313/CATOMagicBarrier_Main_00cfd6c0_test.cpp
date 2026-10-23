// Behaviour test for 00cfd6c0 CATO_MagicBarrier::Main: respawn both barrier halves only when both are gone, until the stage/counter ends.
#include <cstdio>
static int Run(bool stageMode, int frames, const bool* alive1, const bool* alive2)
{
    int respawns = 0;
    for (int f = 0; f < frames; ++f) {
        bool gone = !alive1[f] && !alive2[f];
        if (gone) ++respawns;
    }
    return stageMode ? respawns : respawns * 10;
}
int main()
{
    bool a1[4] = { true, false, false, true };
    bool a2[4] = { true, true, false, false };
    if (Run(true, 4, a1, a2) != 1 || Run(false, 4, a1, a2) != 10) {
        std::printf("00cfd6c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cfd6c0_TEST PASS\n");
    return 0;
}
