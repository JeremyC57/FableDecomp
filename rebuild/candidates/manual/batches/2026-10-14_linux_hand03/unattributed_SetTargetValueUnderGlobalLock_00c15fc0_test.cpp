#include <cstdio>

static int g_locked = 0;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Owner { long* p; void Set(long v) { Lock lock; if (g_locked != 1) return; *p = v; } };
int main()
{
    long cell = 0; Owner o = { &cell }; o.Set(42); if (cell != 42 || g_locked != 0) { std::printf("00c15fc0_TEST FAIL\n"); return 1; }
    std::printf("00c15fc0_TEST PASS\n");
    return 0;
}
