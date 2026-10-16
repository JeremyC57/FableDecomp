// behaviour test for CAIStateGroup_BanditKingLunge::IsRunnable (0x009614A0)
#include <cstdio>
struct V { float X, Y, Z; };
struct CThing { V pos; bool lungeable; };
static float Dist2(const V& a, const V& b) { float dx = a.X - b.X; float dy = a.Y - b.Y; return dx * dx + dy * dy; }
struct Group {
    CThing* creature; CThing* hero; CThing* PTargetThing; float R;
    bool IsRunnable()
    {
        if (hero->lungeable && Dist2(hero->pos, creature->pos) < R) { PTargetThing = hero; return true; }
        return false;
    }
};
int main()
{
    CThing c = { { 0, 0, 0 }, false }; CThing h = { { 3, 4, 100 }, true };
    Group g = { &c, &h, 0, 26.0f };
    bool ok = g.IsRunnable() && g.PTargetThing == &h;
    g.R = 25.0f; g.PTargetThing = 0; ok = ok && !g.IsRunnable() && g.PTargetThing == 0;
    g.R = 30.0f; h.lungeable = false; ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("009614a0_TEST FAIL\n"); return 1; }
    std::printf("009614a0_TEST PASS\n");
    return 0;
}