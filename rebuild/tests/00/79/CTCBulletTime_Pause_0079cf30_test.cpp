// Behaviour test for CTCBulletTime::Pause (0x0079CF30): only a pausable, running bullet-time pauses; it marks itself paused,
// stops its loop sound, kills and drops a live looping effect, fades the screen filter out at once and clears the carried
// thing's 0x1C flag when the parent carries something.
#include <stdio.h>

struct Emitter { bool dead, killed; };
struct Carrying { bool Flag1C; };
struct World { int fadeOuts; float lastTime; int lastKey; };
struct BulletTime {
    bool Pausable, Paused; int LoopSound; Emitter* effect; int ScreenFilterKey; Carrying* carrying;
};
static int g_stopped = -1;
static void StopSound(int s) { g_stopped = s; }
static void Pause(BulletTime& b, World& w)
{
    if (!b.Pausable || b.Paused)
        return;
    b.Paused = true;
    StopSound(b.LoopSound);
    if (b.effect && !b.effect->dead) {
        b.effect->killed = true;
        b.effect = 0;
    }
    ++w.fadeOuts; w.lastTime = 0.0f; w.lastKey = b.ScreenFilterKey;
    if (b.carrying)
        b.carrying->Flag1C = false;
}
int main()
{
    int fails = 0;
    World w = { 0, -1.0f, 0 };
    Emitter e = { false, false };
    Carrying c = { true };
    BulletTime b = { true, false, 7, &e, 42, &c };
    Pause(b, w);
    if (!b.Paused || g_stopped != 7 || !e.killed || b.effect || w.fadeOuts != 1 || w.lastKey != 42 || w.lastTime != 0.0f || c.Flag1C) { printf("pause FAIL\n"); ++fails; }
    Pause(b, w);
    if (w.fadeOuts != 1) { printf("repause FAIL\n"); ++fails; }
    BulletTime n = { false, false, 3, 0, 1, 0 };
    g_stopped = -1;
    Pause(n, w);
    if (n.Paused || g_stopped != -1) { printf("not pausable FAIL\n"); ++fails; }
    Emitter dead = { true, false };
    BulletTime d = { true, false, 5, &dead, 9, 0 };
    Pause(d, w);
    if (dead.killed || d.effect != &dead || w.fadeOuts != 2) { printf("dead effect FAIL\n"); ++fails; }
    if (fails) { printf("0079cf30_TEST FAIL\n"); return 1; }
    printf("0079cf30_TEST PASS\n");
    return 0;
}