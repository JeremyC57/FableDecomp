#include <cstdio>

static const char* g_text = 0; static long g_len = 0;
struct CCharString { void* p; CCharString(const char* t, long l) { g_text = t; g_len = l; p = 0; } };
struct CThingComponentNameTag { CCharString Name; CThingComponentNameTag() : Name("CTCTargetingPlayer", -1) {} };
int main()
{
    CThingComponentNameTag tag;
    if (!g_text || g_text[0] != 'C' || g_len != -1) { std::printf("0049d450_TEST FAIL\n"); return 1; }
    std::printf("0049d450_TEST PASS\n");
    return 0;
}
