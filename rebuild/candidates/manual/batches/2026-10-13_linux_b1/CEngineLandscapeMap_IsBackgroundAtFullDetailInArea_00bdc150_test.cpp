#include <cstdio>

static long g_sum = 0;
struct CLandscapeBackgroundTreeNode { bool IsBackgroundAtFullDetailInArea(long a, long b, long c, long d) { g_sum = a + b + c + d; return false; } };
struct CEngineLandscapeMap { void* vt; CLandscapeBackgroundTreeNode* PatchTree;
    bool IsBackgroundAtFullDetailInArea(long a, long b, long c, long d) { CLandscapeBackgroundTreeNode* t = PatchTree; if (!t) return true; return t->IsBackgroundAtFullDetailInArea(a, b, c, d); } };
int main()
{
    CEngineLandscapeMap m; m.PatchTree = 0;
    if (!m.IsBackgroundAtFullDetailInArea(1, 2, 3, 4)) { std::printf("00bdc150_TEST FAIL\n"); return 1; }
    CLandscapeBackgroundTreeNode t; m.PatchTree = &t;
    if (m.IsBackgroundAtFullDetailInArea(1, 2, 3, 4) || g_sum != 10) { std::printf("00bdc150_TEST FAIL\n"); return 1; }
    std::printf("00bdc150_TEST PASS\n");
    return 0;
}
