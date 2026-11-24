#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#include "engine/C3DVector.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingCompare_Nearest.h"
#include <cstdio>
#include <cstring>

static unsigned failures;
#define CHECK(x) do { if (!(x)) { if (failures < 20) std::printf("line %d: %s\n", __LINE__, #x); ++failures; } } while (0)

struct World {
    static long frame;
    static long GetFrame() { return frame; }
};
long World::frame;
struct CMainGameComponent { static long GetConstantFPS() { return 20; } };

static unsigned long seed;
static unsigned draws;
static float randomValue;
static float GFFloatRandom(float limit, unsigned long* state)
{
    CHECK(limit == 6.0f && state == &seed);
    ++draws;
    return randomValue;
}
struct Faction {};
struct Enemy {
    Faction faction;
    const Faction* GetPFaction() const { return &faction; }
};
struct Modes {
    bool mayPush;
    unsigned adds, removes;
    Modes() : mayPush(false), adds(0), removes(0) {}
    bool AddMode(unsigned long id) { CHECK(id == 15); ++adds; return mayPush; }
    void RemoveMode(unsigned long id) { CHECK(id == 15); ++removes; }
};
static bool Same(const C3DVector& a, const C3DVector& b)
{
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}
struct Navigation {
    bool navigable, directed, nearest;
    mutable unsigned checked, directedCalls, nearestCalls;
    Navigation() : navigable(false), directed(false), nearest(false), checked(0), directedCalls(0), nearestCalls(0) {}
    bool IsPosNavigable(const C3DVector& p) const
    {
        CHECK(Same(p, C3DVector(6.0f,-2.0f,8.0f)));
        ++checked;
        return navigable;
    }
    bool GetNearbyDirectedPosition(const C3DVector& origin, float radius, const C3DVector& direction, float angle, C3DVector& out) const
    {
        CHECK(Same(origin, C3DVector(1.0f,3.0f,6.0f)) && Same(direction, C3DVector(5.0f,-5.0f,2.0f)));
        CHECK(radius == 15.0f && angle == (float)(30.0 / 360.0));
        ++directedCalls;
        out = C3DVector(13.0f, 12.0f, -17.0f);
        return directed;
    }
    bool GetNearestFleePosition(const C3DVector& origin, float radius, C3DVector& out) const
    {
        CHECK(Same(origin, C3DVector(1.0f,3.0f,6.0f)) && radius == 15.0f);
        ++nearestCalls;
        out = C3DVector(15.0f, 10.0f, 20.0f);
        return nearest;
    }
};
struct AICreature {
    unsigned __int64 id;
    C3DVector position;
    Navigation nav;
    Enemy enemy;
    Modes first, replacement;
    int cleanupMode;
    unsigned navReads, enemyReads, modeReads;
    AICreature() : id(0), cleanupMode(0), navReads(0), enemyReads(0), modeReads(0) {}
    unsigned __int64 GetUniqueID() const { return id; }
    const C3DVector& GetPos() const { return position; }
    bool GetTC(const Navigation** out) { ++navReads; *out = &nav; return true; }
    bool PeekTC(const Enemy** out) { ++enemyReads; *out = &enemy; return true; }
    bool GetTC(Modes** out)
    {
        ++modeReads;
        if (modeReads == 1)
            *out = &first;
        else if (cleanupMode == 1)
            *out = &replacement;
        return modeReads == 1 || cleanupMode != 2;
    }
};
struct Excluding {
    const AICreature& self;
    Excluding(const AICreature& subject) : self(subject) {}
    bool operator()(const AICreature& other) const { CHECK(&other != &self); return true; }
};
struct FactionFilter {
    const Faction* faction;
    FactionFilter(const Faction* value) : faction(value) {}
    bool operator()(const AICreature& other) const { CHECK(faction != &other.enemy.faction); return true; }
};
#define CThingAICreature AICreature
// Inline-budget: the native search helper calls the actual comparator at 0x00986C30.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
const CThingAICreature* CThingCompare_Nearest::operator()(const CThingAICreature* first, const CThingAICreature* second) const
{
    if (!first) return second;
    if (!second) return first;
    return (first->GetPos() - Pos).GetSquaredMagnitude() <
           (second->GetPos() - Pos).GetSquaredMagnitude() ? first : second;
}
#undef CThingAICreature

struct Search {
    AICreature ally, other;
    bool found;
    unsigned calls;
    Search() : found(false), calls(0)
    {
        ally.position = C3DVector(6.0f,-2.0f,8.0f);
        other.position = C3DVector(-2.0f,-2.0f,-2.0f);
    }
    bool GetPBestThing(const CFilter_Bind2<Excluding,FactionFilter>& filters, const CThingCompare_Nearest& nearest, const AICreature** out)
    {
        CHECK(filters(ally) && filters(other));
        ++calls;
        *out = found ? nearest(static_cast<const AICreature*>(&ally), static_cast<const AICreature*>(&other)) : 0;
        CHECK(!found || *out == &ally);
        return found;
    }
};

