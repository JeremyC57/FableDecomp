// Behaviour test for 0x0121F7A0: JACK_GLOW_START_FRAME = JACK_TOTAL_DURATION - fade-down frames (initializers run in definition order).
#include <cstdio>
long g_01388a7c = 60;
long g_01388a80 = 40;
namespace {
long JACK_TOTAL_DURATION = g_01388a7c + g_01388a80;
long JACK_GLOW_START_FRAME = JACK_TOTAL_DURATION - g_01388a80;
}
int main()
{
    if (JACK_TOTAL_DURATION != 100 || JACK_GLOW_START_FRAME != 60) { std::printf("0121f7a0_TEST FAIL\n"); return 1; }
    std::printf("0121f7a0_TEST PASS\n");
    return 0;
}