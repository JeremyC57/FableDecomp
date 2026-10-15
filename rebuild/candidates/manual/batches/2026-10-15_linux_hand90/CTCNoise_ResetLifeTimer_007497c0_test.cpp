#include <cstdio>
// Behaviour of CTCNoise::ResetLifeTimer (0x007497C0): LifeTimer = (long)(fps * LifeTime + 5), the world accessor called once.
long g_FramesPerSecond = 15;
static int g_worldCalls = 0;
struct CTCNoiseDef_Retail { long Type; float LifeTime; float DistanceCanBeHeardFrom; };
struct CTCNoise_Retail {
    CTCNoiseDef_Retail Definition;
    long LifeTimer;
    void* GetWorld() { ++g_worldCalls; return 0; }
    void ResetLifeTimer()
    {
        GetWorld();
        LifeTimer = (long)(g_FramesPerSecond * Definition.LifeTime + 5.0f);
    }
};
int main()
{
    CTCNoise_Retail n;
    n.Definition.LifeTime = 2.0f; n.LifeTimer = 0;
    n.ResetLifeTimer();
    if (n.LifeTimer != 35 || g_worldCalls != 1) { std::printf("007497c0_TEST FAIL\n"); return 1; }
    n.Definition.LifeTime = 0.5f;
    n.ResetLifeTimer();
    if (n.LifeTimer != 12) { std::printf("007497c0_TEST FAIL\n"); return 1; }
    std::printf("007497c0_TEST PASS\n");
    return 0;
}
