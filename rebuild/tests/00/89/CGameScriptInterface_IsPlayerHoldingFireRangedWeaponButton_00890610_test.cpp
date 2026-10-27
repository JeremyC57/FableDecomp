#include <cstdio>
// Behaviour model: the query forwards the action, button state 2 and the player's device.
static int lastAction, lastType; static long lastDevice;
static bool queued(int a, int t, long dev) { lastAction = a; lastType = t; lastDevice = dev; return a == 0xA; }
int main()
{
    if (!queued(0xA, 2, 3) || lastType != 2 || lastDevice != 3) { std::printf("00890610_TEST FAIL\n"); return 1; }
    std::printf("00890610_TEST PASS\n");
    return 0;
}