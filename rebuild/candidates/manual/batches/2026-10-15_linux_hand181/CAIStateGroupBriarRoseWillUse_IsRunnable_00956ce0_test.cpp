// behaviour test for CAIStateGroup_BriarRoseWillUse::IsRunnable (0x00956CE0)
#include <cstdio>
struct CThing { bool dead; bool hasTC8; CThing* target; };
static long g_frame;
struct Group {
    CThing* creature; CThing* PTargetThing; long NextUseFrame;
    bool IsRunnable()
    {
        if (g_frame >= NextUseFrame && creature->hasTC8) {
            CThing* t = creature->target;
            if (t && !t->dead) { PTargetThing = t; return true; }
        }
        return false;
    }
};
int main()
{
    CThing t = { false, false, 0 }; CThing c = { false, true, &t };
    Group g = { &c, 0, 10 };
    g_frame = 5; bool ok = !g.IsRunnable() && g.PTargetThing == 0;
    g_frame = 10; ok = ok && g.IsRunnable() && g.PTargetThing == &t;
    t.dead = true; g.PTargetThing = 0; ok = ok && !g.IsRunnable() && g.PTargetThing == 0;
    if (!ok) { std::printf("00956ce0_TEST FAIL\n"); return 1; }
    std::printf("00956ce0_TEST PASS\n");
    return 0;
}
