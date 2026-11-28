// The fixture uses independent engine models and the same control flow and arguments as FillJug.
#include <cassert>
#include <cstdio>
#include <cstring>

#define FABLE_PORTABLE
#include "fable_assert.h"
#include "engine/EAIAction.h"

struct Fixture2DCoordI {
    long X;
    long Y;
    Fixture2DCoordI(long x, long y) : X(x), Y(y) {}
};

class Fixture3DVector {
    float X;
    float Y;
public:
    Fixture3DVector(float x, float y) : X(x), Y(y) {}
    Fixture2DCoordI ToC2DCoordI() const { return Fixture2DCoordI(static_cast<long>(X), static_cast<long>(Y)); }
};

struct Fixture2DBoxI {
    Fixture2DCoordI Center;
    long Radius;
    Fixture2DBoxI(const Fixture2DCoordI& center, long radius) : Center(center), Radius(radius) {}
};

struct FixtureTCTavernJug {
    long Fullness;
    FixtureTCTavernJug() : Fullness(0) {}
};

struct FixtureTCAnimatingObject {
    int Priorities[20];
    int Count;
    FixtureTCAnimatingObject() : Count(0) {}
    void PlayAnimation(const char* name, long priority, bool loop)
    {
        assert(std::strcmp(name, "DRAW") == 0 && !loop);
        assert(Count < 20);
        Priorities[Count++] = static_cast<int>(priority);
    }
};

struct FixtureThing {
    FixtureTCAnimatingObject* Animation;
    FixtureTCTavernJug* Jug;
    Fixture3DVector Pos;
    bool Dead;
    mutable int AnimationLookups;
    mutable int JugLookups;
    FixtureThing() : Animation(0), Jug(0), Pos(13.8f, -4.8f), Dead(false),
        AnimationLookups(0), JugLookups(0) {}
    const Fixture3DVector& GetPos() const { return Pos; }
    bool GetTC(FixtureTCAnimatingObject** out)
    {
        ++AnimationLookups;
        if (Animation)
            *out = Animation;
        return Animation != 0;
    }
    bool GetTC(FixtureTCTavernJug** out)
    {
        ++JugLookups;
        assert(Jug);
        *out = Jug;
        return true;
    }
};

bool IS_THING_ALIVE(const FixtureThing* pointer) { return pointer && !pointer->Dead; }
bool IS_THING_DEAD(const FixtureThing* pointer) { return !IS_THING_ALIVE(pointer); }

struct FixtureThingFilter_HasDefName {
    const char* Name;
    explicit FixtureThingFilter_HasDefName(const char* name) : Name(name) {}
};

struct FixtureThingCompare_Nearest {
    const Fixture3DVector& Pos;
    explicit FixtureThingCompare_Nearest(const Fixture3DVector& pos) : Pos(pos) {}
};

struct FixtureThingSearchTools {
    FixtureThing Tap;
    mutable int Searches;
    const Fixture3DVector* Origin;
    FixtureThingSearchTools() : Searches(0), Origin(0) {}
    bool GetPBestThingInRoughArea(const Fixture2DBoxI& area, const FixtureThingFilter_HasDefName& filter,
        const FixtureThingCompare_Nearest& nearest, FixtureThing** best) const
    {
        ++Searches;
        assert(area.Center.X == 13 && area.Center.Y == -4 && area.Radius == 2);
        assert(std::strcmp(filter.Name, "OBJECT_GENERIC_BARTAP_01") == 0);
        assert(&nearest.Pos == Origin);
        *best = const_cast<FixtureThing*>(&Tap);
        return true;
    }
};

struct FixtureWorld {
    FixtureThingSearchTools SearchTools;
    static long Frame;
    static int Reads;
    const FixtureThingSearchTools& PeekThingSearchTools() const { return SearchTools; }
    static long GetFrame() { ++Reads; return Frame; }
};
long FixtureWorld::Frame;
int FixtureWorld::Reads;

struct FixtureMainGameComponent {
    FixtureWorld World;
    static FixtureMainGameComponent* Instance;
    static long FPS;
    static FixtureMainGameComponent* Get() { return Instance; }
    static long GetConstantFPS() { return FPS; }
    FixtureWorld& GetWorld() { return World; }
};
FixtureMainGameComponent* FixtureMainGameComponent::Instance;
long FixtureMainGameComponent::FPS;

struct FixtureSoundPair {
    long Sound;
    explicit FixtureSoundPair(long sound) : Sound(sound) {}
};

enum Action { Pickup, Animation, Putdown };

