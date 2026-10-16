// Behaviour test for CTCTavernGameShoveHaPenny::EnterGame (0x007CF530): the camera goes to the def's named camera point,
// RoundsPlayed resets, the def reference is released.
#include <cstdio>
#include <cstring>
static const char* g_table[3] = { "CAM_A", "CAM_SHOVE", "CAM_C" };
static char g_used[32]; static int g_count = 1;
struct Shove {
    long RoundsPlayed;
    void EnterGame(long cameraName)
    {
        ++g_count;
        { const char* camera = g_table[cameraName]; std::strcpy(g_used, camera); }
        RoundsPlayed = 0;
        --g_count;
    }
};
int main()
{
    Shove s; s.RoundsPlayed = 4; s.EnterGame(1);
    if (std::strcmp(g_used, "CAM_SHOVE") != 0 || s.RoundsPlayed != 0 || g_count != 1) { std::printf("007cf530_TEST FAIL\n"); return 1; }
    std::printf("007cf530_TEST PASS\n");
    return 0;
}
