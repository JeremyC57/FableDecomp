#include <cstdio>
#include <cmath>
// Behaviour test: CTCAvoidRegionExit::UpdateBestDist -- the best distance is the largest value in the distance map, never below zero.
#define CHECK(c) if (!(c)) { std::printf("007e9250_TEST FAIL\n"); return 1; }
struct SEntry { void* thing; float second; };
static SEntry g_map[8];
static int g_count = 0;
static float g_best = -1.0f;
static void UpdateBestDist()
{
    g_best = 0.0f;
    for (int i = 0; i < g_count; ++i)
        if (g_map[i].second > g_best)
            g_best = g_map[i].second;
}
int main()
{
    UpdateBestDist(); CHECK(g_best == 0.0f)
    g_map[0].second = 3.0f; g_map[1].second = 7.5f; g_map[2].second = 1.0f; g_count = 3;
    UpdateBestDist(); CHECK(g_best == 7.5f)
    g_map[0].second = -2.0f; g_map[1].second = -1.0f; g_count = 2;
    UpdateBestDist(); CHECK(g_best == 0.0f)
    std::printf("007e9250_TEST PASS\n");
    return 0;
}