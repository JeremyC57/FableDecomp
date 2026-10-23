// Behaviour test for CTCHeroStats::ProcessDrunkenness @ 0x0057CB7E
#include <cstdio>

struct Stats { float Drunk, Beer; long FrameToVomit, Deed, FrameEnd; };
static long g_frame = 100, g_fps = 30, g_vomits, g_removed = 7;
static float g_max = 5.0f, g_step = 0.5f, g_rate = 1.0f, g_drain = 3.0f, g_vomitSober = 6.0f;
static bool g_movie;
static void Process(Stats* s)
{
    if (g_movie) return;
    s->Drunk -= g_rate * g_step * 24.0f / g_fps;
    if (s->Beer > 0.0f) {
        float drain = g_drain / g_fps;
        if (drain > s->Beer) { s->Drunk += s->Beer; s->Beer = 0.0f; }
        else { s->Drunk += drain; s->Beer -= drain; }
    }
    if (g_max < s->Drunk && s->FrameToVomit == -1 && s->Deed == -1) {
        long fps = g_fps;
        s->FrameToVomit = (2 + 1) * fps + g_frame;
    }
    if (s->FrameToVomit != -1 && s->FrameToVomit < g_frame) { ++g_vomits; s->FrameToVomit = -1; }
    if (s->FrameEnd != 0) s->Drunk -= g_vomitSober / g_fps;
    if (s->Drunk < 0.0f) s->Drunk = 0.0f;
    if (s->Deed != -1 && g_frame > s->FrameEnd) { s->Deed = -1; s->FrameEnd = 0; g_removed = s->Deed; }
}
int main()
{
    Stats s; s.Drunk = 6.0f; s.Beer = 0.05f; s.FrameToVomit = -1; s.Deed = -1; s.FrameEnd = 0;
    Process(&s);
    if (s.Beer != 0.0f || s.FrameToVomit != 190 || s.Drunk < 5.64f || s.Drunk > 5.66f) { std::printf("0057cb7e_TEST FAIL\n"); return 1; }
    g_frame = 200; s.Deed = 4; s.FrameEnd = 150; Process(&s);
    if (g_vomits != 1 || s.Deed != -1 || s.FrameEnd != 0 || g_removed != -1) { std::printf("0057cb7e_TEST FAIL\n"); return 1; }
    s.Drunk = 0.1f; s.FrameEnd = 500; s.Deed = 2; Process(&s);
    if (s.Drunk != 0.0f || s.Deed != 2) { std::printf("0057cb7e_TEST FAIL\n"); return 1; }
    g_movie = true; s.Drunk = 3.0f; Process(&s);
    if (s.Drunk != 3.0f) { std::printf("0057cb7e_TEST FAIL\n"); return 1; }
    std::printf("0057cb7e_TEST PASS\n");
    return 0;
}
