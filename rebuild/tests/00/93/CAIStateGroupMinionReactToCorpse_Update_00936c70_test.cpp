#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_DISTANCE_PASS26
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/fable_ai_action_enums.h"
#include "stlport/fable_containers.h"
#include <cstdio>
#include <cstring>

static unsigned failures;
#define CHECK(expr) do { if (!(expr)) { \
    if (failures < 20) std::printf("line %d: %s\n", __LINE__, #expr); ++failures; \
} } while (0)

struct Creature;
struct Discovery {
    bool discovered;
    unsigned changes;
    Creature* observer;

    Discovery() : discovered(false), changes(0), observer(0) {}
    bool IsDiscovered() const { return discovered; }
    void SetDiscovered(bool value, Creature* discoverer)
    {
        CHECK(value && !discovered);
        discovered = value;
        observer = discoverer;
        ++changes;
    }
};

struct DeadThing {
    bool dead;
    Discovery* discovery;
    Creature* killer;
    mutable unsigned discoveryReads, killerReads, positionReads;
    C3DVector position;

    DeadThing() : dead(false), discovery(0), killer(0), discoveryReads(0), killerReads(0), positionReads(0) {}
    bool GetTC(Discovery** out) const
    {
        ++discoveryReads;
        if (!discovery)
            return false;
        *out = discovery;
        return true;
    }
    const Creature* GetKiller() const { ++killerReads; return killer; }
    const C3DVector& PeekPos() const { ++positionReads; return position; }
};

struct Talk {};
struct Creature {
    bool player;
    unsigned long property;
    mutable unsigned playerChecks, talkLookups;

    Creature() : player(false), property(0), playerChecks(0), talkLookups(0) {}
    bool IsPlayerCharacter() const { ++playerChecks; return player; }
    unsigned long GetProperty() const { return property; }
    bool GetTC(Talk**) const { ++talkLookups; return false; }
};

struct IntelligentPointer {
    DeadThing* item;
    DeadThing* operator->() const { return item; }
};
static bool IS_THING_ALIVE(const DeadThing* item) { return item && !item->dead; }
static bool IS_THING_ALIVE(const IntelligentPointer& pointer) { return IS_THING_ALIVE(pointer.item); }

struct CThingFilter_None {};
struct AreaSearch {
    DeadThing* candidates[2];
    unsigned searches;

    AreaSearch() : searches(0) { candidates[0] = candidates[1] = 0; }
    template<class T> void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_None&, std::list<T*>& result)
    {
        CHECK(area.TLX == -3 && area.TLY == -8 && area.BRX == 7 && area.BRY == 2);
        ++searches;
        result.push_back(candidates[0]);
        result.push_back(candidates[1]);
    }
};

struct MessageDef {};
static unsigned references;
template<class T> struct DefPointer {
    bool live;
    DefPointer() : live(false) {}
    DefPointer(const DefPointer& other) : live(other.live) { if (live) ++references; }
    ~DefPointer() { if (live) --references; }
    void Reset() { CHECK(!live); live = true; ++references; }
};

struct MessageManager {
    bool found;
    unsigned lookups;
    MessageManager() : found(false), lookups(0) {}
    bool GetDef(const char* name, DefPointer<const MessageDef>* out)
    {
        CHECK(std::strcmp(name, "STANDARD_MESSAGE_EVENT_FOUND_CORPSE") == 0);
        ++lookups;
        if (found)
            out->Reset();
        return found;
    }
};

struct Sweep {
    Creature& creature;
    explicit Sweep(Creature& subject) : creature(subject) {}
};

struct StateGroup {
    IntelligentPointer PDeadThing;
    Creature creature;
    AreaSearch search;
    MessageManager messages;
    EAIActionReturn moveResult, sweepResult;
    int changeAfterMove;
    unsigned moves, terminations, sweeps, posts;

    StateGroup() : moveResult(AI_ACTION_FINISHED), sweepResult(AI_ACTION_FINISHED), changeAfterMove(0),
        moves(0), terminations(0), sweeps(0), posts(0) {}
    void Terminate() { ++terminations; }
    Creature& GetCreature() { return creature; }
    AreaSearch& PeekThingSearchTools() { return search; }
    MessageManager& PeekDefinitionManager() { return messages; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, IntelligentPointer target,
            float radius, bool& route, EMovementType movement, bool a, bool b, bool c, bool d, float speed)
    {
        CHECK(repeat == ACTION_REPEAT_UNTIL_FINISHED && movement == AI_MOVEMENT_RUNNING);
        CHECK(target.item == PDeadThing.item && !route && radius == 2.0f && speed == 0.0f && a && !b && c && d);
        ++moves;
        route = true;
        if (changeAfterMove == 1)
            PDeadThing.item = 0;
        if (changeAfterMove == 2)
            PDeadThing.item->dead = true;
        return moveResult;
    }
    EAIActionReturn AIAction_PerformActionAndWait(const Sweep& sweep, bool wait)
    {
        CHECK(&sweep.creature == &creature && !wait);
        ++sweeps;
        return sweepResult;
    }
    void AIAction_FindAGuardAndGiveMessage(DefPointer<const MessageDef> def, C3DVector position, long flags)
    {
        CHECK(def.live && references == 2 && flags == 0);
        CHECK(position.X == 2.5f && position.Y == -3.9f && position.Z == 4.0f);
        ++posts;
    }
    void Update();
};

