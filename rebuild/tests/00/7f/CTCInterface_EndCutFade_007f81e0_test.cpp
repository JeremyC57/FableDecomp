#include <cstdio>
#include <cstring>
#include <new>

static int g_order[2]; static int g_n; static long g_arg = -1;
struct CTCInterface {
    void ClearCutFade() { g_order[g_n++] = 1; }
    void HandleEndCutFade(long v) { g_order[g_n++] = 2; g_arg = v; }
    void EndCutFade() { ClearCutFade(); HandleEndCutFade(0); }
};
int main()
{
    CTCInterface o; o.EndCutFade();
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_arg != 0) { std::printf("007f81e0_TEST FAIL\n"); return 1; }
    std::printf("007f81e0_TEST PASS\n");
    return 0;
}