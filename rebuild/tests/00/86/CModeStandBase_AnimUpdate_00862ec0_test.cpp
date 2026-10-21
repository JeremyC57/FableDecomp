// Behaviour test for CModeStandBase::AnimUpdate @ 0x00862EC0.
#include <cstdio>
#include <cmath>
#include <cstring>

static char g_log[64];
static void logc(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRandomGenerator {
    unsigned long Seed;
    unsigned long Next() { Seed = (Seed * 0x24a1 + 0x24df) & 0xffffffffUL; Seed = ror13(Seed) & 0xffffffffUL; return Seed; }
};
struct CGameState { bool SuspendIdling, SuspendIdlingForCutscene; };
struct CGlobals { CGameState state; CRandomGenerator rng; };
struct CThingCreatureBase {
    float VelocityX, VelocityY, maxJog; C3DVector pos, vel;
    const C3DVector& GetPosition() const { return pos; }
    C3DVector GetVelocity() const { return vel; }
    float GetMaxJoggingSpeed(const C3DVector&) { return maxJog; }
};
struct CAnimateThing { int dummy; };
static void AddCombinationIdles(CThingCreatureBase*, CAnimateThing&) { logc('C'); }

struct CModeStandBase {
    CThingCreatureBase* ParentThing; CGlobals* globals; long IdleCounter; long FramesToKeepUpdatingMovement; long threshold;
    CGlobals* GetGlobalComponents() const { return globals; }
    long GetNoFramesBeforePlayingCombinationIdleAnims(int) { return threshold; }
    int GetModeType() const { return 1; }
    void UpdateIdleAnim(CAnimateThing&) { logc('I'); }
    void UpdateWalkingAnim(CAnimateThing&) { logc('W'); }
    void AnimUpdate(CAnimateThing& anim);
};

void CModeStandBase::AnimUpdate(CAnimateThing& anim)
{
    CThingCreatureBase* creature = ParentThing;
    float speed = (float)std::sqrt(creature->VelocityX * creature->VelocityX + creature->VelocityY * creature->VelocityY);
    if (GetGlobalComponents()->state.SuspendIdling || GetGlobalComponents()->state.SuspendIdlingForCutscene)
        IdleCounter = 0;
    else
        ++IdleCounter;
    if (speed > 0.0001f || FramesToKeepUpdatingMovement > 0) {
        UpdateWalkingAnim(anim);
        if (creature->GetMaxJoggingSpeed(creature->GetPosition() + creature->GetVelocity()) >= speed) {
            if (IdleCounter > GetNoFramesBeforePlayingCombinationIdleAnims(GetModeType())) {
                if (GetGlobalComponents()->rng.Next() % 3 == 0) {
                    AddCombinationIdles(creature, anim);
                    IdleCounter = 0;
                }
            }
        }
    } else
        UpdateIdleAnim(anim);
}

int main()
{
    CGlobals g; g.state.SuspendIdling = false; g.state.SuspendIdlingForCutscene = false; g.rng.Seed = 1;
    CThingCreatureBase c; c.VelocityX = 0; c.VelocityY = 0; c.maxJog = 10; c.pos = C3DVector(0, 0, 0); c.vel = C3DVector(0, 0, 0);
    CModeStandBase m; m.ParentThing = &c; m.globals = &g; m.IdleCounter = 0; m.FramesToKeepUpdatingMovement = 0; m.threshold = 1000;
    CAnimateThing a;
    m.AnimUpdate(a);
    int ok = m.IdleCounter == 1 && std::strcmp(g_log, "I") == 0;
    g.state.SuspendIdlingForCutscene = true; m.AnimUpdate(a);
    ok = ok && m.IdleCounter == 0;
    g.state.SuspendIdlingForCutscene = false;
    c.VelocityX = 3; c.VelocityY = 4; m.AnimUpdate(a);          // speed 5 <= max jog 10, counter below threshold
    ok = ok && std::strcmp(g_log, "IIW") == 0 && m.IdleCounter == 1;
    m.threshold = 0; int combos = 0;
    for (int i = 0; i < 30; ++i) { g_log[0] = 0; m.AnimUpdate(a); if (std::strchr(g_log, 'C')) { ++combos; ok = ok && m.IdleCounter == 0; } }
    ok = ok && combos > 0 && combos < 30;
    c.maxJog = 1; g_log[0] = 0; m.AnimUpdate(a); ok = ok && std::strcmp(g_log, "W") == 0;
    if (!ok) { std::printf("00862ec0_TEST FAIL\n"); return 1; }
    std::printf("00862ec0_TEST PASS\n");
    return 0;
}