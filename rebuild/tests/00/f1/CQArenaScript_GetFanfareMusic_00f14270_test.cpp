// Behaviour test for 0x00f14270: GetFanfareMusic: round to track table.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long F(long r) { switch (r) { case 0: return 0x23; case 1: return 0x24; case 2: return 0x25; case 3: return 0x26; case 4: return 0x27; case 5: return 0x28; case 6: return 0x29; case 7: return 0x2a; case 8: return 0x2b; case 9: return 0x2c; } return 0x23; }
static int run() { CHECK(F(0) == 0x23 && F(1) == 0x24 && F(9) == 0x2c && F(10) == 0x23 && F(-1) == 0x23 && F(5) == 0x28); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00f14270_TEST FAIL\n"); return 1; }
    std::printf("00f14270_TEST PASS\n");
    return 0;
}