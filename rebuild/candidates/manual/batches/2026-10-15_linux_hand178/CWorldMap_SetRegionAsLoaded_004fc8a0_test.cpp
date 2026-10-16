#include <cstdio>
#include <cstring>
static char g_log[8]; static int g_n = 0; static long g_initRegion = -1;
static void Progress(const char* t) { g_log[g_n++] = t[std::strlen(t) - 1]; }
struct WM { long LoadedRegion; long regions[4];
  void SetRegionAsLoaded(long r) { LoadedRegion = r; Progress("SetRegionAsLoaded: Initialise MiniMap"); g_initRegion = regions[LoadedRegion]; Progress("SetRegionAsLoaded: Initialise MiniMap End"); } };
static bool Run() { WM w; for (int i = 0; i < 4; ++i) w.regions[i] = i * 11; w.SetRegionAsLoaded(2); g_log[g_n] = 0; return w.LoadedRegion == 2 && g_initRegion == 22 && std::strcmp(g_log, "pd") == 0; }
int main()
{
    if (!Run()) { std::printf("004fc8a0_TEST FAIL\n"); return 1; }
    std::printf("004fc8a0_TEST PASS\n");
    return 0;
}
