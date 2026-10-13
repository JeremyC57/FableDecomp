#include <cstdio>

struct G { unsigned char pad[0x2c]; long CrowdState;
    long Is() { long s = CrowdState; if (s == 0 || s == 1 || s == 7 || s == 8 || s == 9 || s == 10) return 1; return 0; } };
int main()
{
    G g; long yes[6] = { 0, 1, 7, 8, 9, 10 }; long no[4] = { 2, 3, 6, 11 };
    for (int i = 0; i < 6; ++i) { g.CrowdState = yes[i]; if (g.Is() != 1) { std::printf("007841c0_TEST FAIL\n"); return 1; } }
    for (int j = 0; j < 4; ++j) { g.CrowdState = no[j]; if (g.Is() != 0) { std::printf("007841c0_TEST FAIL\n"); return 1; } }
    std::printf("007841c0_TEST PASS\n");
    return 0;
}
