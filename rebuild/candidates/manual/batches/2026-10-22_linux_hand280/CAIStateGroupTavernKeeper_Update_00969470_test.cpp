// Behaviour test for CAIStateGroup_TavernKeeper::Update (0x00969470): the keeper walks into the pub when outside (a failed walk terminates), then loops serving payments, refilling empty jug slots or idling (the random step seed*0x24A1+0x24DF rotated right by 13, low bit picks do-nothing or lean) until a step fails.
#include <cstdio>
static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
struct CKeeper {
    bool inside, walkOk, terminated;
    int payments, refills, leans, idles, steps;
    unsigned int seed;
    int emptySlots;
    void Update()
    {
        if (!inside && !walkOk) { terminated = true; return; }
        for (;;) {
            if (++steps > 20) { terminated = true; return; }
            if (payments > 0) { --payments; continue; }
            if (emptySlots > 0) { --emptySlots; ++refills; continue; }
            unsigned int next = seed * 0x24A1U + 0x24DFU;
            next &= 0xFFFFFFFFU;
            seed = Ror13(next) & 0xFFFFFFFFU;
            switch (seed % 2) {
            case 0: ++idles; if (idles == 3) { terminated = true; return; } break;
            case 1: ++leans; break;
            }
        }
    }
};
int main()
{
    CKeeper k = { false, false, false, 0, 0, 0, 0, 0, 1, 0 };
    k.Update();
    bool ok = k.terminated && k.steps == 0;
    CKeeper k2 = { true, false, false, 2, 0, 0, 0, 0, 12345, 3 };
    k2.Update();
    ok = ok && k2.terminated && k2.payments == 0 && k2.refills == 3 && (k2.idles == 3 || k2.steps > 20);
    unsigned int s = 1; s = Ror13((s * 0x24A1U + 0x24DFU) & 0xFFFFFFFFU) & 0xFFFFFFFFU;
    ok = ok && s == Ror13(0x4980U);
    if (!ok) { std::printf("00969470_TEST FAIL\n"); return 1; }
    std::printf("00969470_TEST PASS\n");
    return 0;
}
