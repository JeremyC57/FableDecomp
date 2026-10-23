// Behaviour model of CAIStateGroup_MinionReactToNoise::IsRunnable (0x0091CDC0).
#include <cstdio>
struct V { float x, y, z; };
struct Sim {
    long frame, fps, lastInteraction; bool investigating, found;
    bool hasNoise, hasHome, priorMessage, canHear, inside; V noisePos, recorded; int posted; unsigned long duration;
};
static bool IsRunnable(Sim& s)
{
    long fps = s.fps;
    if ((float)s.frame > (float)(fps * 120.0f + s.lastInteraction)) return false;
    if (!s.investigating && s.hasNoise && s.hasHome) {
        if (!s.priorMessage && s.canHear && s.inside) {
            s.recorded = s.noisePos; ++s.posted; s.duration = (unsigned long)((float)s.fps * 2.0f);
            s.investigating = true; s.found = false;
        }
    }
    return s.investigating;
}
int main()
{
    int ok = 1;
    Sim a = { 100, 30, 0, false, true, true, true, false, true, true, { 1, 2, 3 }, { 0, 0, 0 }, 0, 0 };
    ok &= IsRunnable(a) && a.posted == 1 && a.duration == 60 && a.recorded.y == 2 && !a.found;
    Sim b = a; b.frame = 30 * 120 + 1; b.investigating = true; ok &= !IsRunnable(b);
    Sim c = a; c.investigating = false; c.posted = 0; c.priorMessage = true; ok &= !IsRunnable(c) && c.posted == 0;
    Sim d = a; d.investigating = false; d.posted = 0; d.canHear = false; ok &= !IsRunnable(d);
    Sim e = a; e.investigating = true; e.posted = 0; ok &= IsRunnable(e) && e.posted == 0;
    std::printf(ok ? "0091cdc0_TEST PASS\n" : "0091cdc0_TEST FAIL\n");
    return ok ? 0 : 1;
}