namespace FixtureSpeech { enum DialogueLayer { DIALOGUE_LAYER_BACKGROUND = 2 }; }
struct StateGroup {
    AICreature* ThingReactingTo;
    AICreature creature;
    Search search;
    unsigned moves, speech, routeIteration;
    EAIActionReturn movementResult, speechResult;
    StateGroup() : ThingReactingTo(0), moves(0), speech(0), routeIteration(0),
        movementResult(AI_ACTION_FINISHED), speechResult(AI_ACTION_FINISHED) {}
    AICreature& GetCreature() { return creature; }
    Search& PeekThingSearchTools() { return search; }
    unsigned long* GetPWorldSeed() { return &seed; }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector position, float radius, bool& route,
            EMovementType type, bool a, bool b, bool c, const AICreature* obstruct, bool d, float speed)
    {
        CHECK(repeat == ACTION_REPEAT_CALLBACK && type == AI_MOVEMENT_RUNNING && radius == 1.0f && speed == 0.0f);
        CHECK(a && !b && c && !obstruct && d);
        const bool allyRoute = creature.nav.directedCalls && creature.nav.directed;
        CHECK(Same(position, allyRoute ? C3DVector(13.0f,12.0f,-17.0f) : C3DVector(15.0f,10.0f,20.0f)));
        ++moves;
        CHECK(moves <= 3);
        route = routeIteration != 0 && routeIteration <= moves;
        World::frame += 80;
        return movementResult;
    }
    EAIActionReturn WaitForTextToLoadThenSpeak(const char* message, FixtureSpeech::DialogueLayer layer)
    {
        CHECK(std::strcmp(message, "TEXT_AI_OPINION_ACT_SCREAM_TO_SELF") == 0);
        CHECK(layer == FixtureSpeech::DIALOGUE_LAYER_BACKGROUND && draws == 1);
        ++speech;
        return speechResult;
    }
    bool FleeUpdate();
};

#define CAIStateGroup_OpinionReactionRoot StateGroup
#define CTCCreatureNavigation Navigation
#define CTCEnemy Enemy
#define CThingAICreature AICreature
#define CThingFilter_IsNot Excluding
#define CThingFilter_IsAlliedWithFaction FactionFilter
#define CTCCreatureModeManager Modes
#define CWorld World
#define NSpeechGainManager FixtureSpeech

const float FLEE_DISTANCE = 15.0f;
const float FLEE_MAX_DURATION_SECS = 10.0f;
const float FLEE_MIN_DURATION_SECS = 4.0f;
const float FLEE_TOWARDS_ALLIES_ANGLE_ACCURACY_DEGREES = 30.0f;

bool CAIStateGroup_OpinionReactionRoot::FleeUpdate()
{
    const CTCCreatureNavigation* ptc_nav;
    GetCreature().GetTC(&ptc_nav);
    C3DVector ret_pos;
    bool ret_bool = false;
    if (ThingReactingTo->GetUniqueID() == GetCreature().GetUniqueID()) {
        const CTCEnemy* ptc_enemy;
        GetCreature().PeekTC(&ptc_enemy);
        const CThingAICreature* pcreature;
        if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_IsNot(GetCreature()),
                CThingFilter_IsAlliedWithFaction(ptc_enemy->GetPFaction())), CThingCompare_Nearest(GetCreature().GetPos()),
                &pcreature) && ptc_nav->IsPosNavigable(pcreature->GetPos())) {
            ret_bool = ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(), FLEE_DISTANCE,
                pcreature->GetPos() - GetCreature().GetPos(), (float)(FLEE_TOWARDS_ALLIES_ANGLE_ACCURACY_DEGREES / 360.0), ret_pos);
        }
    }
    if (!ret_bool)
        ret_bool = ptc_nav->GetNearestFleePosition(GetCreature().GetPos(), FLEE_DISTANCE, ret_pos);
    if (ret_bool) {
        CTCCreatureModeManager* ptc_cmm;
        GetCreature().GetTC(&ptc_cmm);
        bool pushed_mode = ptc_cmm->AddMode(15);

        unsigned long frame_to_stop_fleeing = (unsigned long)(CWorld::GetFrame() +
            (GFFloatRandom(FLEE_MAX_DURATION_SECS - FLEE_MIN_DURATION_SECS, GetPWorldSeed()) +
                FLEE_MIN_DURATION_SECS) * CMainGameComponent::GetConstantFPS());
        EAIActionReturn ret = AI_ACTION_FAILED;
        bool just_found_route = false;
        do {
            ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, ret_pos, 1.0f, ret_bool,
                AI_MOVEMENT_RUNNING, true, false, true, 0, true, 0.0f);
            if (!just_found_route && ret_bool) {
                just_found_route = true;
                if (WaitForTextToLoadThenSpeak("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF",
                        NSpeechGainManager::DIALOGUE_LAYER_BACKGROUND) != AI_ACTION_FINISHED)
                    return false;
            }
        } while (ret == AI_ACTION_IN_PROGRESS && (unsigned long)CWorld::GetFrame() < frame_to_stop_fleeing);

        if (pushed_mode) {
            GetCreature().GetTC(&ptc_cmm);
            ptc_cmm->RemoveMode(15);
        }
        return ret == AI_ACTION_FINISHED || ret == AI_ACTION_IN_PROGRESS;
    }
    return false;
}

