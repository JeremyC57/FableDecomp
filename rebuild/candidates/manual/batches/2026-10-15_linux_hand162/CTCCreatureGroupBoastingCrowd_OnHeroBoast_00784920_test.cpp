#include <cstdio>
// Behaviour model of CTCCreatureGroupBoastingCrowd::OnHeroBoast (0x00784920).
static int g_fps = 30;
static int g_frame = 1000;
static float FloatRandom(float range, unsigned int* seed) { (void)seed; return range * 0.5f; }
struct Boast { bool flag; };
struct Crowd {
    int CurrentEncouragementState; bool HeroBoasted, HeroMadeGoodBoast; int FrameToLeave; int entered; unsigned int seed;
    void EnterCrowdState(int s) { entered = s; }
    void OnHeroBoast(const Boast* b)
    {
        HeroBoasted = true;
        switch (CurrentEncouragementState) { case 2: CurrentEncouragementState = 3; break; case 3: CurrentEncouragementState = 4; break; }
        int fps = g_fps;
        float delay = (FloatRandom(0.5f, &seed) + 0.5f) * fps * 10.0f;
        FrameToLeave = (int)(g_frame + delay);
        bool flagged;
        if (b && b->flag) flagged = true; else { flagged = false; HeroMadeGoodBoast = true; }
        EnterCrowdState(flagged ? 6 : 5);
    }
};
int main()
{
    Crowd c; c.CurrentEncouragementState = 2; c.HeroBoasted = false; c.HeroMadeGoodBoast = false; c.entered = 0; c.seed = 1;
    Boast b; b.flag = true;
    c.OnHeroBoast(&b);
    if (c.CurrentEncouragementState != 3 || !c.HeroBoasted || c.HeroMadeGoodBoast || c.entered != 6 || c.FrameToLeave != 1225) { std::printf("00784920_TEST FAIL\n"); return 1; }
    c.OnHeroBoast(0);
    if (c.CurrentEncouragementState != 4 || !c.HeroMadeGoodBoast || c.entered != 5) { std::printf("00784920_TEST FAIL\n"); return 1; }
    c.OnHeroBoast(0);
    if (c.CurrentEncouragementState != 4) { std::printf("00784920_TEST FAIL\n"); return 1; }
    std::printf("00784920_TEST PASS\n");
    return 0;
}
