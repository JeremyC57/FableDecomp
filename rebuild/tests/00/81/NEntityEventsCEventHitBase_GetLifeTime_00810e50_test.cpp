// Behaviour test for 0x00810E50: hit event lifetime = 2 * frames per second
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static long g_framesPerSecond = 30;
struct SEvent { long GetLifeTime() const { return 2 * g_framesPerSecond; } };
int main()
{
    SEvent e; CHECK(e.GetLifeTime() == 60);
    g_framesPerSecond = 25; CHECK(e.GetLifeTime() == 50);
    if (!g_fail) printf("00810e50_TEST PASS\n");
    return g_fail;
}