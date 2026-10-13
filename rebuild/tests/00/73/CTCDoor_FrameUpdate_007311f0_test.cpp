#include <cstdio>

static int g_anim = 0, g_p = 0, g_s = 0;
struct Def { unsigned char pad[0x28]; long TriggerType; };
struct D { unsigned char pad[0x20]; Def* def;
    void UpdateAnimation() { ++g_anim; } void P() { ++g_p; } void S() { ++g_s; }
    void FrameUpdate() { UpdateAnimation(); long t = def->TriggerType; if (t == 0) { P(); return; } if (t == 1) S(); } };
int main()
{
    Def df; D d; d.def = &df; df.TriggerType = 0; d.FrameUpdate(); df.TriggerType = 1; d.FrameUpdate(); df.TriggerType = 5; d.FrameUpdate();
    if (g_anim != 3 || g_p != 1 || g_s != 1) { std::printf("007311f0_TEST FAIL\n"); return 1; }
    std::printf("007311f0_TEST PASS\n");
    return 0;
}