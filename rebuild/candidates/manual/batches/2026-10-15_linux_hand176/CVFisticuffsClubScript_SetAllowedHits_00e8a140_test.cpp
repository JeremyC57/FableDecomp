// Behaviour test for 0x00e8a140: CV_FisticuffsClubScript::SetAllowedHits: base minus hits/fights scaled, truncated; unknown region takes the hits
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_def[16];
struct F { long region; long fights; long hits;
  void Set(long h) { long base; switch (region) { case 1: base = g_def[0]; break; case 4: base = g_def[3]; break; case 2: base = g_def[1]; break; case 3: base = g_def[2]; break; default: hits = h; return; } hits = (long)(base - (float)h / fights * g_def[4]); } };
static void run() { g_def[0] = 10; g_def[1] = 20; g_def[2] = 30; g_def[3] = 40; g_def[4] = 2; F f; f.fights = 4; f.region = 1; f.Set(6); CHECK(f.hits == 7); f.region = 4; f.Set(4); CHECK(f.hits == 38); f.region = 0; f.Set(9); CHECK(f.hits == 9); }

int main()
{
    run();
    if (g_fail) { std::printf("00e8a140_TEST FAIL\n"); return 1; }
    std::printf("00e8a140_TEST PASS\n");
    return 0;
}
