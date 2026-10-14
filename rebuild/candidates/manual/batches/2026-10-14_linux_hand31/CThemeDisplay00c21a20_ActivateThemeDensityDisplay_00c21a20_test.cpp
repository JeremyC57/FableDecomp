#include <cstdio>
static long g_dispatch = -1, g_state = -1, g_process = -1; static int g_order = 0, g_od = 0, g_os = 0, g_op = 0;
static void SetStateValue(long v) { g_state = v; g_os = ++g_order; }
struct CThemeDisplay { void Dispatch(long s) { g_dispatch = s; g_od = ++g_order; } void Process(long a) { g_process = a; g_op = ++g_order; }
    void ActivateThemeDensityDisplay(long arg) { Dispatch(2); SetStateValue(arg); Process(arg); } };
int main() { CThemeDisplay t; t.ActivateThemeDensityDisplay(9); if (g_dispatch != 2 || g_state != 9 || g_process != 9 || g_od != 1 || g_os != 2 || g_op != 3) { std::printf("00c21a20_TEST FAIL\n"); return 1; } std::printf("00c21a20_TEST PASS\n"); return 0; }
