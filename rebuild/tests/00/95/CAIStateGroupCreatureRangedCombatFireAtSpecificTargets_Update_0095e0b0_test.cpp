// Behaviour test for 0x0095E0B0 CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update(): a copy of the function's
// logic over a recording model -- the step order, the arguments of each step, the exits and the unconditional Terminate().
#include <cstdio>
#include <cstring>
enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1, ACTION_REPEAT_CALLBACK = 2 };
enum EMovementType { AI_MOVEMENT_RUNNING = 5 };
struct Thing { bool Dead; bool IsDead() const { return Dead; } };
struct Pointer { Thing* Item; Thing* Get() const { return Item; } };
static bool IS_THING_ALIVE(const Pointer& thing)
{
    if (thing.Get() != 0) {
        if (!thing.Get()->IsDead()) return true;
    }
    return false;
}
enum { ACT_AIM = 'A', ACT_FIRE = 'F', ACT_RELOAD = 'R' };
struct Action { int Kind; int Creature; const Thing* Target; };
static Action Aim(int creature) { Action a; a.Kind = ACT_AIM; a.Creature = creature; a.Target = 0; return a; }
static Action Fire(int creature, const Thing* target) { Action a; a.Kind = ACT_FIRE; a.Creature = creature; a.Target = target; return a; }
static Action Reload(int creature) { Action a; a.Kind = ACT_RELOAD; a.Creature = creature; a.Target = 0; return a; }
struct Group {
    Pointer PTarget;
    int Creature;
    EAIActionReturn Results[4];      // strafe, aim, fire, reload
    int DieAfter;                    // the step (0..3) after which the target dies, -1 never
    char Log[16]; int Len; bool BadArg; int Terminations;
    void Put(char c) { Log[Len++] = c; Log[Len] = 0; }
    void After(int step) { if (DieAfter == step && PTarget.Item) PTarget.Item->Dead = true; }
    int GetCreature() const { return Creature; }
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToThingProximity(EAIActionRepeat repeat, Pointer thing, Pointer target,
        float proximity, bool& reached, EMovementType movement, bool flag)
    {
        Put('S');
        if (repeat != ACTION_REPEAT_UNTIL_FINISHED || thing.Item != PTarget.Item || target.Item != PTarget.Item
            || proximity != 15.0f || movement != AI_MOVEMENT_RUNNING || !flag) BadArg = true;
        reached = true;
        After(0);
        return Results[0];
    }
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const Action& action, Pointer target, bool wait)
    {
        Put((char)action.Kind);
        if (target.Item != PTarget.Item || wait || action.Creature != Creature) BadArg = true;
        if (action.Kind == ACT_FIRE && action.Target != PTarget.Item) BadArg = true;
        int step = action.Kind == ACT_AIM ? 1 : action.Kind == ACT_FIRE ? 2 : 3;
        After(step);
        return Results[step];
    }
    void Terminate() { Put('T'); ++Terminations; }
    void Update()
    {
        if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
        bool inRange;
        if (AIAction_ProjectileAttackAndStrafeToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PTarget, PTarget, 15.0f, inRange, AI_MOVEMENT_RUNNING, true) != AI_ACTION_FINISHED) { Terminate(); return; }
        if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
        if (AIAction_PerformActionTargetFaceAndWait(Aim(GetCreature()), PTarget, false) != AI_ACTION_FINISHED) { Terminate(); return; }
        if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
        EAIActionReturn fired = AIAction_PerformActionTargetFaceAndWait(Fire(GetCreature(), PTarget.Get()), PTarget, false);
        if (fired != AI_ACTION_FINISHED) { Terminate(); return; }
        if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
        AIAction_PerformActionTargetFaceAndWait(Reload(GetCreature()), PTarget, false);
        Terminate();
    }
};
static bool Run(bool haveTarget, bool dead, int r0, int r1, int r2, int r3, int dieAfter, const char* expect)
{
    Thing thing; thing.Dead = dead;
    Group g;
    g.PTarget.Item = haveTarget ? &thing : 0;
    g.Creature = 77;
    g.Results[0] = (EAIActionReturn)r0; g.Results[1] = (EAIActionReturn)r1; g.Results[2] = (EAIActionReturn)r2; g.Results[3] = (EAIActionReturn)r3;
    g.DieAfter = dieAfter; g.Len = 0; g.Log[0] = 0; g.BadArg = false; g.Terminations = 0;
    g.Update();
    return !g.BadArg && g.Terminations == 1 && std::strcmp(g.Log, expect) == 0;
}
int main()
{
    bool ok = true;
    ok = ok && Run(false, false, 2, 2, 2, 2, -1, "T");         // no target
    ok = ok && Run(true, true, 2, 2, 2, 2, -1, "T");           // target already dead
    ok = ok && Run(true, false, 4, 2, 2, 2, -1, "ST");         // strafe still in progress
    ok = ok && Run(true, false, 3, 2, 2, 2, -1, "ST");         // strafe failed
    ok = ok && Run(true, false, 2, 2, 2, 2, 0, "ST");          // target died while strafing
    ok = ok && Run(true, false, 2, 1, 2, 2, -1, "SAT");        // aim interrupted
    ok = ok && Run(true, false, 2, 2, 2, 2, 1, "SAT");         // target died while aiming
    ok = ok && Run(true, false, 2, 2, 3, 2, -1, "SAFT");       // fire failed
    ok = ok && Run(true, false, 2, 2, 2, 2, 2, "SAFT");        // target died from the shot: no reload
    ok = ok && Run(true, false, 2, 2, 2, 2, -1, "SAFRT");      // full cycle
    ok = ok && Run(true, false, 2, 2, 2, 3, -1, "SAFRT");      // reload result is ignored
    ok = ok && Run(true, false, 2, 2, 2, 2, 3, "SAFRT");       // target dying during the reload changes nothing
    if (!ok) { std::printf("0095e0b0_TEST FAIL\n"); return 1; }
    std::printf("0095e0b0_TEST PASS\n");
    return 0;
}