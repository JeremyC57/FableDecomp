// behaviour test for CAIStateGroup_BanditKingThrowWeapons::IsRunnable (0x00961550)
#include <cstdio>
struct V { float X, Y, Z; };
struct CBoss { bool finalPhase, ready; };
struct CThing { V pos; bool dead; CBoss* boss; };
struct Group {
    CThing* creature; CThing* hero; CThing* PTargetThing; float R;
    bool IsRunnable()
    {
        if (hero && !hero->dead && creature->boss) {
            CBoss* b = creature->boss;
            if (!b->finalPhase && b->ready) {
                float dx = hero->pos.X - creature->pos.X, dy = hero->pos.Y - creature->pos.Y;
                float d = dx * dx; d += dy * dy;
                if (d > R) { PTargetThing = hero; return true; }
            }
        }
        return false;
    }
};
int main()
{
    CBoss b = { false, true }; CThing c = { { 0, 0, 0 }, false, &b }; CThing h = { { 30, 40, 0 }, false, 0 };
    Group g = { &c, &h, 0, 2000.0f };
    bool ok = g.IsRunnable() && g.PTargetThing == &h;
    g.PTargetThing = 0; g.R = 2500.0f; ok = ok && !g.IsRunnable();
    g.R = 10.0f; b.finalPhase = true; ok = ok && !g.IsRunnable();
    b.finalPhase = false; h.dead = true; ok = ok && !g.IsRunnable() && g.PTargetThing == 0;
    if (!ok) { std::printf("00961550_TEST FAIL\n"); return 1; }
    std::printf("00961550_TEST PASS\n");
    return 0;
}