#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardSorting::vfunc_1 (0x007D56D0).
struct CPersistContext {
    int calls; const char* names[4]; long* values[4]; long defs[4]; long baseArg;
    void Transfer(const char* n, long& v, const long& d) { names[calls] = n; values[calls] = &v; defs[calls] = d; ++calls; }
};
static int g_base = 0;
struct CTCTavernGameCardBase {
    unsigned char _pad[0x318];
    void vfunc_1(CPersistContext* context, long arg) { ++g_base; context->baseArg = arg; }
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    long m_timeLimitNumRoundsToWin;
    long m_timeLimitBestNumRounds;
    void vfunc_1(CPersistContext* context, long arg);
};
void CTCTavernGameCardSorting::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer("TimeLimitNumRoundsToWin", m_timeLimitNumRoundsToWin, 0);
    context->Transfer("TimeLimitBestNumRounds", m_timeLimitBestNumRounds, 0);
}
int main()
{
    static CTCTavernGameCardSorting g;
    CPersistContext c; c.calls = 0; c.baseArg = 0; c.defs[0] = c.defs[1] = 9;
    g.vfunc_1(&c, 11);
    if (g_base != 1 || c.baseArg != 11 || c.calls != 2
        || std::strcmp(c.names[0], "TimeLimitNumRoundsToWin") != 0 || c.values[0] != &g.m_timeLimitNumRoundsToWin || c.defs[0] != 0
        || std::strcmp(c.names[1], "TimeLimitBestNumRounds") != 0 || c.values[1] != &g.m_timeLimitBestNumRounds || c.defs[1] != 0)
    { std::printf("007d56d0_TEST FAIL\n"); return 1; }
    std::printf("007d56d0_TEST PASS\n");
    return 0;
}
