#include <cstdio>
#include <new>
// Behaviour test for 00f0d490: push_back copy-constructs at finish and advances while capacity remains, else hands
// (finish, x, 1, true) to _M_insert_overflow.
struct TeleportData { int defName; unsigned int seed; float health; int scriptName; int items[3];
    TeleportData() : defName(0), seed(0), health(0), scriptName(0) {}
    TeleportData(const TeleportData& o) : defName(o.defName), seed(o.seed), health(o.health), scriptName(o.scriptName) {} };
static int g_overflows = 0; static unsigned int g_fill = 0; static bool g_atEnd = false; static const TeleportData* g_x = 0;
struct Vec {
    TeleportData* start; TeleportData* finish; TeleportData* end;
    void insert_overflow(TeleportData* pos, const TeleportData& x, unsigned int fill, bool atEnd) { (void)pos; ++g_overflows; g_fill = fill; g_atEnd = atEnd; g_x = &x; }
    void push_back(const TeleportData& x)
    {
        if (finish != end) { new (finish) TeleportData(x); ++finish; }
        else insert_overflow(finish, x, 1, true);
    }
};
int main()
{
    TeleportData buf[2];
    Vec v; v.start = v.finish = buf; v.end = buf + 2;
    TeleportData a; a.defName = 5; a.seed = 77; a.health = 0.5f; a.scriptName = 9;
    v.push_back(a); v.push_back(a);
    int ok = v.finish == buf + 2 && buf[1].seed == 77 && buf[0].scriptName == 9 && g_overflows == 0;
    v.push_back(a);
    ok = ok && g_overflows == 1 && g_fill == 1 && g_atEnd && g_x == &a && v.finish == buf + 2;
    if (!ok) { std::printf("00f0d490_TEST FAIL\n"); return 1; }
    std::printf("00f0d490_TEST PASS\n");
    return 0;
}