#define CAIStateGroup_MinionReactToCorpse StateGroup
#define CThingDeadCreature DeadThing
#define CTCDiscovery Discovery
#define CTCTalk Talk
#define CCreatureAction_OrderSecuritySweep Sweep
#define CDefPointer DefPointer
#define CMessageEventDef MessageDef

const float DISTANCE_FROM_CORPSE_TO_NOTICE_IT = 5.0f;
const float PROXIMITY_TO_LOOK_AT_BODY = 2.0f;

void CAIStateGroup_MinionReactToCorpse::Update()
{
    bool player_did_it = false;
    if (!IS_THING_ALIVE(PDeadThing)) {
        Terminate();
        return;
    }

    bool nav_route_calculated = false;
    EAIActionReturn ret = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PDeadThing,
        PROXIMITY_TO_LOOK_AT_BODY, nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PDeadThing)) {
        Terminate();
        return;
    }

    CTCDiscovery* tc_discovery;
    if (IS_THING_ALIVE(PDeadThing) && PDeadThing->GetTC(&tc_discovery) && !tc_discovery->IsDiscovered()) {
        tc_discovery->SetDiscovered(true, &GetCreature());
        if (PDeadThing->GetKiller() && PDeadThing->GetKiller()->IsPlayerCharacter())
            player_did_it = true;
    }

    std::list<CThingDeadCreature*> dead_things;
    C2DBoxI area(PDeadThing->PeekPos().ToC2DCoordI(), (long)DISTANCE_FROM_CORPSE_TO_NOTICE_IT);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingDeadCreature>(area, CThingFilter_None(), dead_things);
    for (std::list<CThingDeadCreature*>::iterator it = dead_things.begin(); it != dead_things.end(); ++it) {
        CThingDeadCreature* dead_it = *it;
        if (IS_THING_ALIVE(dead_it) && !dead_it->GetTC(&tc_discovery) && !tc_discovery->IsDiscovered()) {
            if (GFGet2DDistanceSquared(C2DVector(PDeadThing->PeekPos()), C2DVector(dead_it->PeekPos())) <=
                    GFSqr(DISTANCE_FROM_CORPSE_TO_NOTICE_IT)) {
                tc_discovery->SetDiscovered(true, &GetCreature());
                if (dead_it->GetKiller() && dead_it->GetKiller()->IsPlayerCharacter())
                    player_did_it = true;
            }
        }
    }

    CTCTalk* ptc_lipsync;
    GetCreature().GetTC(&ptc_lipsync);
    if (GetCreature().GetProperty() == 3) {
        if (player_did_it) {
            if (AIAction_PerformActionAndWait(CCreatureAction_OrderSecuritySweep(GetCreature()), false) != AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
        }
    }
    else {
        CDefPointer<const CMessageEventDef> pdef;
        if (PeekDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_FOUND_CORPSE", &pdef)) {
            AIAction_FindAGuardAndGiveMessage(pdef, PDeadThing->PeekPos(), 0);
        }
    }
}

