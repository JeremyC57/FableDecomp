#include <cstdio>

static int g_end = 0; static float g_t = 0;
struct P { unsigned char pad[0xc]; unsigned long PoolSize; bool ok;
    bool BeginShutdown(float t) { g_t = t; return ok; } void EndShutdown() { ++g_end; }
    bool ResizePool(unsigned long n, float t) { if (!BeginShutdown(t)) return false; PoolSize = n; EndShutdown(); return true; } };
int main()
{
    P p; p.PoolSize = 1; p.ok = false; if (p.ResizePool(50, 2.0f) || p.PoolSize != 1 || g_end) { std::printf("00a02680_TEST FAIL\n"); return 1; }
    p.ok = true; if (!p.ResizePool(50, 2.0f) || p.PoolSize != 50 || g_end != 1 || g_t != 2.0f) { std::printf("00a02680_TEST FAIL\n"); return 1; }
    std::printf("00a02680_TEST PASS\n");
    return 0;
}