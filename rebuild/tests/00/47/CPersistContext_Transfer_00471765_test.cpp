// behaviour test for 0x00471765: mode 2 loads through the traits, mode 3 writes the value, other modes do nothing
#include <cstdio>
static int g_loads = 0; static long g_written = -1;
struct Ctx { int Mode; void Transfer(int& v) { if (Mode == 2) { ++g_loads; v = 5; } else if (Mode == 3) g_written = v; } };
int main()
{
    int v = 9; Ctx c; c.Mode = 3; c.Transfer(v); c.Mode = 2; c.Transfer(v); c.Mode = 1; c.Transfer(v);
    if (g_written != 9 || g_loads != 1 || v != 5) { std::printf("00471765_TEST FAIL\n"); return 1; }
    std::printf("00471765_TEST PASS\n");
    return 0;
}