struct FixtureCreatureActionBase {
    FixtureThing& Actor;
    FixtureThing* Target;
    Action Kind;
    FixtureCreatureActionBase(FixtureThing& actor, FixtureThing* target, Action kind)
        : Actor(actor), Target(target), Kind(kind) {}
};
struct FixtureCreatureAction_PickUpJugToFill : FixtureCreatureActionBase {
    FixtureCreatureAction_PickUpJugToFill(FixtureThing& actor, FixtureThing& jug)
        : FixtureCreatureActionBase(actor, &jug, Pickup) {}
};
struct FixtureCreatureAction_PutDownJugFromFilling : FixtureCreatureActionBase {
    FixtureCreatureAction_PutDownJugFromFilling(FixtureThing& actor, FixtureThing& jug)
        : FixtureCreatureActionBase(actor, &jug, Putdown) {}
};
struct FixtureCreatureAction_PlayAnimation : FixtureCreatureActionBase {
    FixtureCreatureAction_PlayAnimation(FixtureThing& actor, const char* name, const FixtureSoundPair& sound,
        bool blend, bool loop, bool movement, long start, long end)
        : FixtureCreatureActionBase(actor, 0, Animation)
    {
        assert(std::strcmp(name, "FILL_JUG_LOOP") == 0);
        assert(sound.Sound == 0 && !blend && loop && !movement && start == 0 && end == 0);
    }
};

enum JugState { Live, Missing, Dead, DiesInLoop, RemovedInLoop };

struct Scenario {
    const char* Name;
    JugState Jug;
    bool HasDrawAnimation;
    EAIActionReturn PickupResult;
    EAIActionReturn AnimationResult;
    EAIActionReturn PutdownResult;
    long FPS;
    long FrameStep;
    int TerminateOn;
    int AnimationCount;
    bool Finished;
    const char* Events;
};

class FixtureAIStateGroup_TavernKeeper {
    const Scenario& Case;
    FixtureThing Creature;
    FixtureThing Jug;
    FixtureTCTavernJug JugComponent;
    FixtureThing* PJug;
    int Animations;
    char Events[32];
    unsigned EventCount;

    void Record(char event)
    {
        assert(EventCount + 1 < sizeof(Events));
        Events[EventCount++] = event;
        Events[EventCount] = 0;
    }

    FixtureThing& GetCreature() { return Creature; }
    void SetPushableByHero(bool pushable) { Record(pushable ? 'E' : 'D'); }
    EAIActionReturn AIAction_PerformActionAndWait(const FixtureCreatureActionBase& action, bool async)
    {
        assert(&action.Actor == &Creature && !async);
        if (action.Kind == Pickup) {
            assert(action.Target == &Jug && JugComponent.Fullness == 0);
            Record('P');
            return Case.PickupResult;
        }
        if (action.Kind == Putdown) {
            assert(action.Target == &Jug && JugComponent.Fullness == 3);
            Record('U');
            return Case.PutdownResult;
        }
        assert(action.Kind == Animation && action.Target == 0 && JugComponent.Fullness == 0);
        ++Animations;
        assert(Animations <= 20);
        Record('F');
        FixtureWorld::Frame += Case.FrameStep;
        if (Case.Jug == DiesInLoop)
            Jug.Dead = true;
        if (Case.Jug == RemovedInLoop)
            PJug = 0;
        if (Animations == Case.TerminateOn)
            return AI_ACTION_TERMINATE_PROCESS;
        return Case.AnimationResult;
    }
public:
    FixtureAIStateGroup_TavernKeeper(const Scenario& scenario, FixtureMainGameComponent& game)
        : Case(scenario), PJug(scenario.Jug == Missing ? 0 : &Jug), Animations(0), EventCount(0)
    {
        Events[0] = 0;
        Jug.Dead = scenario.Jug == Dead;
        Jug.Jug = &JugComponent;
        game.World.SearchTools.Origin = &Creature.GetPos();
    }
    bool FillJug();
    void Run(FixtureMainGameComponent& game, const FixtureTCAnimatingObject& draw)
    {
        bool result = FillJug();
        assert(result == Case.Finished);
        assert(std::strcmp(Events, Case.Events) == 0);
        assert(Animations == Case.AnimationCount);
        assert(game.World.SearchTools.Searches == (Animations ? 1 : 0));
        assert(game.World.SearchTools.Tap.AnimationLookups == (Animations ? 1 : 0));
        assert(Jug.JugLookups == (!std::strchr(Events, 'U') ? 0 : 1));
        assert(JugComponent.Fullness == (Jug.JugLookups ? 3 : 0));
        assert(draw.Count == (Case.HasDrawAnimation ? Animations : 0));
        for (int i = 0; i < draw.Count; ++i)
            assert(draw.Priorities[i] == i);
        assert(FixtureWorld::Reads == (Animations ? 1 + Animations - (Case.TerminateOn != 0) : 0));
    }
};

