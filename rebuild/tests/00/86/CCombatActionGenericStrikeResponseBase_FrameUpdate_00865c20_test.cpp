#include <cstdio>
#include <cmath>
struct Creature { int type; float vx, vy, vz; bool particles; };
static int g_dust = 0, g_finish = 0, g_base = 0;
struct Action {
    Creature* creature; long frame; long dustFrame; bool createDust; bool finishWhenMoving;
    void FrameUpdate()
    {
        ++g_base;
        if (frame == dustFrame) {
            if (creature->particles && createDust) ++g_dust;
        }
        if (finishWhenMoving) {
            if (creature->type == 2 && (float)std::sqrt(creature->vx * creature->vx + creature->vy * creature->vy + creature->vz * creature->vz) > 0.1f)
                ++g_finish;
        }
    }
};
int main()
{
    Creature c = { 2, 0.3f, 0.0f, 0.0f, true };
    Action a = { &c, 5, 5, true, true };
    a.FrameUpdate();                        // dust + finish
    a.frame = 6; c.vx = 0.05f; a.FrameUpdate();   // neither
    c.type = 1; c.vx = 1.0f; a.FrameUpdate();     // not a creature type 2
    a.frame = 5; a.createDust = false; a.FrameUpdate();
    if (g_base != 4 || g_dust != 1 || g_finish != 1) { std::printf("00865c20_TEST FAIL\n"); return 1; }
    std::printf("00865c20_TEST PASS\n");
    return 0;
}