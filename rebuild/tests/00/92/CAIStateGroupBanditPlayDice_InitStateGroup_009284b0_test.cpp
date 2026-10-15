// Behaviour test for 0x009284B0: InitStateGroup clears the game and schedules join/initiate frames from two seed draws.
#include <cstdio>
static unsigned long g_seed = 12345;
static long g_frame = 1000;
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Draw(unsigned long range)
{
    g_seed = g_seed * 0x24A1 + 0x24DF; g_seed &= 0xFFFFFFFFUL;
    g_seed = ROR13(g_seed) & 0xFFFFFFFFUL;
    return g_seed % range;
}
struct CDice { void* DiceGame; long FrameToInitiate; long FrameToJoin; bool WasCarryingWeapon;
    void Init()
    {
        DiceGame = 0; WasCarryingWeapon = false;
        unsigned long joinDelay = Draw(3000);
        FrameToJoin = (long)(g_frame + joinDelay * 0.5f);
        FrameToInitiate = Draw(3000) + g_frame;
    }
};
int main()
{
    CDice d; d.DiceGame = &d; d.WasCarryingWeapon = true;
    unsigned long s0 = g_seed;
    d.Init();
    // recompute expected
    g_seed = s0; unsigned long a = Draw(3000), b = Draw(3000);
    if (d.DiceGame != 0 || d.WasCarryingWeapon || d.FrameToJoin != (long)(1000 + a * 0.5f) || d.FrameToInitiate != (long)(b + 1000)
        || d.FrameToJoin < 1000 || d.FrameToJoin > 2500 || d.FrameToInitiate < 1000 || d.FrameToInitiate >= 4000) {
        std::printf("009284b0_TEST FAIL\n"); return 1;
    }
    std::printf("009284b0_TEST PASS\n");
    return 0;
}