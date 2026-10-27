#include <cstdio>
// Behaviour model: registration happens only when the scheduler component exists.
static int registered = -1;
static void init(bool hasScheduler) { if (hasScheduler) registered = 4; }
int main()
{
    init(false); if (registered != -1) { std::printf("00938f20_TEST FAIL\n"); return 1; }
    init(true); if (registered != 4) { std::printf("00938f20_TEST FAIL\n"); return 1; }
    std::printf("00938f20_TEST PASS\n");
    return 0;
}