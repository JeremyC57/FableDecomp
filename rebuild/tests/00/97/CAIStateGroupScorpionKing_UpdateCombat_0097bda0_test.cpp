// Behaviour test for 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat: a host model of the same decision logic over stub
// creature / enemy / boss / battle-def / world objects; every AI action is logged and checked per scenario.
#include <cstdio>
#include <cmath>

enum { AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3 };
enum { ACT_NONE, ACT_CHARGE, ACT_RIGHT, ACT_LEFT, ACT_PLUNGE, ACT_LOOK, ACT_PUMP, ACT_NOTHING, ACT_EVENT, ACT_SHIELD, ACT_OBSTRUCT, ACT_TERMINATE };

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }

struct BattleDef { long NumChargesBeforeTailStrike; float DamageToCauseTailStrike; float ClawObstructionRadius; };
struct Boss { bool BCountStrikes; long StrikesSinceHit; };

struct Model {
    // world
    Vec MyPos, EnemyPos, Facing; float Health; float FrontNearLimit; bool EnemyAlive, EnemyHasShield, EnemyKnockedDown;
    long Frame, FPS; long RunnableCalls, RunnableLimit; int ActionResult, LookResult;
    BattleDef Def; Boss TheBoss;
    // state group
    long ChargeCount, EnemyKnockDownFrame; float HealthAtLastTailStrike; bool Terminated;
    // log
    int Log[64]; float Arg[64]; int N;
    void Add(int a, float f) { if (N < 64) { Log[N] = a; Arg[N] = f; ++N; } }
    bool IsStillRunnable() { ++RunnableCalls; return RunnableCalls <= RunnableLimit; }
    void Terminate() { Terminated = true; Add(ACT_TERMINATE, 0.0f); }
    int Perform(int act, float f) { Add(act, f); Frame += 10; return ActionResult; }
    int LookAt(float speed) { Add(ACT_LOOK, speed); Frame += 5; return LookResult; }
    void DoNothing() { Add(ACT_NOTHING, 0.0f); Frame += 1; }

