// Behaviour test for 0x008DEC80 NCreatureMode::CModeJackOfBladesFlash::FrameUpdate: models the per-frame decisions -- the timer counts
// down and flashes below the flash-frame threshold; at the glow start frame the glow theme fades in over glow-fade-frames / 30
// seconds and the sword glow / spot light are created; at zero the fade theme (0.75 s), the kills, the strike-out animation and the
// mode removal end the frame; otherwise the spot light angle is acos(clamp(dir . (0,0,-1))) / (2 pi), and the mode is dropped when the
// boss is ready for its phase change.
#include <stdio.h>
#include <math.h>

enum { GLOW_START = 1, FADE_END = 2, NORMAL = 3 };
struct SFrame { int Branch; bool Flashed; float FadeSeconds; };
static SFrame Step(long& timer, long flashFrames, long glowStart, long glowFadeFrames)
{
    SFrame f = { NORMAL, false, 0.0f };
    if (--timer < flashFrames) f.Flashed = true;
    if (timer == glowStart) { f.Branch = GLOW_START; f.FadeSeconds = (float)glowFadeFrames * (1.0f / 30.0f); }
    else if (timer == 0) { f.Branch = FADE_END; f.FadeSeconds = 0.75f; }
    return f;
}
static float SpotAngle(float dz)
{
    float c = dz * -1.0f;
    if (c < -1.0f) c = -1.0f; else if (c > 1.0f) c = 1.0f;
    return (float)acos(c) * 0.15915494f;
}

int main()
{
    int ok = 1;
    long timer = 40;
    SFrame a = Step(timer, 10, 30, 15);
    ok &= timer == 39 && a.Branch == NORMAL && !a.Flashed;
    timer = 31;
    SFrame b = Step(timer, 10, 30, 15);
    ok &= b.Branch == GLOW_START && fabs(b.FadeSeconds - 0.5f) < 1e-6f;
    timer = 1;
    SFrame c = Step(timer, 10, 30, 15);
    ok &= c.Branch == FADE_END && c.Flashed && c.FadeSeconds == 0.75f;
    ok &= fabs(SpotAngle(-1.0f)) < 1e-6f;                 // looking straight down: angle 0
    ok &= fabs(SpotAngle(0.0f) - 0.25f) < 1e-6f;          // horizontal: a quarter turn
    ok &= fabs(SpotAngle(-2.0f)) < 1e-6f;                 // clamped
    if (ok) printf("008dec80_TEST PASS\n"); else printf("008dec80_TEST FAIL\n");
    return ok ? 0 : 1;
}
