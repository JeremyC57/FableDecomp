#include <math.h>
#include <stdio.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X-other.X,Y-other.Y,Z-other.Z); }
    float GetMagnitude2D() const { return (float)sqrt(X*X + Y*Y); }
    float GetMagnitude() const { return (float)sqrt(X*X + Y*Y + Z*Z); }
};
struct CMazeBattleDef {
    float LightningDuration, LightningRange, EnflameRange;
    mutable int References;
    CMazeBattleDef() : LightningDuration(4.5f), LightningRange(5.0f), EnflameRange(10.0f), References(0) {}
};
template<class T> struct CDefPointer {
    T* Ptr;
    CDefPointer() : Ptr(0) {}
    ~CDefPointer() { if (Ptr) --Ptr->References; }
    const T* operator->() const { return Ptr; }
};
struct CThingBaseDef {
    CMazeBattleDef Maze;
    void GetSubDef(CDefPointer<const CMazeBattleDef>* out) const
    {
        out->Ptr = &Maze;
        ++Maze.References;
    }
};
struct CThing {
    C3DVector Pos;
    unsigned long Flags;
    CThing() : Flags(0) {}
    const C3DVector& GetPos() const { return Pos; }
    unsigned long GetTypeFlags() const { return Flags; }
};
struct CThingCreatureBase : CThing {
    CThingCreatureBase() { Flags = 8; }
};
struct CThingAICreature : CThingCreatureBase {
    CThingBaseDef Def;
    const CThingBaseDef& PeekBaseDef() const { return Def; }
};
template<class T> struct CIntelligentPointer {
    T* Ptr;
    explicit CIntelligentPointer(T* p = 0) : Ptr(p) {}
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& p) : Ptr(p.Get()) {}
    T* Get() const { return Ptr; }
    T* operator->() const { return Ptr; }
};
enum EAIActionReturn { AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1 };
enum EMovementType { AI_MOVEMENT_RUNNING = 5 };
struct CWorld {
    unsigned long Seed;
    CWorld() : Seed(1) {}
    unsigned long* GetWorldSeed() { return &Seed; }
};
unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    unsigned long roll = *seed;
    *seed = (roll + 1) % range;
    return roll % range;
}
enum ActionKind { ENFLAME, LIGHTNING, BOAST, IDLE };
struct CCreatureActionBase {
    ActionKind Kind;
    CThing* Target;
    float Duration;
    long Level;
    CCreatureActionBase(ActionKind kind, CThing* target, float duration = 0, long level = 0)
        : Kind(kind), Target(target), Duration(duration), Level(level) {}
};
struct CCombatAction_WillEnflameInto : CCreatureActionBase {
    CCombatAction_WillEnflameInto(CThingCreatureBase&, long level) : CCreatureActionBase(ENFLAME, 0, 0, level) {}
};
struct CCreatureAction_CastWillLightningSpell : CCreatureActionBase {
    CCreatureAction_CastWillLightningSpell(CThingCreatureBase&, long level, float duration, CThing& target)
        : CCreatureActionBase(LIGHTNING, &target, duration, level) {}
};
struct CCreatureAction_CombatBoast : CCreatureActionBase {
    CCreatureAction_CombatBoast(CThingCreatureBase&, CThingCreatureBase& target) : CCreatureActionBase(BOAST, &target) {}
};
struct CCreatureAction_CombatIdle : CCreatureActionBase {
    CCreatureAction_CombatIdle(CThingCreatureBase&, CThingCreatureBase& target) : CCreatureActionBase(IDLE, &target) {}
};
struct CAIStateGroup_MazeShieldedChase {
    CThingAICreature Creature;
    CWorld World;
    CIntelligentPointer<CThing> PTargetThing;
    bool Knocked, BadArgument;
    unsigned Checks, FailingCheck, Terminations, Moves, Looks, Waits, Nothing;
    float Proximity;
    EAIActionReturn MoveResult, WaitResult;
    ActionKind Actions[2];
    CThing* LastActionTarget;
    CAIStateGroup_MazeShieldedChase(CThing& target)
        : PTargetThing(&target), Knocked(false), BadArgument(false), Checks(0), FailingCheck(0), Terminations(0),
          Moves(0), Looks(0), Waits(0), Nothing(0), Proximity(0), MoveResult(AI_ACTION_FINISHED),
          WaitResult(AI_ACTION_FINISHED), LastActionTarget(0) {}
    bool IsStillRunnable() { return ++Checks != FailingCheck; }
    void Terminate() { ++Terminations; }
    CThingAICreature& GetCreature() { return Creature; }
    CWorld& GetWorld() { return World; }
    bool HasEnemyBeenKnockedDown() { return Knocked; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target,
        float proximity, bool& nav, EMovementType movement, bool first, bool second, bool third, bool fourth, float margin)
    {
        ++Moves;
        Proximity = proximity;
        nav = true;
        if (repeat != ACTION_REPEAT_UNTIL_FINISHED || target.Get() != PTargetThing.Get() ||
            movement != AI_MOVEMENT_RUNNING || !first || second || third || fourth || margin != 0.0f)
            BadArgument = true;
        return MoveResult;
    }
    void AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target, long wait, float speed)
    {
        ++Looks;
        if (repeat != ACTION_REPEAT_UNTIL_FINISHED || target.Get() != PTargetThing.Get() ||
            wait != 0x21 || fabs(speed - 1.0f/12.0f) > 0.0001f)
            BadArgument = true;
    }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait)
    {
        if (Waits < 2)
            Actions[Waits] = action.Kind;
        ++Waits;
        LastActionTarget = action.Target;
        if (wait || (action.Kind == LIGHTNING &&
            (action.Target != PTargetThing.Get() || action.Level != 3 || action.Duration != Creature.Def.Maze.LightningDuration)) ||
            (action.Kind == ENFLAME && action.Level != 3))
            BadArgument = true;
        return WaitResult;
    }
    void AIAction_DoNothing() { ++Nothing; }
    bool CheckArgumentsAndLifetime() const { return !BadArgument && Creature.Def.Maze.References == 0; }
    virtual void Update();
};

