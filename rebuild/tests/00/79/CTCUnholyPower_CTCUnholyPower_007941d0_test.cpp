// Behaviour test for CTCUnholyPower::CTCUnholyPower (0x007941D0): with a special-abilities component the caster's ability mutex is
// locked on the unholy-power spell (19); the spell level comes from the hero-abilities component (0 without one); the three frame
// counts are ConstantFPS times the def's seconds at that level; when the ENVIRONMENT_UNHOLY_POWER theme exists (index > 0) it is
// started on channels 0..5 except 5 with priority 2; a graphic appearance has its lighting channel saved and replaced by the
// exclusion channel (5, 5, 0xFF); a containment tracker stops updating lighting; the stamina cost per frame is split into an integer
// and a fractional part; with the effects table the build-up ring needs both ring effects (blend = (radius - small) / (large - small)),
// the hand effect makes two effects attached to the right and left hands, the dark-faces effect one more; then the start sound
// and the looping sound (kept). A log model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Def { float Secs[3], Duration[3], Env[3], Radius[3], Stamina[3], Small, Large; };
struct Caster { bool HasSpecial, HasInventory, HasAppearance, HasTracker; long Level; unsigned char Channel[3]; bool TrackerUpdates; };
struct Effects { bool Small, Large, Hand, Dark; };
struct Power {
    long SpellLevel, FramesHit, Duration, FramesEnv, StaminaModify; float PerFrameStamina, Blend;
    unsigned char Default[3], Exclusion[3]; bool Locked; long Ability; unsigned long Looping;
    void Construct(Caster& c, const Def& d, long fps, long theme, const Effects* fx)
    {
        Exclusion[0] = 5; Exclusion[1] = 5; Exclusion[2] = 0xFF; Default[0] = Default[1] = Default[2] = 0;
        SpellLevel = 0; Locked = false; Ability = 0; Blend = -1;
        if (c.HasSpecial) { Locked = true; Ability = 19; }
        if (c.HasInventory) SpellLevel = c.Level;
        FramesHit = (long)(fps * d.Secs[SpellLevel]);
        Duration = (long)(fps * d.Duration[SpellLevel]);
        FramesEnv = (long)(fps * d.Env[SpellLevel]);
        if (theme > 0)
            for (long ch = 0; ch < 6; ++ch)
                if (ch != 5) { char b[16]; std::sprintf(b, "theme%ld", ch); Log(b); }
        if (c.HasAppearance) { std::memcpy(Default, c.Channel, 3); std::memcpy(c.Channel, Exclusion, 3); }
        if (c.HasTracker) c.TrackerUpdates = false;
        float per = d.Stamina[SpellLevel] / fps;
        StaminaModify = (long)per;
        PerFrameStamina = per - StaminaModify;
        if (fx) {
            if (fx->Small && fx->Large) { Blend = (d.Radius[SpellLevel] - d.Small) / (d.Large - d.Small); Log("ring"); }
            if (fx->Hand) { Log("hand1"); Log("hand2"); Log("right"); Log("left"); }
            if (fx->Dark) Log("dark");
        }
        Log("start"); Looping = 77; Log("loop");
    }
};
int main()
{
    Def d = { { 0.5f, 1.0f, 2.0f }, { 10, 20, 30 }, { 1, 2, 3 }, { 4, 6, 8 }, { 30, 45, 60 }, 2, 10 };
    Caster c = { true, true, true, true, 1, { 1, 2, 3 }, true };
    Effects fx = { true, true, true, true };
    Power p;
    g_log[0] = 0;
    p.Construct(c, d, 30, 7, &fx);
    bool ok = p.Locked && p.Ability == 19 && p.SpellLevel == 1 && p.FramesHit == 30 && p.Duration == 600 && p.FramesEnv == 60
        && std::strcmp(g_log, "theme0;theme1;theme2;theme3;theme4;ring;hand1;hand2;right;left;dark;start;loop;") == 0
        && p.Default[0] == 1 && p.Default[2] == 3 && c.Channel[0] == 5 && c.Channel[2] == 0xFF && !c.TrackerUpdates
        && p.StaminaModify == 1 && p.PerFrameStamina == 0.5f && p.Blend == 0.5f && p.Looping == 77;
    Caster bare = { false, false, false, false, 2, { 9, 9, 9 }, true };
    Effects some = { true, false, false, true };
    g_log[0] = 0;
    p.Construct(bare, d, 30, 0, &some);
    ok = ok && !p.Locked && p.SpellLevel == 0 && p.FramesHit == 15 && std::strcmp(g_log, "dark;start;loop;") == 0
        && bare.Channel[0] == 9 && bare.TrackerUpdates && p.Blend == -1 && p.StaminaModify == 1 && p.PerFrameStamina == 0.0f;
    g_log[0] = 0;
    p.Construct(bare, d, 30, 3, 0);
    ok = ok && std::strcmp(g_log, "theme0;theme1;theme2;theme3;theme4;start;loop;") == 0;
    if (!ok) { std::printf("%s\n007941d0_TEST FAIL\n", g_log); return 1; }
    std::printf("007941d0_TEST PASS\n");
    return 0;
}