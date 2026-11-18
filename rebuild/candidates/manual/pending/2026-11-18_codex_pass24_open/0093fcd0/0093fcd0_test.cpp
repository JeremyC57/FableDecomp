#include <cassert>
#include <cstdio>
#include <vector>

enum ETCInterfaceType { TCI_PERCEIVE_THING, TCI_NAVIGATION };
enum EAIActionReturn { AI_ACTION_NULL, AI_ACTION_TERMINATE_PROCESS, AI_ACTION_FINISHED, AI_ACTION_FAILED, AI_ACTION_IN_PROGRESS };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED=1, ACTION_REPEAT_CALLBACK=2 };
enum EMovementType { AI_MOVEMENT_RUNNING=5 };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x=0, float y=0, float z=0) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& b) const { return C3DVector(X+b.X,Y+b.Y,Z+b.Z); }
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X-b.X,Y-b.Y,Z-b.Z); }
    void RotateXY(float) { }
};

class CThing;
class CTCPerceiveThing {
public:
    const CThing* target; bool heard, seen, smelt;
    CTCPerceiveThing() : target(0), heard(false), seen(false), smelt(false) { }
    const CThing* GetPerceivedThing() const { return target; }
    bool IsThingHeard() const { return heard; }
    bool IsThingLineOfSight() const { return seen; }
    bool IsThingSmelt() const { return smelt; }
};

class CTCCreatureNavigation {
public:
    mutable unsigned searches;
    CTCCreatureNavigation() : searches(0) { }
    bool GetNearbyWanderPosition(const C3DVector& from, float range, C3DVector& to, bool blocked) const
    {
        assert(from.X==10 && range==4 && !blocked);
        ++searches;
        to = C3DVector(25,0,0);
        return true;
    }
};

class CThing {
public:
    bool dead, weapon;
    CTCPerceiveThing perception;
    CTCCreatureNavigation navigation;
    CThing() : dead(false), weapon(true) { }
    void PeekTC(ETCInterfaceType type, const CTCPerceiveThing** ptr) const
    { assert(type==TCI_PERCEIVE_THING); *ptr=&perception; }
    void GetTC(ETCInterfaceType type, CTCCreatureNavigation** ptr)
    { assert(type==TCI_NAVIGATION); *ptr=&navigation; }
};
bool IS_THING_ALIVE(const CThing* thing) { return thing && !thing->dead; }

class CVillageTaskSecuritySweep {
public:
    C3DVector GetPositionToGoTo() { return C3DVector(10,0,0); }
    C3DVector GetPositionToLookTowards() { return C3DVector(10,5,0); }
    float GetRadiusAroundThePositionToStay() { return 4; }
    float GetAngleToLookTowards() { return 2; }
};

template<class T> class CIntelligentPointer {
public:
    T* data;
    CIntelligentPointer(T* ptr=0) : data(ptr) { }
    T* Get() const { return data; }
    T* operator->() const { return data; }
    T& operator*() const { return *data; }
};

class CTCVillage {
public:
    unsigned calls; bool accepted;
    CTCVillage() : calls(0), accepted(true) { }
    bool SetSecuritySweepTaskAsAcceptedByThing(CVillageTaskSecuritySweep&, const CThing&)
    { ++calls; return accepted; }
};

class CTCVillageMember {
public:
    CTCVillage village;
    CTCVillage* GetPVillage() { return &village; }
};

class CTCCarrying {
public:
    static bool IsThingCarryingWeapon(const CThing& thing) { return thing.weapon; }
};

class CCreatureAction_UnsheatheWeapons {
public:
    explicit CCreatureAction_UnsheatheWeapons(CThing& thing) { assert(!thing.weapon); }
};

class CMainGameComponent {
public:
    static long GetConstantFPS() { return 30; }
};

class CWorld {
public:
    static unsigned long GetFrame() { return 100; }
};

unsigned long GFRandom(unsigned long range, unsigned long* seed) { return (*seed)++ % range; }
float GFFloatRandom(float range, unsigned long*) { return range*0.25f; }

