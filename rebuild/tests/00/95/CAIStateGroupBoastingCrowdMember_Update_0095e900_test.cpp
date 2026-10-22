// Behaviour test for CAIStateGroup_BoastingCrowdMember::Update @ 0x0095E900: crowd member repositions near the hero or looks at him.
#include <cstdio>
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float Get2DLength() const { return (float)sqrt(X * X + Y * Y); }
};
struct CThing {
    C3DVector pos; unsigned char m_flags91;
    CThing() : m_flags91(0) { }
    const C3DVector& GetPosition() const { return pos; }
};
struct CTCCreatureGroupBoastingCrowd {
    CThing* hero; bool offPodium;
    bool CurrentStateIsOffPodiumType() const { return offPodium; }
};
struct CTCCreatureNavigation {
    bool directedOk; int directedCalls, wanderCalls; float lastRadius; C3DVector lastDir;
    bool GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float, C3DVector* out)
    {
        ++directedCalls; lastDir = direction;
        *out = C3DVector(from.X + distance, from.Y, from.Z);
        return directedOk;
    }
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool)
    {
        ++wanderCalls; lastRadius = radius; *out = from; return true;
    }
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
static EAIActionReturn g_script[32];
static char g_kind[32];
static int g_step = 0, g_steps = 0;
struct CAIStateGroup_BoastingCrowdMember {
    bool runnable, terminated;
    CThing* creature; CTCCreatureGroupBoastingCrowd* crowd; CTCCreatureNavigation* nav;
    C3DVector lastDest; CThing* lastLook; unsigned long seed;
    bool IsRunnable() { return runnable; }
    void Terminate() { terminated = true; }
    EAIActionReturn Next(char kind) { g_kind[g_step] = kind; return g_step < g_steps ? g_script[g_step++] : (g_step++, AI_ACTION_ABORTED); }
    EAIActionReturn AIAction_LookAtThing(CThing* thing) { lastLook = thing; return Next('L'); }
    EAIActionReturn AIAction_MoveToPosProximity(const C3DVector& pos) { lastDest = pos; return Next('M'); }
    void Update();
};
static float GFFloatRandom(float range, unsigned long*) { return range * 0.5f; }

void CAIStateGroup_BoastingCrowdMember::Update()
{
    if (IsRunnable()) {
        CThing* hero = crowd->hero;
        if (hero && !(hero->m_flags91 & 1)) {
            bool moving = false;
            bool first = true;
            C3DVector heroPos = hero->GetPosition();
            C3DVector dest;
            for (;;) {
                EAIActionReturn result;
                if (first || !crowd->CurrentStateIsOffPodiumType()) {
                    C3DVector delta = hero->GetPosition() - heroPos;
                    if (first || delta.Get2DLength() > 2.5f) {
                        first = false;
                        C3DVector away = creature->GetPosition() - hero->GetPosition();
                        C3DVector directed;
                        if (nav->GetNearbyDirectedPosition(hero->GetPosition(), 7.0f, away, 0.33333334f, &directed)) {
                            heroPos = hero->GetPosition();
                            if ((creature->GetPosition() - directed).GetLength() > 4.0f
                                && nav->GetNearbyWanderPosition(directed, (GFFloatRandom(0.7f, &seed) + 0.3f) * 3.0f, &dest, false))
                                moving = true;
                            else
                                moving = false;
                        }
                    }
                }
                if (moving) {
                    result = AIAction_MoveToPosProximity(dest);
                    if (result == AI_ACTION_DONE) {
                        moving = false;
                        continue;
                    }
                } else
                    result = AIAction_LookAtThing(hero);
                if (result != AI_ACTION_RUNNING && result != AI_ACTION_DONE)
                    break;
            }
        }
    }
    Terminate();
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CThing hero; hero.pos = C3DVector(0, 0, 0);
    CThing me; me.pos = C3DVector(20, 0, 0);
    CTCCreatureGroupBoastingCrowd crowd; crowd.hero = &hero; crowd.offPodium = false;
    CTCCreatureNavigation nav; nav.directedOk = true; nav.directedCalls = nav.wanderCalls = 0; nav.lastRadius = 0;
    CAIStateGroup_BoastingCrowdMember g; g.runnable = true; g.terminated = false; g.creature = &me; g.crowd = &crowd;
    g.nav = &nav; g.lastLook = 0; g.seed = 0;

    // Not runnable: terminate straight away.
    g.runnable = false; g_step = 0; g_steps = 0;
    g.Update();
    Check(g.terminated && g_step == 0, "not runnable terminates");
    g.runnable = true; g.terminated = false;

    // Far from the directed spot (7 from hero, 13 from me): move there (radius (0.35+0.3)*3), move done, then
    // hero has not moved so look, look running, then abort ends the loop.
    g_step = 0; g_steps = 3; g_script[0] = AI_ACTION_DONE; g_script[1] = AI_ACTION_RUNNING; g_script[2] = AI_ACTION_STOPPING;
    g.Update();
    Check(g_kind[0] == 'M' && g.lastDest.X == 7.0f, "moves to directed spot");
    Check(nav.directedCalls == 1 && nav.wanderCalls == 1 && fabs(nav.lastRadius - 1.95f) < 1e-4f, "wander radius");
    Check(nav.lastDir.X == 20.0f, "away vector from hero to creature");
    Check(g_kind[1] == 'L' && g_kind[2] == 'L' && g.lastLook == &hero, "then looks at hero");
    Check(g.terminated, "terminated after stop");
    g.terminated = false;

    // Already close to the directed spot: just look.
    me.pos = C3DVector(8, 0, 0); nav.directedCalls = nav.wanderCalls = 0;
    g_step = 0; g_steps = 1; g_script[0] = AI_ACTION_ABORTED;
    g.Update();
    Check(g_kind[0] == 'L' && nav.wanderCalls == 0, "close: look only");
    g.terminated = false;

    // Move still running keeps moving; hero then moves 3 away -> new directed search.
    me.pos = C3DVector(20, 0, 0); nav.directedCalls = 0;
    g_step = 0; g_steps = 2; g_script[0] = AI_ACTION_RUNNING; g_script[1] = AI_ACTION_ABORTED;
    g.Update();
    Check(g_kind[0] == 'M' && g_kind[1] == 'M' && nav.directedCalls == 1, "running move repeats");
    g.terminated = false;

    // Directed search fails: moving stays false, look.
    nav.directedOk = false; g_step = 0; g_steps = 1; g_script[0] = AI_ACTION_ABORTED;
    g.Update();
    Check(g_kind[0] == 'L', "no directed spot: look");
    nav.directedOk = true;
    g.terminated = false;

    // Dead hero: terminate without acting.
    hero.m_flags91 = 1; g_step = 0;
    g.Update();
    Check(g.terminated && g_step == 0, "dead hero terminates");

    if (failures == 0)
        std::printf("0095e900_TEST PASS\n");
    return failures;
}