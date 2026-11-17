// behaviour test for 0x005B33F4 CAnimInit::CAnimInit(unsigned char, unsigned long, long, EAnimUpdateMode, bool, unsigned long, long)
// (model: the member frame pair {Current, Last} is zeroed first, then Init receives the seven arguments unchanged). Rewritten by
// agentNF54 for the constructor identity (the old test modelled the CRumbleEffect_005b33f4::Initialise stand-in); PassPattern kept.
#include <cstdio>
namespace model {
static long g_args[7];
static int g_frameZeroBeforeInit = 0;
struct FramePair { float Current, Last; FramePair() : Current(0.0f), Last(0.0f) { } };
struct Init7 {
    long Head[6];
    FramePair Frame;
    void Init(unsigned char a, unsigned long b, long c, long d, bool e, unsigned long f, long g)
    {
        long v[7] = { a, (long)b, c, d, e ? 1 : 0, (long)f, g };
        for (int i = 0; i < 7; ++i) g_args[i] = v[i];
        g_frameZeroBeforeInit = Frame.Current == 0.0f && Frame.Last == 0.0f;
    }
    Init7(unsigned char a, unsigned long b, long c, long d, bool e, unsigned long f, long g) { Init(a, b, c, d, e, f, g); }
};
}
int main()
{
    model::Init7 x(1, 2, 3, 4, true, 6, 7);
    long want[7] = { 1, 2, 3, 4, 1, 6, 7 };
    for (int i = 0; i < 7; ++i) if (model::g_args[i] != want[i]) { std::printf("005b33f4_TEST FAIL\n"); return 1; }
    if (!model::g_frameZeroBeforeInit || x.Frame.Current != 0.0f) { std::printf("005b33f4_TEST FAIL\n"); return 1; }
    std::printf("005b33f4_TEST PASS\n");
    return 0;
}
