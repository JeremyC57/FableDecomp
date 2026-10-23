// Behaviour test for CTCHeroStats::FrameUpdate @ 0x0057D5A0 (stamina/fatness/speed bookkeeping)
#include <cstdio>

struct Stats {
    float Fatness; long Stamina, MaxStamina, StaminaFrames, HealthFrames; bool Override; long FatFrames, SpeedFrames;
    long RegionTime[4];
};
static long g_region = 2, g_frame = 60, g_fps = 30, g_health, g_hasteRemoved, g_drunk;
static bool g_hasBlocker = false, g_block = false, g_has93 = false;
static void FrameUpdate(Stats* s)
{
    long region = g_region;
    if (region >= 0 && (unsigned long)region < 4u) ++s->RegionTime[region];
    --s->FatFrames;
    if (s->FatFrames <= 0) { s->Fatness = s->Fatness - 0.5f; s->FatFrames = 100; }
    if (!s->Override) {
        if (!(g_hasBlocker && (g_block || g_has93))) {
            --s->StaminaFrames;
            if (s->StaminaFrames <= 0) {
                long fps = g_fps;
                if (g_frame % fps == 0) s->Stamina += 5;
                if (s->Stamina < 0) s->Stamina = 0; else if (s->Stamina > s->MaxStamina) s->Stamina = s->MaxStamina;
                s->StaminaFrames = 0;
            }
        }
    }
    --s->HealthFrames;
    if (s->HealthFrames <= 0) { g_health += 3; s->HealthFrames = (long)(g_fps * 2.0f); }
    long& speed = s->SpeedFrames;
    if (speed > 0 && --speed <= 0) ++g_hasteRemoved;
    ++g_drunk;
}
int main()
{
    Stats s; s.Fatness = 2.0f; s.Stamina = 98; s.MaxStamina = 100; s.StaminaFrames = 1; s.HealthFrames = 1; s.Override = false;
    s.FatFrames = 1; s.SpeedFrames = 1; for (int i = 0; i < 4; ++i) s.RegionTime[i] = 0;
    FrameUpdate(&s);
    if (s.RegionTime[2] != 1 || s.Fatness != 1.5f || s.FatFrames != 100 || s.Stamina != 100 || s.StaminaFrames != 0 ||
        g_health != 3 || s.HealthFrames != 60 || g_hasteRemoved != 1 || s.SpeedFrames != 0 || g_drunk != 1) { std::printf("0057d5a0_TEST FAIL\n"); return 1; }
    g_hasBlocker = true; g_has93 = true; s.Stamina = 10; g_region = 7;
    FrameUpdate(&s);
    if (s.Stamina != 10 || s.StaminaFrames != 0 || g_hasteRemoved != 1 || s.RegionTime[2] != 1) { std::printf("0057d5a0_TEST FAIL\n"); return 1; }
    g_has93 = false; FrameUpdate(&s);
    if (s.Stamina != 15) { std::printf("0057d5a0_TEST FAIL\n"); return 1; }
    std::printf("0057d5a0_TEST PASS\n");
    return 0;
}
