#include <cstdio>

static int g_mode;
struct CTCQuestCompletionUI {
    bool IsHeroControlledByPlayer() const
    {
        switch (g_mode) {
        case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        case 17: case 18: case 21: case 23: case 26: case 27: case 29: case 30: case 32: case 36: case 37:
            return false;
        default:
            return true;
        }
    }
};
static bool Run()
{
    static const int offModes[] = { 4, 9, 15, 17, 18, 21, 23, 26, 27, 29, 30, 32, 36, 37 };
    static const int onModes[] = { -1, 0, 1, 3, 16, 19, 20, 22, 24, 25, 28, 31, 33, 35, 38, 100 };
    CTCQuestCompletionUI ui;
    for (unsigned i = 0; i < sizeof(offModes) / sizeof(offModes[0]); ++i) { g_mode = offModes[i]; if (ui.IsHeroControlledByPlayer()) return false; }
    for (unsigned i = 0; i < sizeof(onModes) / sizeof(onModes[0]); ++i) { g_mode = onModes[i]; if (!ui.IsHeroControlledByPlayer()) return false; }
    return true;
}

int main()
{
    if (!Run()) { std::printf("005e3ea0_TEST FAIL\n"); return 1; }
    std::printf("005e3ea0_TEST PASS\n");
    return 0;
}