int main()
{
    const float range[3] = { 0.0f, 5.0f, 5.125f };
    unsigned cases = 0;
    for (int start = 0; start != 3; ++start) {
        for (int afterMove = 0; afterMove != 3; ++afterMove) {
            for (int motion = 0; motion != 3; ++motion) {
                StateGroup state;
                DeadThing corpse;
                corpse.position = C3DVector(2.5f, -3.9f, 4.0f);
                corpse.dead = start == 1;
                state.PDeadThing.item = start == 0 ? 0 : &corpse;
                state.changeAfterMove = afterMove;
                state.moveResult = (EAIActionReturn)motion;
                state.Update();
                bool reached = start == 2 && afterMove == 0 && motion == AI_ACTION_FINISHED;
                CHECK(state.moves == (start == 2 ? 1U : 0U));
                CHECK(state.terminations == (reached ? 0U : 1U));
                CHECK(state.search.searches == (reached ? 1U : 0U));
                CHECK(state.messages.lookups == (reached ? 1U : 0U));
                CHECK(corpse.discoveryReads == (reached ? 1U : 0U));
                CHECK(corpse.killerReads == 0 && state.sweeps == 0 && state.posts == 0 && references == 0);
                ++cases;
            }
        }
    }

    for (unsigned scenario = 0; scenario != 15552; ++scenario) {
        unsigned digits = scenario;
        const int firstStatus = digits % 3; digits /= 3;
        const int firstKiller = digits % 3; digits /= 3;
        const int secondStatus = digits % 4; digits /= 4;
        const int lastStatus = digits % 3; digits /= 3;
        const int lastRange = digits % 3; digits /= 3;
        const int lastKiller = digits % 3; digits /= 3;
        const bool guardProperty = (digits & 1) != 0;
        const bool sweepComplete = (digits & 2) != 0;
        const bool messageFound = (digits & 4) != 0;
        const bool secondDiscovered = (digits & 8) != 0;

        StateGroup state;
        Discovery firstDiscovery, secondDiscovery;
        DeadThing corpse, second, last;
        Creature firstPerpetrator, lastPerpetrator;
        state.PDeadThing.item = &corpse;
        state.creature.property = guardProperty ? 3 : 4;
        state.sweepResult = sweepComplete ? AI_ACTION_FINISHED : (EAIActionReturn)1;
        state.messages.found = messageFound;
        corpse.position = C3DVector(2.5f, -3.9f, 4.0f);
        corpse.discovery = firstStatus ? &firstDiscovery : 0;
        corpse.killer = firstKiller ? &firstPerpetrator : 0;
        firstPerpetrator.player = firstKiller == 2;
        firstDiscovery.discovered = firstStatus == 2;

        second.dead = secondStatus == 1;
        second.discovery = secondStatus == 3 ? &secondDiscovery : 0;
        secondDiscovery.discovered = secondDiscovered;
        second.position = C3DVector(30.0f, -3.9f, 4.0f);
        state.search.candidates[0] = secondStatus ? &second : 0;
        last.dead = lastStatus == 1;
        last.killer = lastKiller ? &lastPerpetrator : 0;
        lastPerpetrator.player = lastKiller == 2;
        last.position = C3DVector(2.5f + range[lastRange], -3.9f, 600.0f);
        state.search.candidates[1] = lastStatus ? &last : 0;

        // The original branch retains the previous lookup output when GetTC returns false.
        // Supply a successful lookup before any case that exercises that retained pointer.
        if ((!firstStatus && secondStatus == 2) ||
                (!firstStatus && secondStatus != 3 && lastStatus == 2))
            continue;

        const bool discoveredCorpse = firstStatus == 1;
        const bool consideredLast = secondStatus == 3 && !secondDiscovered && lastStatus == 2;
        const bool discoveredLast = consideredLast && lastRange != 2;
        const bool playerKilled = (discoveredCorpse && firstKiller == 2) || (discoveredLast && lastKiller == 2);
        const bool swept = guardProperty && playerKilled;
        const bool posted = !guardProperty && messageFound;

        state.Update();

        CHECK(state.moves == 1 && state.search.searches == 1 && state.creature.talkLookups == 1);
        CHECK(corpse.discoveryReads == 1 && firstDiscovery.changes == (discoveredCorpse ? 1U : 0U));
        CHECK(second.discoveryReads == (secondStatus >= 2 ? 1U : 0U) && last.discoveryReads == (lastStatus == 2 ? 1U : 0U));
        CHECK(corpse.killerReads == (discoveredCorpse ? (firstKiller ? 2U : 1U) : 0U));
        CHECK(firstPerpetrator.playerChecks == (discoveredCorpse && firstKiller ? 1U : 0U));
        CHECK(second.positionReads == 0 && second.killerReads == 0);
        CHECK(last.positionReads == (consideredLast ? 1U : 0U));
        CHECK(last.killerReads == (discoveredLast ? (lastKiller ? 2U : 1U) : 0U));
        CHECK(lastPerpetrator.playerChecks == (discoveredLast && lastKiller ? 1U : 0U));
        CHECK(firstDiscovery.observer == (discoveredCorpse ? &state.creature : 0));
        CHECK(secondDiscovery.observer == (discoveredLast ? &state.creature : 0));
        CHECK(secondDiscovery.changes == (discoveredLast ? 1U : 0U));
        CHECK(state.terminations == (swept && !sweepComplete ? 1U : 0U));
        CHECK(state.sweeps == (swept ? 1U : 0U));
        CHECK(state.messages.lookups == (guardProperty ? 0U : 1U) && state.posts == (posted ? 1U : 0U));
        CHECK(references == 0);
        ++cases;
    }
    CHECK(cases == 13419);
    std::puts(failures ? "00936c70_TEST FAIL" : "00936c70_TEST PASS");
    return failures ? 1 : 0;
}