static void CheckComparator()
{
    CThingCompare_Nearest nearest(C3DVector(1.0f, 3.0f, 6.0f));
    const AICreature* absent = 0;
    AICreature a, b;
    const AICreature* first = &a;
    const AICreature* second = &b;
    a.position = C3DVector(0.0f, 3.0f, 6.0f);
    b.position = C3DVector(4.0f, 3.0f, 6.0f);
    CHECK(nearest(absent, absent) == absent);
    CHECK(nearest(absent, second) == &b);
    CHECK(nearest(first, absent) == &a);
    CHECK(nearest(first, second) == &a);
    CHECK(nearest(second, first) == &a);
    b.position = C3DVector(2.0f, 3.0f, 6.0f);
    CHECK(nearest(first, second) == &b);
}

int main()
{
    CheckComparator();
    const unsigned scenarios = 3 * 16 * 2 * 3 * 2 * 5 * 3;
    for (unsigned scenario = 0; scenario != scenarios; ++scenario) {
        unsigned flags = scenario;
        const unsigned matchingId = flags % 3; flags /= 3;
        const bool foundAlly = (flags & 1) != 0;
        const bool navigable = (flags & 2) != 0;
        const bool directed = (flags & 4) != 0;
        const bool nearest = (flags & 8) != 0;
        const bool pushesMode = (flags & 16) != 0;
        flags /= 32;
        const unsigned cleanup = flags % 3; flags /= 3;
        const bool speak = (flags & 1) != 0; flags /= 2;
        const EAIActionReturn movement = (EAIActionReturn)(flags % 5); flags /= 5;
        const unsigned route = flags % 3;

        StateGroup state;
        state.creature.position = C3DVector(1.0f,3.0f,6.0f);
        state.creature.id = 0x1337A5A512345678ui64;
        AICreature threat;
        threat.id = state.creature.id;
        if (matchingId == 1)
            threat.id ^= 0x100000000ui64;
        if (matchingId == 2)
            threat.id ^= 1;
        state.ThingReactingTo = &threat;
        state.search.found = foundAlly;
        state.creature.nav.navigable = navigable;
        state.creature.nav.directed = directed;
        state.creature.nav.nearest = nearest;
        state.creature.first.mayPush = pushesMode;
        state.creature.cleanupMode = cleanup;
        state.routeIteration = route;
        state.movementResult = movement;
        state.speechResult = speak ? AI_ACTION_FINISHED : AI_ACTION_IN_PROGRESS;
        draws = 0;
        randomValue = cleanup ? 5.5f : 0.0f;
        World::frame = 100;

        const bool triedAlly = matchingId == 0 && foundAlly;
        const bool triedDirected = triedAlly && navigable;
        const bool directedRoute = triedDirected && directed;
        const bool reachedRoute = directedRoute || nearest;
        const unsigned maxMoves = reachedRoute ? (movement == AI_ACTION_IN_PROGRESS && cleanup ? 3 : 1) : 0;
        const bool screams = route > 0 && route <= maxMoves;
        const bool returnsEarly = screams && !speak;
        const bool removes = reachedRoute && pushesMode && !returnsEarly;
        const bool result = state.FleeUpdate();

        CHECK(result == (reachedRoute && !returnsEarly && (movement == AI_ACTION_FINISHED || movement == AI_ACTION_IN_PROGRESS)));
        CHECK(state.creature.navReads == 1);
        CHECK(state.creature.enemyReads == (matchingId ? 0U : 1U) && state.search.calls == state.creature.enemyReads);
        CHECK(state.creature.nav.checked == (triedAlly ? 1U : 0U));
        CHECK(state.creature.nav.directedCalls == (triedDirected ? 1U : 0U));
        CHECK(state.creature.nav.nearestCalls == (directedRoute ? 0U : 1U));
        CHECK(draws == (reachedRoute ? 1U : 0U) && state.creature.first.adds == draws);
        CHECK(state.moves == (returnsEarly ? route : maxMoves) && state.speech == (screams ? 1U : 0U));
        CHECK(state.creature.modeReads == draws + (removes ? 1U : 0U));
        CHECK(state.creature.first.removes == (removes && cleanup != 1 ? 1U : 0U));
        CHECK(state.creature.replacement.removes == (removes && cleanup == 1 ? 1U : 0U));
    }
    std::puts(failures ? "00984f60_TEST FAIL" : "00984f60_TEST PASS");
    return failures ? 1 : 0;
}