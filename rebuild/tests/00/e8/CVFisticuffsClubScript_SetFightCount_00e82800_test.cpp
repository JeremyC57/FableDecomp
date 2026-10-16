// Behaviour test for 0x00e82800: CV_FisticuffsClubScript::SetFightCount: the region fight counts, Bowerstone for unknown regions
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_def[16];
struct F { long region; long fights; long hits;
  void Set() { switch (region) { case 1: fights = g_def[5]; return; case 4: fights = g_def[8]; return; case 2: fights = g_def[6]; return; case 3: fights = g_def[7]; return; } fights = g_def[5]; } };
static void run() { g_def[5] = 3; g_def[6] = 4; g_def[7] = 5; g_def[8] = 6; F f; f.region = 4; f.Set(); CHECK(f.fights == 6); f.region = 2; f.Set(); CHECK(f.fights == 4); f.region = 3; f.Set(); CHECK(f.fights == 5); f.region = 9; f.Set(); CHECK(f.fights == 3); f.region = 1; f.Set(); CHECK(f.fights == 3); }

int main()
{
    run();
    if (g_fail) { std::printf("00e82800_TEST FAIL\n"); return 1; }
    std::printf("00e82800_TEST PASS\n");
    return 0;
}