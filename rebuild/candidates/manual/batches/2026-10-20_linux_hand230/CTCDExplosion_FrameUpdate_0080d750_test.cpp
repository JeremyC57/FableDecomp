// Behaviour test for CTCDExplosion::FrameUpdate (0x0080D750): dynamic/static update, emitter pruning and the kill rule.
#include <cstdio>

static int g_dyn = 0, g_static = 0, g_hits = 0, g_impulse = 0, g_kills = 0;
struct Expl {
    bool dynamic, dynFinished, needTrigger, createdEmitter; long timer; int emitters; int deadEmitters;
    void FrameUpdate()
    {
        if (dynamic) ++g_dyn;
        else if (needTrigger) { ++g_static; ++g_hits; needTrigger = false; }
        ++g_impulse;
        emitters -= deadEmitters; deadEmitters = 0;
        if (dynamic) { if (dynFinished) ++g_kills; return; }
        if (createdEmitter) { if (emitters != 0) return; }
        else if (--timer != 0) return;
        ++g_kills;
    }
};

int main()
{
    Expl e = { false, false, true, false, 2, 0, 0 };
    e.FrameUpdate();
    if (g_static != 1 || e.needTrigger || g_kills != 0 || e.timer != 1) { std::printf("0080d750_TEST FAIL\n"); return 1; }
    e.FrameUpdate();
    if (g_static != 1 || g_kills != 1) { std::printf("0080d750_TEST FAIL\n"); return 1; }
    Expl c = { false, false, false, true, 5, 2, 1 };
    c.FrameUpdate();
    if (g_kills != 1 || c.emitters != 1) { std::printf("0080d750_TEST FAIL\n"); return 1; }
    c.deadEmitters = 1; c.FrameUpdate();
    if (g_kills != 2) { std::printf("0080d750_TEST FAIL\n"); return 1; }
    Expl d = { true, true, false, false, 1, 0, 0 };
    d.FrameUpdate();
    if (g_dyn != 1 || g_kills != 3) { std::printf("0080d750_TEST FAIL\n"); return 1; }
    std::printf("0080d750_TEST PASS\n");
    return 0;
}
