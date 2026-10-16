#include <cstdio>
#include <cstring>
// Behaviour model of CTCHeroStats::GetDrinkResistance (0x0057A7B9): drinker class 1..5 and its text symbol.
static long Level(float multiplier)
{
    long level = (long)((1.0f - multiplier) * 5.0f + 1.0f);
    if (level < 1) level = 1;
    if (level > 5) level = 5;
    return level;
}
int main()
{
    char sym[64];
    std::sprintf(sym, "%s%ld", "TEXT_GUI_STATS_LABEL_DRINKERCLASS", Level(0.5f));
    if (Level(1.0f) != 1 || Level(0.5f) != 3 || Level(0.0f) != 5 || Level(2.0f) != 1 || Level(-1.0f) != 5
        || std::strcmp(sym, "TEXT_GUI_STATS_LABEL_DRINKERCLASS3") != 0) { std::printf("0057a7b9_TEST FAIL\n"); return 1; }
    std::printf("0057a7b9_TEST PASS\n");
    return 0;
}
