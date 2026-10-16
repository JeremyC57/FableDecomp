#include <cstdio>
// Behaviour model of CTCTavernGameCardPairs::vfunc_53 (0x007D6260): the OK box shows the def's title (+0xA0) and body (+0xF8)
// text ids and leads to state 0x17.
struct Def { unsigned long title, body; };
static unsigned long g_t, g_b; static long g_state;
static void Show(const Def& d) { g_t = d.title; g_b = d.body; g_state = 0x17; }
int main()
{
    Def d = { 101, 202 };
    Show(d);
    if (g_t != 101 || g_b != 202 || g_state != 0x17) { std::printf("007d6260_TEST FAIL\n"); return 1; }
    std::printf("007d6260_TEST PASS\n");
    return 0;
}
