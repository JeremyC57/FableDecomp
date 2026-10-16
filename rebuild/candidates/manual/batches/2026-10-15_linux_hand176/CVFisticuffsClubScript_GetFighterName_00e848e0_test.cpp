// Behaviour test for 0x00e848e0: CV_FisticuffsClubScript::GetFighterName: the region club member definition, "An error" otherwise
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct F { long region; const char* Name(const char* n1, const char* n4, const char* n2, const char* n3) { switch (region) { case 1: return n1; case 4: return n4; case 2: return n2; case 3: return n3; } return "An error"; } };

static void run() { F f; const char* a = "CREATURE_BS_FISTICUFFS_MEMBER"; const char* b = "CREATURE_KHG_FISTICUFFS_MEMBER"; const char* c = "CREATURE_OV_FISTICUFFS_MEMBER"; const char* d = "CREATURE_BC_FISTICUFFS_MEMBER";
  f.region = 1; CHECK(!std::strcmp(f.Name(a, b, c, d), a)); f.region = 4; CHECK(!std::strcmp(f.Name(a, b, c, d), b)); f.region = 3; CHECK(!std::strcmp(f.Name(a, b, c, d), d)); f.region = 0; CHECK(!std::strcmp(f.Name(a, b, c, d), "An error")); }

int main()
{
    run();
    if (g_fail) { std::printf("00e848e0_TEST FAIL\n"); return 1; }
    std::printf("00e848e0_TEST PASS\n");
    return 0;
}
