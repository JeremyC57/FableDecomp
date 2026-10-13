#include <cstdio>

static const char* g_text = 0; static long g_len = 0;
struct CCharString { void* p; CCharString(const char* t, long l) { g_text = t; g_len = l; p = 0; } };
struct CAIStateGroupNameTag { CCharString Name; CAIStateGroupNameTag() : Name("SG_MINION_HAWKING_FROM_STALL", -1) {} };
int main()
{
    CAIStateGroupNameTag tag;
    if (!g_text || g_text[0] != 'S' || g_len != -1) { std::printf("008fdd10_TEST FAIL\n"); return 1; }
    std::printf("008fdd10_TEST PASS\n");
    return 0;
}