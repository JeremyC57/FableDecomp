// Behaviour test for 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp(C3DVector, unsigned long, bool): a host model of
// the row's control flow over stand-in game objects (the callees are out-of-line game functions), driven by scripted results of the
// proximity move, the speech, the stop flag, the entity events and the world frame; each exit and every side effect is checked.
#include <cstdio>
#include <cstring>

enum { STOPPING = 1, DONE = 2, ABORTED = 3, RUNNING = 4 };
enum { SPEECH_STARTED = 0, SPEECH_FAILED = 1, SPEECH_RETRY = 2 };
enum { APOLOGY_ACCEPTED = 78, APOLOGY_REFUSED = 79 };

struct Village {
    bool enemyBecauseOfCrime; bool heroCriminal; long frameBribeEnds;
    long apologiseableRemoved; long crimeRemoved; int apologised;
};
struct Opinion { int reports; Opinion* reportedTo; };
struct Event { long type; const char* expression; bool removed; };
struct Creature {
    bool hasVillageMember; Village* village;
    bool hasTalk; bool hasEvents; Event* events; int eventCount;
    bool hasOpinion; Opinion opinion; bool hasDeedLog;
    Creature* lastAttackedBy; int stopSpeakingCalls;
    long deedPosted; int deedCount;
};
struct Script {
    // inputs
    bool defFound; long lifespan; Creature* guard;
    const int* moveResults; const bool* movingFlags; int moveCount;
    const int* speechResults; int speechCount; const bool* stopping; int stopCount;
    const int* frames; int frameCount; bool stillValid; Creature* hero;
    // progress / outputs
    int moveIndex, speechIndex, stopIndex, frameIndex, yields, messages, playCalls;
};

static int Frame(Script& s) { int f = s.frames[s.frameIndex < s.frameCount ? s.frameIndex : s.frameCount - 1]; ++s.frameIndex; return f; }

static int Run(Creature& me, Script& s, unsigned long crime)
{
    int result = ABORTED;
    if (!s.defFound) return ABORTED;
    Creature* guard = s.guard;
    if (!guard) return ABORTED;
    int last_yell_frame = (int)0x80000000;               // retail's 32-bit `sub ; cmp ; jle` wraps: frame - INT_MIN
    int frames_between_yells = (int)(s.lifespan * 0.8f);
    bool spoken = false;
    do {
        Village* village = me.hasVillageMember ? me.village : 0;
        if (village) {
            if (village->enemyBecauseOfCrime) { result = DONE; break; }
            bool moving = false;
            result = s.moveResults[s.moveIndex]; moving = s.movingFlags[s.moveIndex]; ++s.moveIndex;
            if (!spoken && moving && (result == RUNNING || result == DONE)) {
                spoken = true;
                if (me.hasTalk) {
                    int speech;
                    bool stop;
                    do {
                        speech = s.speechResults[s.speechIndex++]; ++s.playCalls;
                        if (speech != SPEECH_RETRY) break;
                        ++s.yields;
                        stop = s.stopping[s.stopIndex++];
                    } while (!stop);
                    if (speech == SPEECH_RETRY) { ++me.stopSpeakingCalls; return STOPPING; }
                }
            }
            if (me.hasEvents) {
                for (int i = 0; i < me.eventCount; ++i) {
                    Event& e = me.events[i];
                    if (e.removed) continue;
                    if (e.type == 3 && e.expression && e.expression[0] == 'E' && e.expression[11] == 'A') {
                        e.removed = true;
                        village->apologiseableRemoved = 10;
                        village->crimeRemoved = (long)crime;
                        ++village->apologised;
                        Creature* hero = s.hero;
                        if (hero) {
                            hero->deedPosted = s.stillValid ? APOLOGY_REFUSED : APOLOGY_ACCEPTED; ++hero->deedCount;
                            if (!village->heroCriminal && me.lastAttackedBy == hero) me.lastAttackedBy = 0;
                        }
                        return ABORTED;
                    }
                }
            }
        }
        if ((int)((unsigned int)Frame(s) - (unsigned int)last_yell_frame) > frames_between_yells) { ++s.messages; last_yell_frame = Frame(s); }
    } while (result == RUNNING);
    if (result != DONE) return result;
    if (guard) {
        Village* gv = guard->hasVillageMember ? guard->village : 0;
        if (gv) gv->frameBribeEnds = 0;
        if (guard->hasOpinion && me.hasOpinion) { ++me.opinion.reports; me.opinion.reportedTo = &guard->opinion; }
    }
    return DONE;
}

template<class T> static T Zero() { T t; std::memset(&t, 0, sizeof t); return t; }
static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(x) Check((x), __LINE__)

