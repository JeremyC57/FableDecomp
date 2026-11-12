// Behaviour test for NScript::CV_AmbushScamScript::TricksterApproach 0x00E2EAC0: the same control flow run through a small
// mock of the script interface (frame counter, terminate-call counter, a counted trickster handle, scripted health / distance).
#include <cstdio>

struct Sim {
    int frames;             // NewScriptFrame calls so far
    int termCalls;          // IsActiveThreadTerminating calls so far
    int terminateAtCall;    // the Nth terminate check (1-based) and every later one answers true; 0 = never
    bool tricksterExists;   // GetTrickster returns a non-null thing
    int dieAtFrame;         // the thing stops being alive from this frame on (-1 = never)
    int noHealthAtFrame;    // health drops to 0.0001f from this frame on (-1 = never)
    int nearAtFrame;        // the thing is within the proximity from this frame on (-1 = never)
    float proximity;        // TricksterApproachProximity
    float proximityAsked;   // last distance bound passed to the distance test
    int liveThings;         // constructed minus destroyed trickster handles (must end at 0)
    int healthCalls, distanceCalls;
    bool TricksterKilledBeforeAmbush, DoTricksterIntro;
    void NewScriptFrame() { ++frames; }
    bool IsActiveThreadTerminating() { ++termCalls; return terminateAtCall != 0 && termCalls >= terminateAtCall; }
};

struct Thing {              // the by-value CScriptThing: a counted handle with a destructor at every exit
    Sim* sim;
    bool data;
    Thing(Sim* s, bool d) : sim(s), data(d) { ++sim->liveThings; }
    Thing(const Thing& o) : sim(o.sim), data(o.data) { ++sim->liveThings; }
    ~Thing() { --sim->liveThings; }
    bool IsAlive() const { return data && !(sim->dieAtFrame >= 0 && sim->frames >= sim->dieAtFrame); }
};

static Thing GetTrickster(Sim& s) { return Thing(&s, s.tricksterExists); }
static float GetHealth(Sim& s, const Thing&) { ++s.healthCalls; return (s.noHealthAtFrame >= 0 && s.frames >= s.noHealthAtFrame) ? 0.0001f : 50.0f; }
static bool IsDistanceBetweenThingsUnder(Sim& s, const Thing&, float bound)
{
    ++s.distanceCalls;
    s.proximityAsked = bound;
    return s.nearAtFrame >= 0 && s.frames >= s.nearAtFrame;
}

static void TricksterApproach(Sim& s)
{
    Thing trickster = GetTrickster(s);
    for (;;) {
        do { if (s.IsActiveThreadTerminating()) return; } while (0);
        if (!trickster.IsAlive() || GetHealth(s, trickster) <= 0.0001f) {
            do {
                do { if (s.IsActiveThreadTerminating()) return; } while (0);
            } while (0);
            s.TricksterKilledBeforeAmbush = true;
            break;
        }
        if (IsDistanceBetweenThingsUnder(s, trickster, s.proximity)) {
            do { if (s.IsActiveThreadTerminating()) return; } while (0);
            s.DoTricksterIntro = true;
            break;
        }
        s.NewScriptFrame();
    }
    do { if (s.IsActiveThreadTerminating()) return; } while (0);
}

static Sim Fresh()
{
    Sim s;
    s.frames = 0; s.termCalls = 0; s.terminateAtCall = 0; s.tricksterExists = true;
    s.dieAtFrame = -1; s.noHealthAtFrame = -1; s.nearAtFrame = -1;
    s.proximity = 12.5f; s.proximityAsked = -1.0f; s.liveThings = 0; s.healthCalls = 0; s.distanceCalls = 0;
    s.TricksterKilledBeforeAmbush = false; s.DoTricksterIntro = false;
    return s;
}

static int Check(const Sim& s, bool killed, bool intro, int frames, int termCalls, int healthCalls, int distanceCalls)
{
    return s.TricksterKilledBeforeAmbush == killed && s.DoTricksterIntro == intro && s.frames == frames
        && s.termCalls == termCalls && s.healthCalls == healthCalls && s.distanceCalls == distanceCalls && s.liveThings == 0;
}

int main()
{
    int ok = 1;
    {   // 1: no trickster at all -> killed at once; no health / distance query; checks: head, dead arm, after the loop
        Sim s = Fresh(); s.tricksterExists = false; TricksterApproach(s);
        ok &= Check(s, true, false, 0, 3, 0, 0);
    }
    {   // 2: trickster dies during frame 3 -> three waits, then killed; health asked while alive only
        Sim s = Fresh(); s.dieAtFrame = 3; TricksterApproach(s);
        ok &= Check(s, true, false, 3, 6, 3, 3);
    }
    {   // 3: health reaches exactly 0.0001f at frame 2 (<= counts as dead)
        Sim s = Fresh(); s.noHealthAtFrame = 2; TricksterApproach(s);
        ok &= Check(s, true, false, 2, 5, 3, 2);
    }
    {   // 4: the hero comes near at frame 4 -> intro, not killed; the member proximity is the bound
        Sim s = Fresh(); s.nearAtFrame = 4; TricksterApproach(s);
        ok &= Check(s, false, true, 4, 7, 5, 5) && s.proximityAsked == 12.5f;
    }
    {   // 5: dead and near in the same frame -> the dead test comes first
        Sim s = Fresh(); s.dieAtFrame = 1; s.nearAtFrame = 1; TricksterApproach(s);
        ok &= Check(s, true, false, 1, 4, 1, 1);
    }
    {   // 6: terminating at the very first check -> nothing queried, nothing set
        Sim s = Fresh(); s.terminateAtCall = 1; s.nearAtFrame = 0; TricksterApproach(s);
        ok &= Check(s, false, false, 0, 1, 0, 0);
    }
    {   // 7: terminating at the dead arm's own check -> the flag is NOT set
        Sim s = Fresh(); s.tricksterExists = false; s.terminateAtCall = 2; TricksterApproach(s);
        ok &= Check(s, false, false, 0, 2, 0, 0);
    }
    {   // 8: terminating at the near arm's own check -> the flag is NOT set
        Sim s = Fresh(); s.nearAtFrame = 0; s.terminateAtCall = 2; TricksterApproach(s);
        ok &= Check(s, false, false, 0, 2, 1, 1);
    }
    {   // 9: terminating at the check after the loop -> the flag set in the arm stays (it was set before the check)
        Sim s = Fresh(); s.nearAtFrame = 0; s.terminateAtCall = 3; TricksterApproach(s);
        ok &= Check(s, false, true, 0, 3, 1, 1);
    }
    {   // 10: terminating while waiting (head check after the second frame) -> leaves the loop with nothing set
        Sim s = Fresh(); s.terminateAtCall = 3; TricksterApproach(s);
        ok &= Check(s, false, false, 2, 3, 2, 2);
    }
    if (!ok) { std::printf("00e2eac0_TEST FAIL\n"); return 1; }
    std::printf("00e2eac0_TEST PASS\n");
    return 0;
}