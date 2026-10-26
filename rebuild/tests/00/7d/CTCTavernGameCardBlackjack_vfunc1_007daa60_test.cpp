#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardBlackjack::vfunc_1 (0x007DAA60).
class CPersistContext { public:
    int calls; const char* name; long* value; long def; long baseArg;
    void Transfer_00410620(const char* n, long& v, const long& d) { ++calls; name = n; value = &v; def = d; }
};
static int g_base = 0;
struct CTCTavernGameCardBase {
    unsigned char _pad[0x318];
    void vfunc_1(CPersistContext* context, long arg) { ++g_base; context->baseArg = arg; }
};
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    unsigned char _pad_0x318[0x30];
    long m_mostMoneyWon;
    void vfunc_1(CPersistContext* context, long arg);
};
void CTCTavernGameCardBlackjack::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer_00410620("MostMoneyWon", m_mostMoneyWon, 0);
}
int main()
{
    static CTCTavernGameCardBlackjack g;
    CPersistContext c; c.calls = 0; c.def = 99; c.baseArg = 0;
    g.vfunc_1(&c, 7);
    if (g_base != 1 || c.baseArg != 7 || c.calls != 1 || std::strcmp(c.name, "MostMoneyWon") != 0
        || c.value != &g.m_mostMoneyWon || c.def != 0) { std::printf("007daa60_TEST FAIL\n"); return 1; }
    std::printf("007daa60_TEST PASS\n");
    return 0;
}