    void UpdateCombat()
    {
        if (IsStillRunnable()) {
            EnemyKnockDownFrame = Frame;
            Boss* boss = &TheBoss;
            Vec to_enemy = V(EnemyPos.X - MyPos.X, EnemyPos.Y - MyPos.Y, EnemyPos.Z - MyPos.Z);
            float distance = (float)sqrt(to_enemy.X * to_enemy.X + to_enemy.Y * to_enemy.Y);
            float len = (float)sqrt(to_enemy.X * to_enemy.X + to_enemy.Y * to_enemy.Y + to_enemy.Z * to_enemy.Z);
            to_enemy.X /= len; to_enemy.Y /= len; to_enemy.Z /= len;
            if (distance < 6.0f && Facing.X * to_enemy.X + Facing.Y * to_enemy.Y + Facing.Z * to_enemy.Z > 0.75f) {
                ++ChargeCount;
                Perform(ACT_CHARGE, 8.0f);
                if (!IsStillRunnable()) { Terminate(); return; }
            } else if (distance < FrontNearLimit) {
                boss->BCountStrikes = false;
                float facing_dot = Facing.X * to_enemy.X + Facing.Y * to_enemy.Y + Facing.Z * to_enemy.Z;
                if (facing_dot > 0.5f) {
                    if (EnemyAlive && EnemyHasShield) Add(ACT_SHIELD, 1.0f);
                    Add(ACT_PUMP, 0.0f);
                    if (!IsStillRunnable()) { Terminate(); return; }
                } else {
                    int result;
                    // CrossWithUp of the facing: (Y, -X, 0)
                    if (to_enemy.X * Facing.Y + to_enemy.Y * -Facing.X + to_enemy.Z * 0.0f > 0.0f) result = Perform(ACT_RIGHT, 0.0f);
                    else result = Perform(ACT_LEFT, 0.0f);
                    if (!IsStillRunnable()) { Terminate(); return; }
                    if (result != AI_ACTION_FINISHED) return;
                    if (facing_dot > -0.5f) result = LookAt(0.1f);
                    if (!IsStillRunnable()) { Terminate(); return; }
                    if (result != AI_ACTION_FINISHED) return;
                    if (HealthAtLastTailStrike - Def.DamageToCauseTailStrike >= Health) {
                        Perform(ACT_PLUNGE, 0.0f);
                        if (!IsStillRunnable()) { Terminate(); return; }
                        HealthAtLastTailStrike = Health;
                        boss->StrikesSinceHit = 0;
                        ChargeCount = 0;
                    }
                }
            } else {
                boss->BCountStrikes = true;
                int result = LookAt(0.02f);
                if (!IsStillRunnable()) { Terminate(); return; }
                if (result == AI_ACTION_FINISHED) {
                    if (ChargeCount >= Def.NumChargesBeforeTailStrike || Health <= (HealthAtLastTailStrike - Def.DamageToCauseTailStrike)) {
                        Perform(ACT_PLUNGE, 0.0f);
                        if (!IsStillRunnable()) { Terminate(); return; }
                        HealthAtLastTailStrike = Health;
                        boss->StrikesSinceHit = 0;
                        ChargeCount = 0;
                    } else {
                        to_enemy = V(EnemyPos.X - MyPos.X, EnemyPos.Y - MyPos.Y, EnemyPos.Z - MyPos.Z);
                        distance = (float)sqrt(to_enemy.X * to_enemy.X + to_enemy.Y * to_enemy.Y);
                        Add(ACT_EVENT, 0.0f);
                        ++ChargeCount;
                        result = Perform(ACT_CHARGE, distance);
                        if (!IsStillRunnable()) { Terminate(); return; }
                        if (result != AI_ACTION_FINISHED) return;
                    }
                }
            }
        }
        if (!IsStillRunnable()) { Terminate(); return; }
        if (EnemyKnockedDown) {
            long start_frame = Frame;
            float radius = Def.ClawObstructionRadius + 0.2f;
            Vec position = V(MyPos.X + Facing.X * radius, MyPos.Y + Facing.Y * radius, MyPos.Z + Facing.Z * radius);
            Add(ACT_OBSTRUCT, position.X);
            long frames_waited;
            do {
                DoNothing();
                if (!IsStillRunnable()) { Terminate(); return; }
                frames_waited = Frame - start_frame;
            } while (frames_waited / FPS < 2);
        }
    }
};

static void Init(Model& m)
{
    m.MyPos = V(0, 0, 0); m.EnemyPos = V(4, 0, 0); m.Facing = V(1, 0, 0); m.Health = 100.0f; m.FrontNearLimit = 10.0f;
    m.EnemyAlive = true; m.EnemyHasShield = false; m.EnemyKnockedDown = false; m.Frame = 0; m.FPS = 3; m.RunnableCalls = 0;
    m.RunnableLimit = 100; m.ActionResult = AI_ACTION_FINISHED; m.LookResult = AI_ACTION_FINISHED;
    m.Def.NumChargesBeforeTailStrike = 3; m.Def.DamageToCauseTailStrike = 20.0f; m.Def.ClawObstructionRadius = 1.0f;
    m.TheBoss.BCountStrikes = false; m.TheBoss.StrikesSinceHit = 5;
    m.ChargeCount = 0; m.EnemyKnockDownFrame = -1; m.HealthAtLastTailStrike = 100.0f; m.Terminated = false; m.N = 0;
}

static int fails = 0;
static void Check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) Check((c), __LINE__)

