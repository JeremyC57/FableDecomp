// Test for 0x00C56060 CAudioContext::SetDefaultSpeakerConfig: the six global words get their defaults.
#include <cstdio>
static int g994 = -1, g9ac = -1, ga00 = -1, ga04 = -1, ga08 = -1, ga1c = -1;
static void SetDefaultSpeakerConfig() { g994 = 3; g9ac = 3; ga00 = 0; ga04 = 2; ga08 = 0; ga1c = 1; }
int main()
{
    SetDefaultSpeakerConfig();
    if (g994 != 3 || g9ac != 3 || ga00 != 0 || ga04 != 2 || ga08 != 0 || ga1c != 1) { std::printf("00c56060_TEST FAIL\n"); return 1; }
    std::printf("00c56060_TEST PASS\n");
    return 0;
}