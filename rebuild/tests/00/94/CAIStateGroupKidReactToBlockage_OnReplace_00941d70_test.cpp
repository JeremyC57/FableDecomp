// Behaviour test for CAIStateGroup_KidReactToBlockage::OnReplace (0x00941D70): LastEndState takes the current frame,
// CurrentActivationMinInterval = fps*5 + (LCG/rotate random % fps*10), and the world seed advances.
#include <cstdio>

long g_framesPerSecond = 30;
static long g_frame = 1234;
static unsigned long g_seed = 0x12345678UL;
static unsigned long GFROR13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
static CWorld g_world;
class CThingAICreature { public:};

class CAIStateGroupBase { public:
    unsigned long* GetPWorldSeed() { return &g_seed; }
    CWorld* GetPThingWorld() { return &g_world; }
    unsigned long RandomFramesWithin(float seconds)
    {
        long framesPerSecond = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = (long)(framesPerSecond * seconds);
        unsigned long next = (*seed * 0x24A1U + 0x24DFU) & 0xFFFFFFFFUL;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
};

// Independent expectation: the same LCG + rotate, range = fps * 10, interval = fps * 5 + r.
static long Expected(unsigned long seedBefore, unsigned long* seedAfter)
{
    unsigned long next = (seedBefore * 9377UL + 9439UL) & 0xFFFFFFFFUL;
    unsigned long rot = ((next >> 13) | (next << 19)) & 0xFFFFFFFFUL;
    *seedAfter = rot;
    return 150 + (long)(rot % 300);
}

struct CAIStateGroup_KidReactToBlockage : CAIStateGroupBase {
    long LastEndState, CurrentActivationMinInterval;
    void OnReplace()
    {
        LastEndState = GetPThingWorld()->GetFrame();
        CurrentActivationMinInterval = (long)(g_framesPerSecond * 5.0f + (float)RandomFramesWithin(10.0f));
    }
};

int main()
{
    CThingAICreature c; (void)c;
    CAIStateGroup_KidReactToBlockage g;
    bool ok = true;
    for (int i = 0; i < 50; ++i) {
        unsigned long after;
        long expect = Expected(g_seed, &after);
        g_frame = 1000 + i;
        g.OnReplace();
        if (g.LastEndState != 1000 + i || g.CurrentActivationMinInterval != expect || g_seed != after
            || expect < 150 || expect >= 450) ok = false;
    }
    if (!ok) { std::printf("00941d70_TEST FAIL\n"); return 1; }
    std::printf("00941d70_TEST PASS\n");
    return 0;
}