// Behaviour test for CAIStateGroupBase::AIAction_MinionIdle (0x0097FA00): the window arithmetic (wrap to 2330 / 30), the
// 80% idle gate and the 9% yawn gate inside either sleep window, with the world LCG seed*0x24A1+0x24DF then ror 13.
#include <cstdio>
#include <cstring>
static unsigned int ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int Rand(unsigned int& seed, unsigned int range) { seed = seed * 0x24A1u + 0x24DFu; seed = ror13(seed); return range ? seed % range : 0; }
static bool InRange(int now, int a, int b) { return a <= b ? (now >= a && now <= b) : (now >= a || now <= b); }
struct Mi {
    bool stopping; unsigned int seed; int wake; int sleep; int now; int tracks; char anim[16];
    int Idle()
    {
        if (stopping) return 1;
        ++tracks;
        if (Rand(seed, 100) < 80) {
            int beforeWake = wake - 100; if (beforeWake < 0) beforeWake = 2330;
            int afterSleep = sleep + 100; if (afterSleep > 2400) afterSleep = 30;
            if ((InRange(now, sleep, afterSleep) || InRange(now, beforeWake, wake)) && Rand(seed, 100) < 9)
                std::strcpy(anim, "STANDARD_YAWN");
            else
                std::strcpy(anim, "ST_IDLE");
            return 2;
        }
        std::strcpy(anim, "");
        return 2;
    }
};
int main()
{
    int ok = 1;
    Mi m = { true, 1, 600, 2200, 1200, 0, "" };
    ok &= m.Idle() == 1 && m.tracks == 0;
    m.stopping = false;
    int idles = 0, yawns = 0, none = 0;
    for (int i = 0; i < 2000; ++i) {
        m.now = (i % 2) ? 1200 : 2250;          // midday (never yawns) / just after sleep time (may yawn)
        m.Idle();
        if (std::strcmp(m.anim, "ST_IDLE") == 0) ++idles;
        else if (std::strcmp(m.anim, "STANDARD_YAWN") == 0) { ++yawns; ok &= m.now == 2250; }
        else ++none;
    }
    ok &= none > 200 && none < 600 && yawns > 20 && yawns < 200 && idles > 1000;
    Mi w = { false, 7, 50, 2350, 0, 0, "" };
    int bw = w.wake - 100; if (bw < 0) bw = 2330;
    int as = w.sleep + 100; if (as > 2400) as = 30;
    ok &= bw == 2330 && as == 30;
    std::printf(ok ? "0097fa00_TEST PASS\n" : "0097fa00_TEST FAIL\n");
    return ok ? 0 : 1;
}
