#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

static const char* g_text = 0; static long g_len = 0;
class CCharString { public: void* p; CCharString(const char* t, long l) { g_text = t; g_len = l; p = 0; } };
struct CAIStateGroupNameTag { CCharString Name; CAIStateGroupNameTag() : Name("SG_MINION_HAWKING_FROM_STALL", -1) {} };
int main()
{
    CAIStateGroupNameTag tag;
    if (!RetailTextEquals(g_text, "SG_MINION_HAWKING_FROM_STALL") || g_len != -1) { std::printf("008fdd10_TEST FAIL\n"); return 1; }
    std::printf("008fdd10_TEST PASS\n");
    return 0;
}