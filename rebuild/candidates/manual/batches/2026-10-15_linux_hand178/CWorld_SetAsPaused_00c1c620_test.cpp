#include <cstdio>
#include <cstring>
static int g_states[4]; static int g_n = 0; static int g_refresh = 0; static int g_guard = 0;
struct Guard { Guard() { ++g_guard; } ~Guard() { --g_guard; } };
struct W { bool Paused;
  void SetAsPaused(bool p) { if (Paused != p) { Paused = p; Guard g; g_states[g_n++] = Paused ? 1 : 4; ++g_refresh; } } };
static bool Run() { W w; w.Paused = false; w.SetAsPaused(false); if (g_n) return false; w.SetAsPaused(true); w.SetAsPaused(false); return g_n == 2 && g_states[0] == 1 && g_states[1] == 4 && g_refresh == 2 && g_guard == 0; }
int main()
{
    if (!Run()) { std::printf("00c1c620_TEST FAIL\n"); return 1; }
    std::printf("00c1c620_TEST PASS\n");
    return 0;
}
