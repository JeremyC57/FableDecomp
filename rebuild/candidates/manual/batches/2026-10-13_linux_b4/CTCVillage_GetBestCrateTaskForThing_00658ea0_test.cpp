#include <cstdio>

struct CThing { int x; };
struct Task { int y; };
static Task g_task; static void** g_list = 0;
struct V { unsigned char pad[0x60]; void* Head; bool any;
    bool IsThere() { return any; } Task* Find(CThing*, void** l) { g_list = l; return &g_task; }
    Task* Get(CThing* t) { if (!IsThere()) return 0; return Find(t, &Head); } };
int main()
{
    V v; CThing t; v.any = false; if (v.Get(&t)) { std::printf("00658ea0_TEST FAIL\n"); return 1; }
    v.any = true; if (v.Get(&t) != &g_task || g_list != &v.Head) { std::printf("00658ea0_TEST FAIL\n"); return 1; }
    std::printf("00658ea0_TEST PASS\n");
    return 0;
}
