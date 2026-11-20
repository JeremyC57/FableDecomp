// Behaviour test for 0x009BE380 CDisplayManager::CalcPresentationIntervalTime (links the REAL row object).
// The row reads neither this nor any global: `fild vsync_count ; fidiv refresh_rate`, the quotient returned on the x87 stack.
// Reference model (independent): the quotient of the two integers taken in double precision (the CRT's default x87 precision
//   control, which is what the row's fidiv runs under) and then rounded once to float, the type the caller stores.
// Checks: hand-written pairs (exact, inexact, negative, large, 60000 / 1001) + 20000 seeded random pairs, bit patterns compared;
//   this is passed as null, as a valid block and as an odd pointer (it must not matter); refresh_rate is never zero.
#include <stdio.h>
#include <string.h>

class CDisplayManager
{
public:
    static float Call(void* m, long refresh_rate, long vsync_count) { return ((CDisplayManager*)m)->CalcPresentationIntervalTime(refresh_rate, vsync_count); }
private:
    float CalcPresentationIntervalTime(long refresh_rate, long vsync_count);
};

typedef unsigned long UL;
static UL g_seed = 0x9BE380u;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static int g_fail, g_cases;
static unsigned char g_block[64];

static UL Bits(float f) { UL u; memcpy(&u, &f, 4); return u; }
static void Run(long refresh_rate, long vsync_count)
{
    ++g_cases;
    volatile double num = (double)vsync_count; volatile double den = (double)refresh_rate;
    volatile float want = (float)(num / den);
    void* thises[3] = { 0, (void*)g_block, (void*)(g_block + 7) };
    volatile float got = CDisplayManager::Call(thises[g_cases % 3], refresh_rate, vsync_count);
    if (Bits(got) != Bits(want)) { if (g_fail < 5) printf("  case %d: %ld / %ld -> %08lX, want %08lX\n", g_cases, vsync_count, refresh_rate, Bits(got), Bits(want)); ++g_fail; }
}

int main()
{
    static const long pairs[][2] = {
        { 60, 1 }, { 60, 2 }, { 75, 1 }, { 85, 3 }, { 1, 60 }, { 1001, 60000 }, { 4, 2 }, { 2, 4 }, { 3, 1 }, { 7, 22 }, { -60, 1 }, { 60, -1 },
        { -3, -1 }, { 1, 0 }, { 59, 0 }, { 0x7FFFFFFF, 1 }, { 1, 0x7FFFFFFF }, { (long)0x80000000, 1 }, { 1, (long)0x80000000 },
        { 0x7FFFFFFF, 0x7FFFFFFE }, { 16777217, 16777219 }, { 3, 16777217 }, { 100, 1 }, { 120, 1 }, { 50, 4 }
    };
    for (int i = 0; i < (int)(sizeof(pairs) / sizeof(pairs[0])); ++i) Run(pairs[i][0], pairs[i][1]);
    for (int n = 0; n < 20000; ++n)
    {
        long r = (long)((Rnd() << 8) ^ Rnd()), v = (long)((Rnd() << 8) ^ Rnd());
        if (n & 1) { r = (long)(Rnd() % 240) + 1; v = (long)(Rnd() % 5); }
        if (r == 0) r = 60;
        Run(r, v);
    }
    if (g_fail) { printf("009be380_TEST FAIL (%d of %d cases)\n", g_fail, g_cases); return 1; }
    printf("009be380_TEST PASS (%d cases)\n", g_cases);
    return 0;
}