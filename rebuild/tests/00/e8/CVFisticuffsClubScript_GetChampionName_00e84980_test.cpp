// Behaviour test for 0x00e84980: CV_FisticuffsClubScript::GetChampionName: the region champion definition, "An error" otherwise
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct F { long region; const char* Name(const char* n1, const char* n4, const char* n2, const char* n3) { switch (region) { case 1: return n1; case 4: return n4; case 2: return n2; case 3: return n3; } return "An error"; } };

static void run() { F f; const char* a = "CREATURE_BS_PUNCH_CLUB_CHAMPION_01"; const char* b = "CREATURE_KG_CHIEF"; const char* c = "CREATURE_OV_PUNCH_CLUB_CHAMPION_01"; const char* d = "CREATURE_BC_PUNCH_CLUB_CHAMPION_01";
  f.region = 2; CHECK(!std::strcmp(f.Name(a, b, c, d), c)); f.region = 4; CHECK(!std::strcmp(f.Name(a, b, c, d), b)); f.region = 5; CHECK(!std::strcmp(f.Name(a, b, c, d), "An error")); }

int main()
{
    run();
    if (g_fail) { std::printf("00e84980_TEST FAIL\n"); return 1; }
    std::printf("00e84980_TEST PASS\n");
    return 0;
}