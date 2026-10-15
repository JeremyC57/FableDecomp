// Behaviour test for CTCOnHeroPush::vfunc_9 @ 0x007BBA70: removal once frames since the push exceed 5 * fps.
#include <cstdio>
static long g_framesPerSecond = 2;
struct CThing { int removed; long type; void RemoveTC(long t) { ++removed; type = t; } };
struct CTCOnHeroPush {
    CThing* ParentThing;
    long FrameSincePushed;
    void vfunc_9()
    {
        static long s_maxFramesSincePushed = g_framesPerSecond * 5;
        ++FrameSincePushed;
        if (FrameSincePushed > s_maxFramesSincePushed) ParentThing->RemoveTC(0xc4);
    }
};
int main()
{
    CThing t; t.removed = 0; t.type = 0;
    CTCOnHeroPush p; p.ParentThing = &t; p.FrameSincePushed = 0;
    for (int i = 0; i < 10; ++i) p.vfunc_9();
    if (t.removed != 0 || p.FrameSincePushed != 10) { std::printf("007bba70_TEST FAIL\n"); return 1; }
    g_framesPerSecond = 100;                          // the static is initialised once only
    p.vfunc_9();
    if (t.removed != 1 || t.type != 0xc4) { std::printf("007bba70_TEST FAIL\n"); return 1; }
    std::printf("007bba70_TEST PASS\n");
    return 0;
}