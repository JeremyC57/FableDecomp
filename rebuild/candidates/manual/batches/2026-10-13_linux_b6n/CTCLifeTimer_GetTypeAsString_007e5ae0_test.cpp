#include <cstdio>
#include <cstring>
static const char* g_t = 0; static long g_l = 0;
struct CCharString { void* p; CCharString(const char* t, long l) { g_t = t; g_l = l; p = 0; } };
struct M { CCharString Get() { return CCharString("CTCLifeTimer", -1); } };
int main()
{
    M m; CCharString s = m.Get(); if (!g_t || g_l != -1 || std::strcmp(g_t, "CTCLifeTimer") != 0) { std::printf("007e5ae0_TEST FAIL\n"); return 1; }
    std::printf("007e5ae0_TEST PASS\n");
    return 0;
}
