// Behaviour test for 0x0057A690 CTCHeroStats::GetMoralityPercentage() const on the shared engine/CTCHeroStats.h (links the row object):
// (Morality + max) / (2 * max), with GetMoralityMax supplied here.
#include <cstdio>
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#include "engine/CTCHeroStats.h"
static long g_max = 50;
static int g_calls = 0;
long CTCHeroStats::GetMoralityMax() const { ++g_calls; return g_max; }
static long g_storage[0x1dc / 4 + 1];
int main()
{
    CTCHeroStats& stats = *(CTCHeroStats*)g_storage;
    stats.Morality = 20;
    float r = stats.GetMoralityPercentage();          // (20 + 50) / 100
    stats.Morality = -50;
    float low = stats.GetMoralityPercentage();        // 0 / 100
    if (r > 0.6999f && r < 0.7001f && low > -0.0001f && low < 0.0001f && g_calls == 4) {
        std::printf("CTCHeroStats_0057a690_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%f\n", r);
    return 1;
}
