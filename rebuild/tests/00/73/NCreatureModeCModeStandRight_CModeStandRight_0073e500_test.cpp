#include <cstdio>
#include <cstring>
// Behaviour model: the filter receives the three group names in declaration order; the temporaries die afterwards.
static const char* g_got[3];
static int g_live;
struct Str { const char* s; Str(const char* t) : s(t) { ++g_live; } ~Str() { --g_live; } };
struct Filter { Filter(const Str& a, const Str& b, const Str& c) { g_got[0] = a.s; g_got[1] = b.s; g_got[2] = c.s; } };
struct Mode { Filter f; Mode() : f(Str("STAND_RIGHT"), Str("NOT_CARRYING"), Str("STAND")) { } };
int main()
{
    Mode m;
    if (std::strcmp(g_got[0], "STAND_RIGHT") || std::strcmp(g_got[1], "NOT_CARRYING") || std::strcmp(g_got[2], "STAND") || g_live != 0) { std::printf("0073e500_TEST FAIL\n"); return 1; }
    std::printf("0073e500_TEST PASS\n");
    return 0;
}