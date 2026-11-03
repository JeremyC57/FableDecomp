#include <cstdio>
#include <cstring>
// Model of CEngineStateBlockAdditiveAlpha::Apply: the render-state sequence it requests through the inline CRenderStateManager setters
// (source / dest blend ONE / ONE, blending on, stage 0 colour op MODULATE and alpha op MODULATE with texture x diffuse
// arguments, stage 1 colour and alpha ops disabled), and the deferred-state rule each setter follows (only a changed value
// is recorded, once per bookmark).
struct State { unsigned long desired; unsigned long mask; int dirty; };
static State g_states[16]; static unsigned long g_bookmark = 1; static int g_pushes = 0;
static void Set(int slot, unsigned long v)
{
    State& s = g_states[slot];
    if (s.desired != v) {
        if ((s.mask & g_bookmark) == 0) { s.mask |= g_bookmark; ++g_pushes; }
        s.desired = v; s.dirty = 1;
    }
}
enum { SRC, DST, BLEND, OP0, AOP0, ARG10, ARG20, AARG10, AARG20, OP1, AOP1 };
static const char* g_log[16]; static int g_n = 0;
static void Rec(const char* s) { if (g_n < 16) g_log[g_n] = s; ++g_n; }
static void Apply()
{
    Set(SRC, 1); Rec("SRC_ONE"); Set(DST, 2); Rec("DST_ONE"); Set(BLEND, 1); Rec("BLEND_ON");
    Set(OP0, 3); Rec("OP0_MODULATE"); Set(AOP0, 3); Rec("AOP0_MODULATE");
    Set(ARG10, 2); Rec("ARG1_TEXTURE"); Set(ARG20, 0); Rec("ARG2_DIFFUSE");
    Set(AARG10, 2); Rec("AARG1_TEXTURE"); Set(AARG20, 0); Rec("AARG2_DIFFUSE");
    Set(OP1, 1); Rec("OP1_DISABLE"); Set(AOP1, 1); Rec("AOP1_DISABLE");
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 16; ++i) { g_states[i].desired = 0xffffffff; g_states[i].mask = 0; g_states[i].dirty = 0; }
    Apply();
    static const char* exp[11] = { "SRC_ONE", "DST_ONE", "BLEND_ON", "OP0_MODULATE", "AOP0_MODULATE", "ARG1_TEXTURE", "ARG2_DIFFUSE",
        "AARG1_TEXTURE", "AARG2_DIFFUSE", "OP1_DISABLE", "AOP1_DISABLE" };
    if (g_n != 11) bad = 1;
    for (int i = 0; i < 11 && i < g_n; ++i) if (std::strcmp(g_log[i], exp[i]) != 0) bad = 1;
    if (g_pushes != 11) bad = 1;
    int before = g_pushes; g_n = 0;
    Apply();                                               // same values again: nothing new is recorded
    if (g_pushes != before || g_n != 11) bad = 1;
    if (bad) { std::printf("00b57d20_TEST FAIL\n"); return 1; }
    std::printf("00b57d20_TEST PASS\n");
    return 0;
}
