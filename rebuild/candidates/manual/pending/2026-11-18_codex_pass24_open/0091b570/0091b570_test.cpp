// Standalone C++03 fixture for perception, nearby alerts, and the pursuit animation.
#include <algorithm>
#include <cstdio>
#include <functional>
#include <list>

struct CThing {
    int ID;
};

struct CTCPerceiveThing {
    bool Aware, Direct;
    CThing* Target;
    int IndirectCalls;
    CTCPerceiveThing() : Aware(false), Direct(false), Target(0), IndirectCalls(0) {}
    bool IsAwareOfThing() const { return Aware; }
    bool IsAwarenessOfThingDirect() const { return Direct; }
    const CThing* GetPerceivedThing() const { return Target; }
    void SetAsIndirectlyAwareOfPerceivedThing() { ++IndirectCalls; }
};

struct C3DVector { float X, Y, Z; };
struct CAnimationEntry { int Id; };
struct CSoundPair {};
enum ETCInterfaceType { TCI_PERCEIVE_THING };

struct Creature : CThing {
    C3DVector Position;
    const CTCPerceiveThing* Perception;
    CAnimationEntry Animation;
    bool AnimationQueryValid;
    int AnimationCalls;

    Creature() : Perception(0), AnimationQueryValid(false), AnimationCalls(0) {
        Position.X = 10.0f;
        Position.Y = -5.0f;
        Position.Z = 2.0f;
        Animation.Id = 25;
    }

    bool GetTC(ETCInterfaceType, const CTCPerceiveThing** out) const {
        *out = Perception;
        return Perception != 0;
    }

    const C3DVector& GetPos() const { return Position; }

    const CAnimationEntry& GetAnimFromName(const char* anim, long index) {
        ++AnimationCalls;
        const char alarm[] = "ALARM";
        AnimationQueryValid = index == 0;
        for (int i = 0; i < 6; ++i)
            AnimationQueryValid = AnimationQueryValid && anim[i] == alarm[i];
        return Animation;
    }
};

struct C2DBoxI {
    long Left, Top, Right, Bottom;
    void Initialise(long l, long t, long r, long b) {
        Left = l;
        Top = t;
        Right = r;
        Bottom = b;
    }
};

struct CThingFilter_IsPerceivingThing {
    const CThing& Target;
    explicit CThingFilter_IsPerceivingThing(const CThing& thing) : Target(thing) {}
};
struct CThingFilter_IsNot {
    const CThing& Excluded;
    explicit CThingFilter_IsNot(const CThing& thing) : Excluded(thing) {}
};
template<class A, class B> struct CFilter_Bind2 {
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) {}
};

struct CThingSearchTools {
    CTCPerceiveThing Neighbor;
    const CThing* Target;
    const CThing* Excluded;
    C2DBoxI Area;
    int SearchCalls;
    CThingSearchTools() : Target(0), Excluded(0), SearchCalls(0) {}

    template<class T, class Filter, class Container>
    void GetAllTCsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& output) {
        ++SearchCalls;
        Target = &filter.First.Target;
        Excluded = &filter.Second.Excluded;
        Area = area;
        output.push_back(&Neighbor);
    }
};

struct CThingManager {
    CThingSearchTools Search;
    CThingSearchTools& GetThingSearchTools() { return Search; }
};

struct CCreatureAction_PlayAnimation {
    bool ArgumentsValid;
    CCreatureAction_PlayAnimation(Creature&, const CAnimationEntry& entry, const CSoundPair&,
        bool a, bool b, bool c, long d, long e)
        : ArgumentsValid(entry.Id == 25 && !a && b && !c && d == 0 && e == 0) {}
};

template<class T> struct CIntelligentPointer {
    T* Object;
    explicit CIntelligentPointer(T* object) : Object(object) {}
};

struct CAIStateGroup_SeekPerceivedThing {
    Creature Actor;
    CThingManager Manager;
    const CThing* Facing;
    int AnimationActions, MoveCalls;
    bool ActionValid;

    CAIStateGroup_SeekPerceivedThing() : Facing(0), AnimationActions(0), MoveCalls(0), ActionValid(false) {}
    Creature& GetCreature() { return Actor; }
    CThingManager& GetThingManager() { return Manager; }
    void AIAction_PerformActionTargetFaceAndWait(const CCreatureAction_PlayAnimation& action,
        CIntelligentPointer<const CThing> target, bool flag) {
        ++AnimationActions;
        Facing = target.Object;
        ActionValid = action.ArgumentsValid && !flag && MoveCalls < AnimationActions;
    }
    void RunToPerceivedThingPosition() { ++MoveCalls; }
    void Update();
};

// Recovered method body, with game dependencies supplied by the fixture.
void CAIStateGroup_SeekPerceivedThing::Update()
{
    const CTCPerceiveThing* tc_perception;
    if (!GetCreature().GetTC(TCI_PERCEIVE_THING, &tc_perception) || !tc_perception->IsAwareOfThing())
        return;
    if (tc_perception->IsAwarenessOfThingDirect()) {
        std::list<CTCPerceiveThing*> tclist;
        const C3DVector& pos = GetCreature().GetPos();
        C2DBoxI area;
        area.Initialise((long)(pos.X - 8.0f), (long)(pos.Y - 8.0f),
                        (long)(pos.X + 8.0f), (long)(pos.Y + 8.0f));
        GetThingManager().GetThingSearchTools().GetAllTCsInRoughArea<CTCPerceiveThing>(area,
            CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>(
                CThingFilter_IsPerceivingThing(*tc_perception->GetPerceivedThing()), CThingFilter_IsNot(GetCreature())),
            tclist);
        std::for_each(tclist.begin(), tclist.end(), std::mem_fun(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
    }
    const CAnimationEntry& anim_entry = GetCreature().GetAnimFromName("ALARM", 0);
    CCreatureAction_PlayAnimation action(GetCreature(), anim_entry, CSoundPair(), false, true, false, 0, 0);
    AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointer<const CThing>(tc_perception->GetPerceivedThing()), false);
    RunToPerceivedThingPosition();
}

int main()
{
    CAIStateGroup_SeekPerceivedThing owner;
    CTCPerceiveThing perception;
    CThing target = { 12 };
    perception.Target = &target;
    owner.Update();
    bool ok = owner.AnimationActions == 0 && owner.MoveCalls == 0;
    owner.Actor.Perception = &perception;
    owner.Update();
    ok = ok && owner.AnimationActions == 0 && owner.MoveCalls == 0;
    perception.Aware = true;
    owner.Update();
    CThingSearchTools& search = owner.Manager.Search;
    ok = ok && search.SearchCalls == 0 && owner.ActionValid
        && owner.Actor.AnimationQueryValid && owner.Facing == &target && owner.MoveCalls == 1;
    perception.Direct = true;
    owner.Update();
    ok = ok && search.SearchCalls == 1 && search.Neighbor.IndirectCalls == 1
        && search.Excluded == &owner.Actor && search.Target == &target
        && search.Area.Left == 2 && search.Area.Top == -13 && search.Area.Right == 18 && search.Area.Bottom == 3
        && owner.AnimationActions == 2 && owner.MoveCalls == 2 && owner.ActionValid;
    std::puts(ok ? "0091b570_TEST PASS" : "0091b570_TEST FAIL");
    return ok ? 0 : 1;
}
