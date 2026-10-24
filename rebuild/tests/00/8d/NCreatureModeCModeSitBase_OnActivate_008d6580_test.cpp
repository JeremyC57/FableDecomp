#include <cstdio>
// Behaviour test for NCreatureMode::CModeSitBase::OnActivate (retail 0x008D6580): base activation, then the idle counter is
// seeded with minFrames + random % range, where minFrames = 0.75 x the mode's combination-idle delay and range is
// -minFrames - (long)(delay x -1.5) (the retail arithmetic), using the world generator seed = ROR13(seed * 0x24A1 + 0x24DF).
struct CRandomSeed { unsigned long Seed; };
unsigned long GFROR13(unsigned long value) { value &= 0xffffffffUL; return ((value >> 13) | (value << 19)) & 0xffffffffUL; }
static CRandomSeed g_seed;
struct CWorld {
    CRandomSeed* GetRandomSeed() { return &g_seed; }
};
static CWorld g_world;
static int g_baseActivations;
static long g_delay;
struct CModeBase {
    unsigned long Type;
    unsigned long GetType() const { return Type; }
    CWorld* GetWorld() const { return &g_world; }
    long GetNoFramesBeforePlayingCombinationIdleAnims(unsigned long mode) { return mode == 7 ? g_delay : -1; }
    void OnActivate() { ++g_baseActivations; }
};
inline unsigned long GFRandom(CRandomSeed* random)
{
    random->Seed = (random->Seed * 0x24a1 + 0x24df) & 0xffffffffUL;
    random->Seed = GFROR13(random->Seed);
    return random->Seed;
}
struct CModeSitBase : CModeBase {
    long IdleCounter;
    void OnActivate()
    {
        CModeBase::OnActivate();
        float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims(GetType());
        long minFrames = (long)(frames * 0.75f);
        CRandomSeed* random = GetWorld()->GetRandomSeed();
        unsigned int range = (unsigned int)(-minFrames - (long)(frames * -1.5f));
        unsigned int r = (unsigned int)GFRandom(random);
        IdleCounter = (long)((range ? r % range : 0) + (unsigned int)minFrames);
    }
};

int main()
{
    CModeSitBase m;
    m.Type = 7; m.IdleCounter = -5;
    g_delay = 200;
    g_seed.Seed = 1;
    g_baseActivations = 0;
    m.OnActivate();
    unsigned long expectSeed = GFROR13(1 * 0x24a1 + 0x24df);
    int pass = g_baseActivations == 1 && g_seed.Seed == expectSeed && m.IdleCounter == (long)(150 + expectSeed % 150);
    pass = pass && m.IdleCounter >= 150 && m.IdleCounter < 300;
    g_delay = 0;
    m.OnActivate();
    pass = pass && m.IdleCounter == 0 && g_baseActivations == 2;
    if (!pass) { std::printf("008d6580_TEST FAIL\n"); return 1; }
    std::printf("008d6580_TEST PASS\n");
    return 0;
}