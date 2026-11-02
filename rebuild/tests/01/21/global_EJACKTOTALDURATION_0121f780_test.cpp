// Behaviour test for 0x0121F780: JACK_TOTAL_DURATION = glow frames (0x01388A7C, 60) + fade-down frames (0x01388A80, 40).
#include <cstdio>
long g_01388a7c = 60;
long g_01388a80 = 40;
namespace {
long JACK_TOTAL_DURATION = g_01388a7c + g_01388a80;
}
int main()
{
    if (JACK_TOTAL_DURATION != 100) { std::printf("0121f780_TEST FAIL\n"); return 1; }
    std::printf("0121f780_TEST PASS\n");
    return 0;
}