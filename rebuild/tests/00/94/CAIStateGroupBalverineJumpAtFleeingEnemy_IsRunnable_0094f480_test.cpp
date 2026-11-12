// Behaviour test for CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable (0x0094F480): the same decision through a small
// copy of the model (targeting component present, best enemy alive and known, enemy moving away along our facing faster than
// half its running speed at the point ahead of us, 2D distance strictly between 3 and 5.5).
#include <cstdio>

struct V3 { float X, Y, Z; };
static V3 Sub(const V3& a, const V3& b) { V3 r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V3 Add(const V3& a, const V3& b) { V3 r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
static float Dot(const V3& a, const V3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct Physics { V3 Position, OldPosition, Facing; };
struct Creature {
    Physics   Phys;
    bool      Dead;
    bool      HasTargeting;
    Creature* BestEnemy;
    bool      AwareOfEnemy;
    float     MaxRunningSpeed;
    V3        LastSpeedQueryPoint;
    int       SpeedQueries;
    float GetMaxRunningSpeed(const V3& at) { LastSpeedQueryPoint = at; ++SpeedQueries; return MaxRunningSpeed; }
};
struct StateGroup {
    Creature* Self;
    Creature* PTarget;
    bool IsRunnable()
    {
        if (Self->HasTargeting) {
            Creature* enemy = Self->BestEnemy;
            if (enemy && !enemy->Dead && Self->AwareOfEnemy) {
                const Physics* enemyPhysics = &enemy->Phys;
                V3 enemyMovement = Sub(enemyPhysics->Position, enemyPhysics->OldPosition);
                V3 facing = Self->Phys.Facing;
                V3 toEnemy = Sub(enemyPhysics->Position, Self->Phys.Position);
                V3 ahead = Add(Self->Phys.Position, Self->Phys.Facing);
                float approach = Dot(facing, enemyMovement);
                if (approach > enemy->GetMaxRunningSpeed(ahead) * 0.5f) {
                    float distanceSquared = toEnemy.X * toEnemy.X + toEnemy.Y * toEnemy.Y;
                    if (distanceSquared < 30.25f && distanceSquared > 9.0f) {
                        PTarget = enemy;
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

static Creature g_self, g_enemy;
static StateGroup g_group;

static void Reset()
{
    Creature zero = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, false, false, 0, false, 0.0f, { 0, 0, 0 }, 0 };
    g_self = zero; g_enemy = zero;
    g_self.Phys.Position.X = 10.0f; g_self.Phys.Position.Y = 20.0f; g_self.Phys.Position.Z = 1.0f;
    g_self.Phys.Facing.X = 1.0f;
    g_self.HasTargeting = true; g_self.BestEnemy = &g_enemy; g_self.AwareOfEnemy = true;
    g_enemy.Phys.Position.X = 14.0f; g_enemy.Phys.Position.Y = 20.0f; g_enemy.Phys.Position.Z = 1.0f;       // 4 units ahead
    g_enemy.Phys.OldPosition.X = 13.0f; g_enemy.Phys.OldPosition.Y = 20.0f; g_enemy.Phys.OldPosition.Z = 1.0f; // moving away at 1
    g_enemy.MaxRunningSpeed = 1.5f;                                                                          // half = 0.75 < 1
    g_group.Self = &g_self; g_group.PTarget = 0;
}
static int Fail() { std::printf("0094f480_TEST FAIL\n"); return 1; }

int main()
{
    Reset();                                            // the full success path
    if (!g_group.IsRunnable() || g_group.PTarget != &g_enemy) return Fail();
    if (g_enemy.SpeedQueries != 1) return Fail();
    if (g_enemy.LastSpeedQueryPoint.X != 11.0f || g_enemy.LastSpeedQueryPoint.Y != 20.0f || g_enemy.LastSpeedQueryPoint.Z != 1.0f) return Fail();

    Reset(); g_self.HasTargeting = false;               // no targeting component
    if (g_group.IsRunnable() || g_group.PTarget != 0 || g_enemy.SpeedQueries != 0) return Fail();
    Reset(); g_self.BestEnemy = 0;                      // no enemy
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();
    Reset(); g_enemy.Dead = true;                       // dead enemy
    if (g_group.IsRunnable() || g_group.PTarget != 0 || g_enemy.SpeedQueries != 0) return Fail();
    Reset(); g_self.AwareOfEnemy = false;               // not aware of it
    if (g_group.IsRunnable() || g_group.PTarget != 0 || g_enemy.SpeedQueries != 0) return Fail();

    Reset(); g_enemy.MaxRunningSpeed = 2.0f;            // approach 1 is not greater than 1
    if (g_group.IsRunnable() || g_group.PTarget != 0 || g_enemy.SpeedQueries != 1) return Fail();
    Reset(); g_enemy.Phys.OldPosition.X = 15.0f;        // enemy coming towards us
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();
    Reset(); g_self.Phys.Facing.X = 0.0f; g_self.Phys.Facing.Y = 1.0f;   // moving across our facing
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();

    Reset(); g_enemy.Phys.Position.X = 13.0f; g_enemy.Phys.OldPosition.X = 12.0f;   // exactly 3 away: not > 9
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();
    Reset(); g_enemy.Phys.Position.X = 15.5f; g_enemy.Phys.OldPosition.X = 14.5f;   // exactly 5.5 away: not < 30.25
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();
    Reset(); g_enemy.Phys.Position.X = 15.25f; g_enemy.Phys.OldPosition.X = 14.25f; // 5.25 away: inside the ring
    if (!g_group.IsRunnable() || g_group.PTarget != &g_enemy) return Fail();
    Reset(); g_enemy.Phys.Position.Z = 500.0f; g_enemy.Phys.OldPosition.Z = 500.0f; // height is ignored (2D distance)
    if (!g_group.IsRunnable() || g_group.PTarget != &g_enemy) return Fail();
    Reset(); g_enemy.Phys.Position.Y = 23.0f; g_enemy.Phys.OldPosition.Y = 23.0f;   // (4,3): 25, inside the ring
    if (!g_group.IsRunnable() || g_group.PTarget != &g_enemy) return Fail();
    Reset(); g_enemy.Phys.Position.Y = 24.0f; g_enemy.Phys.OldPosition.Y = 24.0f;   // (4,4): 32, outside
    if (g_group.IsRunnable() || g_group.PTarget != 0) return Fail();

    std::printf("0094f480_TEST PASS\n");
    return 0;
}