bool FixtureAIStateGroup_TavernKeeper::FillJug()
{
    if (!IS_THING_ALIVE(PJug))
        return false;

    SetPushableByHero(false);
    if (AIAction_PerformActionAndWait(FixtureCreatureAction_PickUpJugToFill(GetCreature(), *PJug), false) != AI_ACTION_FINISHED) {
        SetPushableByHero(true);
        return false;
    }

    const long box_width_and_height = 2;
    Fixture2DBoxI area(GetCreature().GetPos().ToC2DCoordI(), box_width_and_height);
    const FixtureThingSearchTools& search_tools = FixtureMainGameComponent::Get()->GetWorld().PeekThingSearchTools();
    FixtureThing* bar_tap;
    search_tools.GetPBestThingInRoughArea(area, FixtureThingFilter_HasDefName("OBJECT_GENERIC_BARTAP_01"),
        FixtureThingCompare_Nearest(GetCreature().GetPos()), &bar_tap);

    const long amount_to_fill = 7;
    const long fill_until = FixtureWorld::GetFrame() + FixtureMainGameComponent::GetConstantFPS() * amount_to_fill;
    FixtureTCAnimatingObject* tc_animating_object = 0;
    if (!bar_tap->GetTC(&tc_animating_object)) {
        FABLE_RETAIL_ASSERT(false);
    }
    int priority = 0;
    do {
        if (tc_animating_object)
            tc_animating_object->PlayAnimation("DRAW", priority++, false);
        if (AIAction_PerformActionAndWait(FixtureCreatureAction_PlayAnimation(GetCreature(), "FILL_JUG_LOOP", FixtureSoundPair(0),
                false, true, false, 0, 0), false) == AI_ACTION_TERMINATE_PROCESS) {
            SetPushableByHero(true);
            return false;
        }
    } while (fill_until > FixtureWorld::GetFrame());

    if (IS_THING_DEAD(PJug))
        return false;

    FixtureTCTavernJug* tc_tavern_jug;
    if (!PJug->GetTC(&tc_tavern_jug)) {
        FABLE_RETAIL_ASSERT(false);
    }
    tc_tavern_jug->Fullness = 3;
    if (AIAction_PerformActionAndWait(FixtureCreatureAction_PutDownJugFromFilling(GetCreature(), *PJug), false) != AI_ACTION_FINISHED) {
        SetPushableByHero(true);
        return false;
    }
    SetPushableByHero(true);
    return true;
}

int main()
{
    const Scenario cases[] = {
        {"initial_missing", Missing, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 0, false, ""},
        {"initial_dead", Dead, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 0, false, ""},
        {"pickup_failed", Live, true, AI_ACTION_FAILED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 0, false, "DPE"},
        {"pickup_terminated", Live, true, AI_ACTION_TERMINATE_PROCESS, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 0, false, "DPE"},
        {"first_loop_terminated", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 1, 1, false, "DPFE"},
        {"second_loop_terminated", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 2, 2, false, "DPFFE"},
        {"animation_failed_times_out", Live, true, AI_ACTION_FINISHED, AI_ACTION_FAILED, AI_ACTION_FINISHED, 1, 4, 0, 2, true, "DPFFUE"},
        {"absent_draw_component", Live, false, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 2, true, "DPFFUE"},
        {"equal_deadline", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 7, 0, 1, true, "DPFUE"},
        {"no_clock_advance_at_deadline", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 0, 0, 0, 1, true, "DPFUE"},
        {"jug_dies_during_fill", DiesInLoop, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 2, false, "DPFF"},
        {"jug_removed_during_fill", RemovedInLoop, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 1, 4, 0, 2, false, "DPFF"},
        {"putdown_failed", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FAILED, 1, 4, 0, 2, false, "DPFFUE"},
        {"putdown_terminated", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_TERMINATE_PROCESS, 1, 4, 0, 2, false, "DPFFUE"},
        {"successful_three_ticks", Live, true, AI_ACTION_FINISHED, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 2, 5, 0, 3, true, "DPFFFUE"}
    };
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        FixtureMainGameComponent game;
        FixtureTCAnimatingObject animation;
        game.World.SearchTools.Tap.Animation = cases[i].HasDrawAnimation ? &animation : 0;
        FixtureMainGameComponent::Instance = &game;
        FixtureMainGameComponent::FPS = cases[i].FPS;
        FixtureWorld::Frame = 100;
        FixtureWorld::Reads = 0;
        FixtureAIStateGroup_TavernKeeper keeper(cases[i], game);
        keeper.Run(game, animation);
    }
    std::puts("00968fa0_TEST PASS");
}