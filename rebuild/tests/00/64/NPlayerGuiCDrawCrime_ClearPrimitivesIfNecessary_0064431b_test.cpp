// behaviour test for 0x0064431B: the bar is cleared only when not drawing and present
#include <cstdio>
static int g_clears = 0;
struct Bar { void Clear() { ++g_clears; } };
static void Run(bool drawing, Bar* bar) { if (!drawing && bar) bar->Clear(); }
int main()
{
    Bar b; Run(false, &b); Run(true, &b); Run(false, 0);
    if (g_clears != 1) { std::printf("0064431b_TEST FAIL\n"); return 1; }
    std::printf("0064431b_TEST PASS\n");
    return 0;
}