int main()
{
    static const int frames[] = { 100, 100, 141, 141, 142, 142, 143, 143 };
    // the first yell needs a frame within 2^31 above INT_MIN (the wrapped difference is negative for every frame >= 0)
    static const int early[] = { -2147483000, -2147483000, -2147482900, -2147482900, -2147482899, -2147482899, -2147482899, -2147482899 };
    // 1. no definition / no guard: aborted.
    {
        Creature me = Zero<Creature>(); Script s = Zero<Script>(); s.defFound = false;
        CHECK(Run(me, s, 7) == ABORTED);
        s.defFound = true; s.guard = 0;
        CHECK(Run(me, s, 7) == ABORTED);
    }
    // 2. run to the guard: running, running, done; the speech plays once after one retry; one yell then one every 40 frames;
    //    the guard's village bribe is reset and the opinion reports the crimes.
    {
        Village v = Zero<Village>(); v.frameBribeEnds = 55;
        Creature guard = Zero<Creature>(); guard.hasVillageMember = true; guard.village = &v; guard.hasOpinion = true;
        Creature me = Zero<Creature>(); me.hasVillageMember = true; me.village = &v; me.hasTalk = true; me.hasOpinion = true;
        static const int moves[] = { RUNNING, RUNNING, DONE }; static const bool moving[] = { false, true, true };
        static const int speech[] = { SPEECH_RETRY, SPEECH_STARTED }; static const bool stop[] = { false };
        Script s = Zero<Script>(); s.defFound = true; s.lifespan = 50; s.guard = &guard;
        s.moveResults = moves; s.movingFlags = moving; s.moveCount = 3; s.speechResults = speech; s.speechCount = 2;
        s.stopping = stop; s.stopCount = 1; s.frames = early; s.frameCount = 8;
        CHECK(Run(me, s, 7) == DONE);
        CHECK(s.moveIndex == 3 && s.playCalls == 2 && s.yields == 1);
        CHECK(s.messages == 2);
        CHECK(v.frameBribeEnds == 0 && me.opinion.reports == 1 && me.opinion.reportedTo == &guard.opinion);
    }
    // 3. stopping while the speech keeps failing: StopSpeaking and AI_ACTION_STOPPING.
    {
        Village v = Zero<Village>();
        Creature guard = Zero<Creature>();
        Creature me = Zero<Creature>(); me.hasVillageMember = true; me.village = &v; me.hasTalk = true;
        static const int moves[] = { RUNNING }; static const bool moving[] = { true };
        static const int speech[] = { SPEECH_RETRY, SPEECH_RETRY }; static const bool stop[] = { false, true };
        Script s = Zero<Script>(); s.defFound = true; s.lifespan = 50; s.guard = &guard;
        s.moveResults = moves; s.movingFlags = moving; s.moveCount = 1; s.speechResults = speech; s.speechCount = 2;
        s.stopping = stop; s.stopCount = 2; s.frames = frames; s.frameCount = 8;
        CHECK(Run(me, s, 7) == STOPPING && me.stopSpeakingCalls == 1 && s.yields == 2);
    }
    // 4. the hero apologised: the event is removed, the crimes cleared, the deed posted (accepted unless the group is still valid)
    //    and the attacker cleared when the hero is no criminal; aborted.
    {
        for (int valid = 0; valid < 2; ++valid) {
            Village v = Zero<Village>(); v.heroCriminal = false;
            Creature hero = Zero<Creature>();
            Creature guard = Zero<Creature>();
            Event events[2] = { { 3, "EXPRESSION_LAUGH", false }, { 3, "EXPRESSION_APOLOGY", false } };
            Creature me = Zero<Creature>(); me.hasVillageMember = true; me.village = &v; me.hasEvents = true; me.events = events;
            me.eventCount = 2; me.lastAttackedBy = &hero;
            static const int moves[] = { RUNNING }; static const bool moving[] = { false };
            Script s = Zero<Script>(); s.defFound = true; s.lifespan = 50; s.guard = &guard; s.hero = &hero; s.stillValid = valid != 0;
            s.moveResults = moves; s.movingFlags = moving; s.moveCount = 1; s.frames = frames; s.frameCount = 8;
            CHECK(Run(me, s, 9) == ABORTED);
            CHECK(!events[0].removed && events[1].removed);
            CHECK(v.apologiseableRemoved == 10 && v.crimeRemoved == 9 && v.apologised == 1);
            CHECK(hero.deedCount == 1 && hero.deedPosted == (valid ? APOLOGY_REFUSED : APOLOGY_ACCEPTED));
            CHECK(me.lastAttackedBy == 0);
            CHECK(s.messages == 0);
        }
    }
    // 5. the village is hostile because of crime: straight to the guard handover (done) without moving.
    {
        Village v = Zero<Village>(); v.enemyBecauseOfCrime = true; v.frameBribeEnds = 3;
        Creature guard = Zero<Creature>(); guard.hasVillageMember = true; guard.village = &v;
        Creature me = Zero<Creature>(); me.hasVillageMember = true; me.village = &v;
        Script s = Zero<Script>(); s.defFound = true; s.lifespan = 50; s.guard = &guard; s.frames = frames; s.frameCount = 8;
        CHECK(Run(me, s, 7) == DONE && s.moveIndex == 0 && v.frameBribeEnds == 0);
    }
    // 6. no village: no yell for a non-negative frame (the wrapped difference), the initial abort result ends the loop.
    {
        Creature guard = Zero<Creature>();
        Creature me = Zero<Creature>();
        Script s = Zero<Script>(); s.defFound = true; s.lifespan = 50; s.guard = &guard; s.frames = frames; s.frameCount = 8;
        CHECK(Run(me, s, 7) == ABORTED && s.messages == 0);
    }
    if (fails) { std::printf("0097fea0_TEST FAIL\n"); return 1; }
    std::printf("0097fea0_TEST PASS\n");
    return 0;
}
