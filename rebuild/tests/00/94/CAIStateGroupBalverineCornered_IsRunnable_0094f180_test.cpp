// Behaviour test for CAIStateGroup_BalverineCornered::IsRunnable (0x0094F180): a self-contained model of the decision logic.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V sub(V a, V b) { V r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
struct Thing { V pos; bool dead; unsigned long flags; bool creature() const { return (flags & 8) != 0; } };
struct World {
    Thing self; float radius; float frontMiddle; bool hasTargeting; Thing* best;
    bool hasEvents; bool meleeEvent; Thing* attacker; bool aware;
    bool losJumpBack; bool losLand; unsigned long frame;
};
struct Group { Thing* PTarget; Thing* PLastJumpTarget; unsigned long TimeOfLastJump; V LandPos; V jumpBack; };
static V GetLandPos(const Thing& t) { V r = { t.pos.X + 1.0f, t.pos.Y, t.pos.Z }; return r; }
static bool IsRunnable(Group& g, World& w)
{
    if (w.hasTargeting) {
        Thing* target = w.best;
        bool hitByMelee = false;
        if (w.hasEvents && w.meleeEvent) {
            Thing* a = w.attacker;
            if (a && !a->dead && a->creature()) { target = a; hitByMelee = true; }
        }
        if (target && !target->dead && (hitByMelee || w.aware)) {
            V away = sub(w.self.pos, target->pos);
            float len = std::sqrt(away.X * away.X + away.Y * away.Y + away.Z * away.Z);
            away.X /= len; away.Y /= len; away.Z /= len;
            float d = w.radius * 2.0f;
            V jb = { away.X * d + w.self.pos.X, away.Y * d + w.self.pos.Y, away.Z * d + w.self.pos.Z };
            g.jumpBack = jb;
            V to = sub(w.self.pos, target->pos);
            float range = w.frontMiddle;
            if (hitByMelee || (to.X * to.X + to.Y * to.Y < range * range && !w.losJumpBack)) {
                V land = GetLandPos(*target);
                if (w.losLand) {
                    g.LandPos = land; g.TimeOfLastJump = w.frame; g.PLastJumpTarget = target; g.PTarget = target;
                    return true;
                }
            }
        }
    }
    return false;
}
int main()
{
    Thing enemy = { { 3.0f, 0.0f, 0.0f }, false, 8 };
    Thing hitter = { { 0.0f, 2.0f, 0.0f }, false, 8 };
    World w = { { { 0.0f, 0.0f, 0.0f }, false, 8 }, 0.5f, 5.0f, true, &enemy, true, false, 0, true, false, true, 77 };
    Group g = { 0, 0, 0, { 0, 0, 0 }, { 0, 0, 0 } };
    int bad = 0;
    if (!IsRunnable(g, w) || g.PTarget != &enemy || g.PLastJumpTarget != &enemy || g.TimeOfLastJump != 77 || g.LandPos.X != 4.0f) bad |= 1;
    if (std::fabs(g.jumpBack.X + 1.0f) > 1e-5f) bad |= 2;                  // one diameter (1.0) behind, away from the enemy
    Group g2 = { 0, 0, 0, { 0, 0, 0 }, { 0, 0, 0 } };
    w.frontMiddle = 2.0f; if (IsRunnable(g2, w) || g2.PTarget) bad |= 4;   // out of range
    w.frontMiddle = 5.0f; w.losJumpBack = true; if (IsRunnable(g2, w)) bad |= 8;   // jump-back spot visible
    w.aware = false; w.losJumpBack = false; if (IsRunnable(g2, w)) bad |= 16;     // unaware of enemy
    w.meleeEvent = true; w.attacker = &hitter; w.losJumpBack = true; w.frontMiddle = 0.1f;
    if (!IsRunnable(g2, w) || g2.PTarget != &hitter) bad |= 32;                  // melee attacker bypasses range/awareness
    hitter.dead = true; if (IsRunnable(g2 = g, w)) bad |= 64;                     // dead attacker -> best enemy, unaware -> false
    hitter.dead = false; hitter.flags = 0; w.aware = true; w.frontMiddle = 5.0f; w.losJumpBack = false;
    Group g3 = { 0, 0, 0, { 0, 0, 0 }, { 0, 0, 0 } };
    if (!IsRunnable(g3, w) || g3.PTarget != &enemy) bad |= 128;                   // non-creature attacker ignored
    w.losLand = false; if (IsRunnable(g3 = g2, w)) bad |= 256;
    w.hasTargeting = false; w.losLand = true; if (IsRunnable(g3, w)) bad |= 512;
    if (bad) { std::printf("0094f180_TEST FAIL\n"); return 1; }
    std::printf("0094f180_TEST PASS\n");
    return 0;
}