// Behaviour test for 0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("00957db0_TEST FAIL line %d\n", line); exit(1); } }
enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1, ACTION_REPEAT_CALLBACK = 2 };
enum EMovementType { AI_MOVEMENT_WALKING = 3 };
class CThing { };
class C3DVector { public: float X, Y, Z; };
template <class T> class CIntelligentPointer { public: T* Item; };
class CTCPerceiveThing { public:
    float AwarenessTimer; bool LineOfSight; bool DirectAwareness;
    bool IsAwareOfThing() const { return AwarenessTimer != 0.0f; }
    bool IsAwarenessOfThingDirect() const { trace += 'D'; return DirectAwareness; }
    bool IsThingLineOfSight() const { return LineOfSight; }
};
class CThingAICreature { public:
    CTCPerceiveThing* perceive;
    bool GetTC(CTCPerceiveThing** out) { trace += 'T'; if (!perceive) return false; *out = perceive; return true; }
};
class CAIStateGroup_MinionBehaviourCrowdBase { public:
    CThingAICreature creature;
    CIntelligentPointer<const CThing> PCrowdThing;
    C3DVector DesignatedCrowdPosition;
    const EAIActionReturn* moves; const EAIActionReturn* looks;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(creature); }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& reached, EMovementType movement, bool, bool, const CThing* avoid, bool, float speed)
    { check(repeat == ACTION_REPEAT_CALLBACK && pos.X == 1.0f && movement == AI_MOVEMENT_WALKING && avoid == 0 && speed == 0.0f, __LINE__); reached = true; trace += 'M'; return *moves++; }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames, float speed)
    { check(repeat == ACTION_REPEAT_CALLBACK && thing.Item == PCrowdThing.Item && frames == 0x11 && speed == 0.01f, __LINE__); trace += 'L'; return *looks++; }
    void Terminate() { trace += 'X'; }
    void UpdateLeaf() { trace += 'U'; }
    void Update();
};

void CAIStateGroup_MinionBehaviourCrowdBase::Update()
{
    CTCPerceiveThing* perceive;
    GetCreature().GetTC(&perceive);
    bool nav_route_calculated;
    EAIActionReturn ret;
    do {
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, DesignatedCrowdPosition, nav_route_calculated, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
        if (!perceive->IsAwareOfThing() || !perceive->IsAwarenessOfThingDirect() || !perceive->IsThingLineOfSight()) {
            ret = AI_ACTION_FAILED;
            break;
        }
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    do {
        ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PCrowdThing, 0x11, 0.01f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED)
        UpdateLeaf();
}

static std::string run(float timer, bool direct, bool los, const EAIActionReturn* moves, const EAIActionReturn* looks)
{
    CTCPerceiveThing p; p.AwarenessTimer = timer; p.DirectAwareness = direct; p.LineOfSight = los;
    CAIStateGroup_MinionBehaviourCrowdBase g;
    g.creature.perceive = &p; g.PCrowdThing.Item = 0; g.DesignatedCrowdPosition.X = 1.0f;
    g.moves = moves; g.looks = looks;
    trace = ""; g.Update(); return trace;
}

int main()
{
    static const EAIActionReturn m1[] = { AI_ACTION_IN_PROGRESS, AI_ACTION_IN_PROGRESS, AI_ACTION_FINISHED };
    static const EAIActionReturn m2[] = { AI_ACTION_FAILED };
    static const EAIActionReturn l1[] = { AI_ACTION_IN_PROGRESS, AI_ACTION_FINISHED };
    static const EAIActionReturn l2[] = { AI_ACTION_FAILED };
    check(run(1.0f, true, true, m1, l1) == "TMDMDMDLLU", __LINE__);    // arrive, look until done, leaf update
    check(run(1.0f, true, true, m1, l2) == "TMDMDMDL", __LINE__);      // look fails: no leaf update, no terminate
    check(run(1.0f, true, true, m2, l1) == "TMDX", __LINE__);          // move failed: terminate
    check(run(0.0f, true, true, m1, l1) == "TMX", __LINE__);           // not aware: terminate at once
    check(run(1.0f, false, true, m1, l1) == "TMDX", __LINE__);         // not directly aware
    check(run(1.0f, true, false, m1, l1) == "TMDX", __LINE__);         // no line of sight
    printf("00957db0_TEST PASS\n");
    return 0;
}