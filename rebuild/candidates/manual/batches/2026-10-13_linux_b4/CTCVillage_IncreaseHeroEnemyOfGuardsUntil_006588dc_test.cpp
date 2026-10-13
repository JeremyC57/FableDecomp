#include <cstdio>

static long g_now = 0;
struct W { static long GetCurrentFrame() { return g_now; } };
static W g_w;
template<class T> inline const T& MaxOf(const T& a, const T& b) { return (a < b) ? b : a; }
struct V { unsigned char pad[0xd4]; long Until; W* GetWorld() { return &g_w; }
    void Inc(long f) { long now = GetWorld()->GetCurrentFrame(); Until = MaxOf(now, Until) + f; } };
int main()
{
    V v; v.Until = 100; g_now = 50; v.Inc(10); if (v.Until != 110) { std::printf("006588dc_TEST FAIL\n"); return 1; }
    g_now = 200; v.Inc(5); if (v.Until != 205) { std::printf("006588dc_TEST FAIL\n"); return 1; }
    std::printf("006588dc_TEST PASS\n");
    return 0;
}
