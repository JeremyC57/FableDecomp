// Behaviour test for CTCHeroStats::Vomit @ 0x0057B783
#include <cstdio>

struct Stats { long Vomits; float Drunk, Beer; long FrameToVomit, Deed, FrameEnd; };
static long g_fps = 30, g_frame = 1000, g_score;
static long g_minS = 2, g_maxS = 6; static float g_per = 2.0f;
static bool g_free = true, g_hasHero = true, g_hasLog = true;
static unsigned long Random(unsigned long range) { return range - 1; }   // deterministic: top of the range
static void Vomit(Stats* s, float* loopOut)
{
    float seconds = (s->Drunk - 3.0f) / g_per;
    if (g_maxS < seconds) seconds = (float)g_maxS;
    float loopTime = (float)(g_minS + Random((long)(seconds + 1.0f - g_minS)));
    *loopOut = loopTime;
    ++s->Vomits;
    g_score = s->Vomits;
    if (g_free) {
        s->FrameToVomit = -1;
        if (g_hasHero) { if (g_hasLog) s->Deed = 42; }
        long fps = g_fps;
        s->FrameEnd = (long)(g_frame + fps * loopTime);
        s->Beer = 0.0f;
    }
}
int main()
{
    Stats s; s.Vomits = 0; s.Drunk = 11.0f; s.Beer = 2.0f; s.FrameToVomit = 900; s.Deed = -1; s.FrameEnd = 0;
    float loop = 0.0f;
    Vomit(&s, &loop);   // seconds = 4 -> range 3 -> 2 + 2 = 4
    if (loop != 4.0f || s.Vomits != 1 || g_score != 1 || s.FrameToVomit != -1 || s.Deed != 42 || s.FrameEnd != 1120 || s.Beer != 0.0f) { std::printf("0057b783_TEST FAIL\n"); return 1; }
    s.Drunk = 40.0f; g_free = false; s.Beer = 1.0f; s.FrameToVomit = 5;
    Vomit(&s, &loop);   // capped at 6 -> range 5 -> 2 + 4 = 6
    if (loop != 6.0f || s.Vomits != 2 || s.FrameToVomit != 5 || s.Beer != 1.0f) { std::printf("0057b783_TEST FAIL\n"); return 1; }
    std::printf("0057b783_TEST PASS\n");
    return 0;
}
