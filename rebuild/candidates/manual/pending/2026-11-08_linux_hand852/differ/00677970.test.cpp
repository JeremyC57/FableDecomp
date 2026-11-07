// Behaviour test for CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn (0x00677970): `.Set(reaction, Type(args))` fills
// LowHigh[0][reaction] with the traits as read and LowHigh[1][reaction] with the high-frequency copy: Linear times (x fps) and
// x0.2; PeriodicFrequency (a*0.2, b*0.2, c*5, d*2); ControlledCount (gap*5, recharge*0.2, max*0.2), the optional flag kept.
#include <cstdio>

struct Traits { int Kind; float A, B, C, D; bool Flag; };
static const long FPS = 30;

static void Linear(float minWait, float range, Traits& low, Traits& high)
{
    unsigned long fps = FPS;
    low.Kind = 0; low.A = minWait * fps; low.B = range * fps;
    high.Kind = 0; high.A = minWait * fps * 0.2f; high.B = range * fps * 0.2f;
}
static void Periodic(float a, float b, float c, float d, Traits& low, Traits& high)
{
    low.Kind = 1; low.A = a; low.B = b; low.C = c; low.D = d;
    high.Kind = 1; high.A = a * 0.2f; high.B = b * 0.2f; high.C = c * 5.0f; high.D = d * 2.0f;
}
static void Controlled(long gap, float recharge, float maxCount, bool flag, Traits& low, Traits& high)
{
    low.Kind = 2; low.A = (float)gap; low.B = recharge; low.C = maxCount; low.Flag = flag;
    high.Kind = 2; high.A = (float)(gap * 5); high.B = recharge * 0.2f; high.C = maxCount * 0.2f; high.Flag = flag;
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }

int main()
{
    Traits lo, hi;
    Linear(2.0f, 1.0f, lo, hi);
    bool ok = Near(lo.A, 60.0f) && Near(hi.A, 12.0f) && Near(hi.B, 6.0f);
    Periodic(1.0f, 2.0f, 3.0f, 4.0f, lo, hi);
    ok = ok && Near(hi.A, 0.2f) && Near(hi.B, 0.4f) && Near(hi.C, 15.0f) && Near(hi.D, 8.0f);
    Controlled(10, 5.0f, 10.0f, true, lo, hi);
    ok = ok && Near(hi.A, 50.0f) && Near(hi.B, 1.0f) && Near(hi.C, 2.0f) && hi.Flag && lo.Flag;
    if (!ok) { std::printf("00677970_TEST FAIL\n"); return 1; }
    std::printf("00677970_TEST PASS\n");
    return 0;
}