class CAIStateGroup_VillageTaskSecuritySweep {
public:
    CIntelligentPointer<CVillageTaskSecuritySweep> PTask;
    CTCVillageMember* PTCVillageMember;
    CThing creature;
    unsigned look, unsheathe, moves, exits, wander;
    unsigned long seed;
    bool stop_on_look, first_move_in_progress, fail_first_move, lose_task;
    explicit CAIStateGroup_VillageTaskSecuritySweep(CTCVillageMember& member)
        : PTCVillageMember(&member), look(0), unsheathe(0), moves(0), exits(0), wander(0),
          seed(0), stop_on_look(false), first_move_in_progress(false), fail_first_move(false),
          lose_task(false) { }
    CThing& GetCreature() { return creature; }
    unsigned long* GetPWorldSeed() { return &seed; }
    void Terminate() { ++exits; }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_UnsheatheWeapons&, bool wait)
    { assert(!wait); ++unsheathe; return AI_ACTION_FINISHED; }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target,
                                       long frames, float speed)
    {
        assert(repeat==ACTION_REPEAT_UNTIL_FINISHED && target.Get() && frames==35 && speed>0);
        ++look;
        return stop_on_look ? AI_ACTION_TERMINATE_PROCESS : AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat, C3DVector, long, float)
    { ++look; return AI_ACTION_TERMINATE_PROCESS; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& calculated,
        EMovementType movement, bool nearby, bool use_path, const CThing* avoid, bool moving, float speed)
    {
        assert(movement==AI_MOVEMENT_RUNNING && nearby && use_path && !avoid && moving && speed==0);
        if (repeat==ACTION_REPEAT_UNTIL_FINISHED) {
            assert(pos.X==25);
            calculated = false;
            ++wander;
            return AI_ACTION_TERMINATE_PROCESS;
        }
        assert(pos.X==10 && calculated==(moves!=0));
        calculated=true;
        ++moves;
        if (lose_task) PTask.data=0;
        if (fail_first_move) return AI_ACTION_FAILED;
        return moves==1 && first_move_in_progress ? AI_ACTION_IN_PROGRESS : AI_ACTION_FINISHED;
    }
    void Update();
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CAIStateGroup_VillageTaskSecuritySweep::Update()
{
    if (!PTask.Get()) {
        Terminate();
        return;
    }
    CTCVillage* pvillage = PTCVillageMember->GetPVillage();
    if (!pvillage->SetSecuritySweepTaskAsAcceptedByThing(*PTask, GetCreature())) {
        Terminate();
        return;
    }
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false);
    const CTCPerceiveThing* ptc_perception = 0;
    GetCreature().PeekTC(TCI_PERCEIVE_THING, &ptc_perception);
    EAIActionReturn ret;
    if (((IS_THING_ALIVE(ptc_perception->GetPerceivedThing()) && ptc_perception->IsThingHeard()) ||
         ptc_perception->IsThingLineOfSight() || ptc_perception->IsThingSmelt()) &&
        (ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, ptc_perception->GetPerceivedThing(), 35, 0.0027777f))
            == AI_ACTION_TERMINATE_PROCESS) {
    } else {
        ret = AI_ACTION_IN_PROGRESS;
    }
    bool nav_route_calculated = false;
    while (ret == AI_ACTION_IN_PROGRESS) {
        if (!PTask.Get()) {
            Terminate();
            return;
        }
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, PTask->GetPositionToGoTo(),
            nav_route_calculated, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
    }
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    unsigned long* pworld_seed = GetPWorldSeed();
    do {
        long num_looks_before_moving = GFRandom(5, pworld_seed) + 2;
        while (num_looks_before_moving <= 0) {
            --num_looks_before_moving;
            if (ret == AI_ACTION_TERMINATE_PROCESS)
                break;
            if (!PTask.Get()) {
                Terminate();
                return;
            }
            float t_angle = GFFloatRandom(PTask->GetAngleToLookTowards(), pworld_seed);
            t_angle -= PTask->GetAngleToLookTowards() * 0.5f;
            C3DVector vector_to_look = PTask->GetPositionToLookTowards() - PTask->GetPositionToGoTo();
            vector_to_look.RotateXY(t_angle);
            C3DVector pos_to_look_at = PTask->GetPositionToGoTo() + vector_to_look;
            unsigned long frame_to_stop_looking = CMainGameComponent::GetConstantFPS() * 2;
            frame_to_stop_looking += CMainGameComponent::GetConstantFPS() * GFRandom(3, pworld_seed) + CWorld::GetFrame();
            do {
                ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, pos_to_look_at, 11, 0.0027777f);
            } while (static_cast<unsigned long>(CWorld::GetFrame()) < frame_to_stop_looking &&
                     ret != AI_ACTION_TERMINATE_PROCESS);
        }
        if (!PTask.Get()) {
            Terminate();
            return;
        }
        CTCCreatureNavigation* pnav;
        GetCreature().GetTC(TCI_NAVIGATION, &pnav);
        C3DVector next_pos;
        if (pnav->GetNearbyWanderPosition(PTask->GetPositionToGoTo(),
            PTask->GetRadiusAroundThePositionToStay(), next_pos, false)) {
            bool nav_route_calculated2;
            ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, next_pos, nav_route_calculated2,
                AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
        }
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
    Terminate();
}

int main()
{
    CTCVillageMember member;
    CVillageTaskSecuritySweep task;
    CAIStateGroup_VillageTaskSecuritySweep missing(member);
    missing.Update();
    assert(missing.exits==1 && member.village.calls==0);

    member.village.accepted=false;
    CAIStateGroup_VillageTaskSecuritySweep rejected(member);
    rejected.PTask.data=&task;
    rejected.Update();
    assert(rejected.exits==1 && rejected.moves==0 && member.village.calls==1);

    member.village.accepted=true;
    CThing extra;
    CAIStateGroup_VillageTaskSecuritySweep blocked(member);
    blocked.PTask.data=&task;
    blocked.creature.perception.target=&extra;
    blocked.creature.perception.heard=true;
    blocked.stop_on_look=true;
    blocked.Update();
    assert(blocked.exits==1 && blocked.look==1 && blocked.moves==0);

    CAIStateGroup_VillageTaskSecuritySweep lost(member);
    lost.PTask.data=&task;
    lost.first_move_in_progress=true;
    lost.lose_task=true;
    lost.Update();
    assert(lost.exits==1 && lost.moves==1 && lost.wander==0);

    CAIStateGroup_VillageTaskSecuritySweep success(member);
    success.PTask.data=&task;
    success.first_move_in_progress=true;
    success.creature.weapon=false;
    success.Update();
    assert(success.exits==1 && success.moves==2 && success.wander==1 && success.unsheathe==1);
    assert(success.seed==1 && success.creature.navigation.searches==1);
    std::puts("0093fcd0_TEST PASS");
}
