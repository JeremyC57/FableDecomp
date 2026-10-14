#include <cstdio>
#include <cstring>
#include <new>

static int g_order[2]; static int g_n; static long g_arg = -1;
struct CThingCreature {
    void NotifyManager() { g_order[g_n++] = 1; }
    void OnPropertyChanged(long v) { g_order[g_n++] = 2; g_arg = v; }
    void EndCutFade() { NotifyManager(); OnPropertyChanged(0x104); }
};
int main()
{
    CThingCreature o; o.EndCutFade();
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_arg != 0x104) { std::printf("0082fe00_TEST FAIL\n"); return 1; }
    std::printf("0082fe00_TEST PASS\n");
    return 0;
}
