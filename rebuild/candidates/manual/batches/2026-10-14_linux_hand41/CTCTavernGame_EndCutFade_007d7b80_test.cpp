#include <cstdio>
#include <cstring>
#include <new>

static int g_order[2]; static int g_n; static long g_arg = -1;
struct CTCTavernGame {
    void ResetPlayback() { g_order[g_n++] = 1; }
    void SetState(long v) { g_order[g_n++] = 2; g_arg = v; }
    void EndCutFade() { ResetPlayback(); SetState(0x27); }
};
int main()
{
    CTCTavernGame o; o.EndCutFade();
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_arg != 0x27) { std::printf("007d7b80_TEST FAIL\n"); return 1; }
    std::printf("007d7b80_TEST PASS\n");
    return 0;
}
