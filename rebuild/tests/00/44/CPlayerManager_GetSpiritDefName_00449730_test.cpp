// Behaviour test for 0x00449730: GetSpiritDefName: slot to spirit def name.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
#include <cstring>
// Expected values come from the retail jump table 0x004497A4 (entries 0x449741 ONE, 0x449789 TWO, 0x449759 THREE, 0x449771 FOUR).
static const char* F(long p) { switch (p) { case 0: return "COOP_SPIRIT_PLAYER_ONE"; case 2: return "COOP_SPIRIT_PLAYER_THREE"; case 3: return "COOP_SPIRIT_PLAYER_FOUR"; } return "COOP_SPIRIT_PLAYER_TWO"; }
static int run() { CHECK(!std::strcmp(F(0), "COOP_SPIRIT_PLAYER_ONE") && !std::strcmp(F(1), "COOP_SPIRIT_PLAYER_TWO") && !std::strcmp(F(2), "COOP_SPIRIT_PLAYER_THREE") && !std::strcmp(F(3), "COOP_SPIRIT_PLAYER_FOUR") && !std::strcmp(F(7), "COOP_SPIRIT_PLAYER_TWO")); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00449730_TEST FAIL\n"); return 1; }
    std::printf("00449730_TEST PASS\n");
    return 0;
}