void CAIStateGroup_MazeShieldedChase::Update()
{
    CDefPointer<const CMazeBattleDef> p_maze_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_maze_def);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    float max_distance_for_close_will = p_maze_def->LightningRange;
    float max_distance_for_will = p_maze_def->EnflameRange;
    float ideal_distance_for_will;
    if (GFRandom(100, GetWorld().GetWorldSeed()) > 50)
        ideal_distance_for_will = p_maze_def->EnflameRange + 0.5f;
    else
        ideal_distance_for_will = p_maze_def->EnflameRange - 1.0f;

    float dis_to_target = (GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude();
    if (dis_to_target < 30.0f) {
        EAIActionReturn ret = AI_ACTION_FINISHED;
        if (dis_to_target > max_distance_for_close_will) {
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, ideal_distance_for_will, nav,
                AI_MOVEMENT_RUNNING, true, false, false, false, 0.0f);
        }
        if (ret == AI_ACTION_FINISHED) {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, 0x21, 1.0f / 12.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if ((GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude() < max_distance_for_will)
                ret = AIAction_PerformActionAndWait(CCombatAction_WillEnflameInto(GetCreature(), 3), false);
            else
                ret = AIAction_PerformActionAndWait(CCreatureAction_CastWillLightningSpell(GetCreature(), 3,
                    p_maze_def->LightningDuration, *PTargetThing.Get()), false);
        }
        if (!IsStillRunnable() || ret != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        if (PTargetThing->GetTypeFlags() & 8) {
            CThingCreatureBase* pthing_creature = static_cast<CThingCreatureBase*>(PTargetThing.Get());
            if (HasEnemyBeenKnockedDown())
                AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(), *pthing_creature), false);
            else if ((GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude2D() < max_distance_for_close_will)
                AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(), *pthing_creature), false);
        }
    } else {
        AIAction_DoNothing();
    }
}

int main()
{
    CThingCreatureBase target;
    target.Pos = C3DVector(30.0f,0,0);
    CAIStateGroup_MazeShieldedChase distant(target);
    distant.Update();
    if (distant.Nothing != 1 || distant.Waits || distant.Moves || distant.Terminations || !distant.CheckArgumentsAndLifetime())
        return 1;

    target.Pos = C3DVector(14.0f,0,0);
    CAIStateGroup_MazeShieldedChase lightning(target);
    lightning.World.Seed = 60;
    lightning.Knocked = true;
    lightning.Update();
    if (lightning.Proximity != 10.5f || lightning.Moves != 1 || lightning.Looks != 1 || lightning.Waits != 2 ||
        lightning.Actions[0] != LIGHTNING || lightning.Actions[1] != BOAST || lightning.LastActionTarget != &target ||
        lightning.Terminations || !lightning.CheckArgumentsAndLifetime())
        return 2;

    target.Pos = C3DVector(2.0f,1.0f,0);
    CAIStateGroup_MazeShieldedChase enflame(target);
    enflame.Update();
    if (enflame.Moves || enflame.Looks != 1 || enflame.Waits != 2 || enflame.Actions[0] != ENFLAME ||
        enflame.Actions[1] != IDLE || enflame.Terminations || !enflame.CheckArgumentsAndLifetime())
        return 3;

    target.Pos = C3DVector(7.0f,0,0);
    CAIStateGroup_MazeShieldedChase stalled(target);
    stalled.MoveResult = AI_ACTION_FAILED;
    stalled.Update();
    if (stalled.Proximity != 9.0f || stalled.Moves != 1 || stalled.Looks || stalled.Waits || stalled.Terminations != 1 ||
        !stalled.CheckArgumentsAndLifetime())
        return 4;

    CAIStateGroup_MazeShieldedChase unavailable(target);
    unavailable.FailingCheck = 1;
    unavailable.Update();
    if (unavailable.Terminations != 1 || unavailable.Moves || unavailable.Looks || unavailable.Waits || !unavailable.CheckArgumentsAndLifetime())
        return 5;

    CAIStateGroup_MazeShieldedChase interrupted(target);
    interrupted.FailingCheck = 3;
    interrupted.Update();
    if (interrupted.Terminations != 1 || interrupted.Moves != 1 || interrupted.Looks != 1 || interrupted.Waits ||
        !interrupted.CheckArgumentsAndLifetime())
        return 6;

    CAIStateGroup_MazeShieldedChase spell_failed(target);
    spell_failed.WaitResult = AI_ACTION_FAILED;
    spell_failed.Update();
    if (spell_failed.Terminations != 1 || spell_failed.Moves != 1 || spell_failed.Looks != 1 || spell_failed.Waits != 1 ||
        !spell_failed.CheckArgumentsAndLifetime())
        return 7;

    CThing bottle;
    bottle.Pos = C3DVector(1.0f,0,0);
    CAIStateGroup_MazeShieldedChase object(bottle);
    object.Knocked = true;
    object.Update();
    if (object.Waits != 1 || object.Actions[0] != ENFLAME || object.Terminations || !object.CheckArgumentsAndLifetime())
        return 8;

    printf("009552c0_TEST PASS\n");
    return 0;
}
