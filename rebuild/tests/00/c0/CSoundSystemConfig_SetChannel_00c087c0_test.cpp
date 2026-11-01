#include <cstdio>
#include <cstdlib>
#include <cstring>
class CMainGameComponent; static void* g_window = 0; static bool g_flag = false; static CMainGameComponent* g_owner = 0; static long g_context = -1;
static void BeginLoading(void* window, bool flag, CMainGameComponent* owner, long context) { g_window = window; g_flag = flag; g_owner = owner; g_context = context; }
class CMainGameComponent { public: void BeginInputLoading(void* window, bool flag) { BeginLoading(window, flag, this, 0); } };

int main()
{
    CMainGameComponent c; int w; c.BeginInputLoading(&w, true); if (g_window != &w || !g_flag || g_owner != &c || g_context != 0) { std::printf("00c087c0_TEST FAIL\n"); return 1; } c.BeginInputLoading(0, false); if (g_window || g_flag) { std::printf("00c087c0_TEST FAIL\n"); return 1; }
    std::printf("00c087c0_TEST PASS\n");
    return 0;
}