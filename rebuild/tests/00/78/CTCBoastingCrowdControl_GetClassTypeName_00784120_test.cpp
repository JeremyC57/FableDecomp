#include <cstdio>
#include <cstring>
static const char* g_t = 0; static long g_l = 0;
class CCharString { public: void* p; CCharString(const char* t, long l) { g_t = t; g_l = l; p = 0; } };
struct M { CCharString Get() { return CCharString("CTCBoastingCrowdControl", -1); } };
int main()
{
    M m; CCharString s = m.Get(); if (!g_t || g_l != -1 || std::strcmp(g_t, "CTCBoastingCrowdControl") != 0) { std::printf("00784120_TEST FAIL\n"); return 1; }
    std::printf("00784120_TEST PASS\n");
    return 0;
}