int main()
{
    Model m;
    // 1. close and facing: charge into the enemy with 8.0f, no event
    Init(m); m.UpdateCombat();
    CHECK(m.N == 1 && m.Log[0] == ACT_CHARGE && m.Arg[0] == 8.0f && m.ChargeCount == 1 && !m.Terminated && m.EnemyKnockDownFrame == 0);
    // 2. near range, facing (> 0.5): shield reaction + pump
    Init(m); m.EnemyPos = V(7, 0, 0); m.EnemyHasShield = true; m.UpdateCombat();
    CHECK(m.N == 2 && m.Log[0] == ACT_SHIELD && m.Log[1] == ACT_PUMP && !m.TheBoss.BCountStrikes);
    // 3. near range, enemy to the side (cross > 0 -> right turn), look at (dot > -0.5), lost enough health -> tail plunge
    Init(m); m.EnemyPos = V(0, -7, 0); m.Health = 70.0f; m.ChargeCount = 2; m.UpdateCombat();
    CHECK(m.N == 3 && m.Log[0] == ACT_RIGHT && m.Log[1] == ACT_LOOK && m.Arg[1] == 0.1f && m.Log[2] == ACT_PLUNGE);
    CHECK(m.HealthAtLastTailStrike == 70.0f && m.TheBoss.StrikesSinceHit == 0 && m.ChargeCount == 0);
    // 4. near range, other side, behind (dot <= -0.5: no look), not enough damage: no plunge
    Init(m); m.EnemyPos = V(-5, 5, 0); m.UpdateCombat();
    CHECK(m.N == 1 && m.Log[0] == ACT_LEFT && m.TheBoss.StrikesSinceHit == 5);
    // 5. near range, turn action failed: return before looking
    Init(m); m.EnemyPos = V(0, 7, 0); m.ActionResult = AI_ACTION_FAILED; m.UpdateCombat();
    CHECK(m.N == 1 && m.Log[0] == ACT_LEFT);
    // 6. far: look, then charge with the 2D distance and the charging event
    Init(m); m.EnemyPos = V(30, 40, 5); m.UpdateCombat();
    CHECK(m.N == 3 && m.Log[0] == ACT_LOOK && m.Arg[0] == 0.02f && m.Log[1] == ACT_EVENT && m.Log[2] == ACT_CHARGE);
    CHECK(std::fabs(m.Arg[2] - 50.0f) < 1e-3f && m.ChargeCount == 1 && m.TheBoss.BCountStrikes);
    // 7. far, charges used up: tail plunge
    Init(m); m.EnemyPos = V(30, 0, 0); m.ChargeCount = 3; m.Health = 90.0f; m.UpdateCombat();
    CHECK(m.N == 2 && m.Log[1] == ACT_PLUNGE && m.HealthAtLastTailStrike == 90.0f && m.ChargeCount == 0);
    // 8. far, look failed: skip to the post checks
    Init(m); m.EnemyPos = V(30, 0, 0); m.LookResult = AI_ACTION_FAILED; m.UpdateCombat();
    CHECK(m.N == 1 && m.Log[0] == ACT_LOOK && !m.Terminated);
    // 9. no longer runnable at entry: terminate, nothing else
    Init(m); m.RunnableLimit = 0; m.UpdateCombat();
    CHECK(m.N == 1 && m.Log[0] == ACT_TERMINATE && m.EnemyKnockDownFrame == -1);
    // 10. enemy knocked down: obstruction ahead at radius + 0.2, then wait two seconds of frames
    Init(m); m.EnemyKnockedDown = true; m.UpdateCombat();
    CHECK(m.Log[0] == ACT_CHARGE && m.Log[1] == ACT_OBSTRUCT && std::fabs(m.Arg[1] - 1.2f) < 1e-5f);
    CHECK(m.N == 8 && m.Log[7] == ACT_NOTHING && m.Frame == 16);
    // 11. terminate inside the wait
    Init(m); m.EnemyKnockedDown = true; m.RunnableLimit = 4; m.UpdateCombat();
    CHECK(m.Terminated && m.Log[m.N - 1] == ACT_TERMINATE && m.Log[m.N - 2] == ACT_NOTHING);
    if (fails) { std::printf("0097bda0_TEST FAIL\n"); return 1; }
    std::printf("0097bda0_TEST PASS\n");
    return 0;
}