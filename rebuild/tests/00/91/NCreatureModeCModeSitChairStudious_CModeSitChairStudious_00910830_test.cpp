#include <cstdio>
#include <cstring>
// Behaviour model: the filter receives the three group names in declaration order; the temporaries die afterwards.
static const char* g_got[3];
static int g_live;
struct Str { const char* s; Str(const char* t) : s(t) { ++g_live; } ~Str() { --g_live; } };
struct Filter { Filter(const Str& a, const Str& b, const Str& c) { g_got[0] = a.s; g_got[1] = b.s; g_got[2] = c.s; } };
struct Mode { Filter f; Mode() : f(Str("SIT_CHAIR"), Str("SIT_CHAIR_NORMAL"), Str("SIT_CHAIR_STUDIOUS")) { } };
int main()
{
    Mode m;
    if (std::strcmp(g_got[0], "SIT_CHAIR") || std::strcmp(g_got[1], "SIT_CHAIR_NORMAL") || std::strcmp(g_got[2], "SIT_CHAIR_STUDIOUS") || g_live != 0) { std::printf("00910830_TEST FAIL\n"); return 1; }
    std::printf("00910830_TEST PASS\n");
    return 0;
}