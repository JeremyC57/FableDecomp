#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardPairs::vfunc_1 (0x007D6830).
class CPersistContext { public:
    int calls; const char* name; float* value; float def; long baseArg;
    void Transfer(const char* n, float& v, const float& d) { ++calls; name = n; value = &v; def = d; }
};
static int g_base = 0;
struct CTCTavernGameCardBase {
    unsigned char _pad[0x98];
    float TargetTime;
    unsigned char _pad_0x9c[0x27c];
    void vfunc_1(CPersistContext* context, long arg) { ++g_base; context->baseArg = arg; }
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    void vfunc_1(CPersistContext* context, long arg);
};
void CTCTavernGameCardPairs::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer("TargetTime", TargetTime, 0.0f);
}
int main()
{
    static CTCTavernGameCardPairs g;
    CPersistContext c; c.calls = 0; c.def = 5.0f; c.baseArg = 0;
    g.vfunc_1(&c, 3);
    if (g_base != 1 || c.baseArg != 3 || c.calls != 1 || std::strcmp(c.name, "TargetTime") != 0
        || c.value != &g.TargetTime || c.def != 0.0f) { std::printf("007d6830_TEST FAIL\n"); return 1; }
    std::printf("007d6830_TEST PASS\n");
    return 0;
}