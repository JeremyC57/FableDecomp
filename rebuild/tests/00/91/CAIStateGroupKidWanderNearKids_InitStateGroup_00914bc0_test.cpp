#include <cstdio>
// Behaviour model: registration happens only when the scheduler component exists.
static int registered = -1;
static void init(bool hasScheduler) { if (hasScheduler) registered = 11; }
int main()
{
    init(false); if (registered != -1) { std::printf("00914bc0_TEST FAIL\n"); return 1; }
    init(true); if (registered != 11) { std::printf("00914bc0_TEST FAIL\n"); return 1; }
    std::printf("00914bc0_TEST PASS\n");
    return 0;
}