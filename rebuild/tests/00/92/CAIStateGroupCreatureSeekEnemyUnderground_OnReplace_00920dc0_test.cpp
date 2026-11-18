// Behaviour test for CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace (0x00920DC0): with a live enemy the creature's physics is
// reset to RisePos, faced along the flat direction to the enemy (angle in turns from +Y towards +X, wrapped into [0, 1)) and given the
// RH set (up Z, forward = that unit direction); a missing or dead enemy skips all that; the rise-up action is set in every case.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Thing { bool dead; V pos; };
struct Physics { int resets, faces, rhsets; V reset_pos; float angle; V up, fwd; };
struct Creature { Physics phys; int rise_actions; };
struct State {
    Creature* creature; Thing* enemy; V RisePos;
    void IdleOnReplace() { ++creature->rise_actions; }
    void OnReplace()
    {
        if (enemy != 0 && !enemy->dead) {
            creature->phys.resets++; creature->phys.reset_pos = RisePos;
            V d = { enemy->pos.X - RisePos.X, enemy->pos.Y - RisePos.Y, 0.0f };
            float m = std::sqrt(d.X * d.X + d.Y * d.Y); d.X /= m; d.Y /= m;
            float a = (float)std::atan2(d.X, d.Y) * 0.15915494f;
            if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; }
            creature->phys.faces++; creature->phys.angle = a;
            V up = { 0.0f, 0.0f, 1.0f }; creature->phys.rhsets++; creature->phys.up = up; creature->phys.fwd = d;
        }
        IdleOnReplace();
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 2e-3f; }
static int fail(int n) { std::printf("00920dc0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Creature c = { { 0, 0, 0, { 0, 0, 0 }, 0, { 0, 0, 0 }, { 0, 0, 0 } }, 0 };
    Thing e = { false, { 5.0f, 0.0f, 9.0f } };
    State s = { &c, &e, { 1.0f, 0.0f, 2.0f } };
    s.OnReplace();                                                         // enemy due +X: a quarter turn
    if (c.phys.resets != 1 || c.phys.reset_pos.Z != 2.0f || !near(c.phys.angle, 0.25f) || !near(c.phys.fwd.X, 1.0f) || c.phys.fwd.Z != 0.0f) return fail(1);
    if (c.phys.up.Z != 1.0f || c.rise_actions != 1) return fail(2);
    e.pos.X = -3.0f; e.pos.Y = 0.0f; s.OnReplace();                        // due -X: atan2 gives -0.25, wrapped to 0.75
    if (!near(c.phys.angle, 0.75f) || c.rise_actions != 2) return fail(3);
    e.dead = true; s.OnReplace();                                          // dead: only the rise
    if (c.phys.resets != 2 || c.rise_actions != 3) return fail(4);
    s.enemy = 0; s.OnReplace();                                            // none: only the rise
    if (c.phys.resets != 2 || c.rise_actions != 4) return fail(5);
    std::printf("00920dc0_TEST PASS\n");
    return 0;
}