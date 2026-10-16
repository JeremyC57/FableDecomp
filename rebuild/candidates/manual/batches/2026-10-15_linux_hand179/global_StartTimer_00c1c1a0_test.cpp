#include <cstdio>
struct CDefaultSettings { unsigned int Words[0x18]; unsigned int Rate; unsigned int More[0x0F]; };
struct CTimerDescriptor { unsigned int F0, F4, Period, Enabled, F10; void* Owner; unsigned int F18; };
static int g_init; static CTimerDescriptor g_reg; static unsigned int g_flags; static int g_dummy;
void InitTimerGlobals() { ++g_init; }
void GetDefaultSettings(CDefaultSettings* out) { for (int i = 0; i < 0x18; ++i) out->Words[i] = 0; out->Rate = 30000; }
void PrepareDescriptor(CTimerDescriptor* d) { d->F0 = d->F4 = d->F10 = d->F18 = 0; }
void NoOp(unsigned int) {}
void* RegisterDescriptor(unsigned int flags, CTimerDescriptor* d) { g_flags = flags; g_reg = *d; return &g_dummy; }
void* StartTimer(void* owner, unsigned int unused)
{
    InitTimerGlobals();
    CDefaultSettings settings;
    GetDefaultSettings(&settings);
    CTimerDescriptor desc;
    desc.Enabled = 1;
    PrepareDescriptor(&desc);
    desc.Period = settings.Rate * 80 / 1000;
    desc.Owner = owner;
    NoOp(0);
    return RegisterDescriptor(0, &desc);
}
int main()
{
    int owner;
    void* r = StartTimer(&owner, 5);
    if (r != &g_dummy || g_init != 1 || g_reg.Period != 2400 || g_reg.Enabled != 1 || g_reg.Owner != &owner || g_flags != 0)
    { std::printf("00c1c1a0_TEST FAIL\n"); return 1; }
    std::printf("00c1c1a0_TEST PASS\n");
    return 0;
}
