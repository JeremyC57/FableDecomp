// Behaviour test for CModeScorpionKingTailPlunge::OnActivate (0x008D9E30): countdowns from the battle def, tail position at
// ground height, idle effect -> tail emitter, travel effect -> emitter attached to itself and stored as the dust trail.
#include <cstdio>

struct V { float X, Y, Z; };
struct Thing { bool dead; bool hasEmitter; Thing* attachedTo; long bone; };
struct Def { float Recovery, Spike, Decision; };

static Thing g_things[4];
static int g_next = 0;
static Thing* Create(long effect) { (void)effect; Thing* t = &g_things[g_next++]; return t; }

struct Mode {
    long RecoveryCount, SpikeTime, SpikeDecisionTime;
    V TailPos; Thing* PTailEmitter; Thing* PDustTrail;
    void OnActivate(long fps, const Def& def, const V& thingPos, const V& dummyPos, bool haveIdle, bool haveTravel)
    {
        RecoveryCount = (long)(fps * def.Recovery);
        SpikeTime = (long)(RecoveryCount - fps * def.Spike);
        SpikeDecisionTime = (long)(RecoveryCount - fps * def.Decision);
        V pos = thingPos;
        pos = dummyPos;                       // GetDummyObject fills it
        pos.Z = thingPos.Z;
        TailPos = pos;
        if (haveIdle) {
            Thing* fx = Create(1);
            if (fx && !fx->dead) PTailEmitter = fx;
        }
        if (haveTravel) {
            Thing* fx = Create(2);
            if (fx && !fx->dead) {
                if (fx->hasEmitter) { fx->attachedTo = fx; fx->bone = 10; }
                PDustTrail = fx;
            }
        }
    }
};

int main()
{
    for (int i = 0; i < 4; ++i) { g_things[i].dead = false; g_things[i].hasEmitter = true; g_things[i].attachedTo = 0; g_things[i].bone = 0; }
    Mode m; m.PTailEmitter = 0; m.PDustTrail = 0;
    Def def; def.Recovery = 2.0f; def.Spike = 0.5f; def.Decision = 1.0f;
    V tp = { 1, 2, 3 }; V dp = { 7, 8, 9 };
    m.OnActivate(30, def, tp, dp, true, true);
    if (m.RecoveryCount != 60 || m.SpikeTime != 45 || m.SpikeDecisionTime != 30 || m.TailPos.X != 7 || m.TailPos.Y != 8
        || m.TailPos.Z != 3 || m.PTailEmitter != &g_things[0] || m.PDustTrail != &g_things[1]
        || g_things[1].attachedTo != &g_things[1] || g_things[1].bone != 10 || g_things[0].attachedTo != 0) {
        std::printf("008d9e30_TEST FAIL\n"); return 1;
    }
    std::printf("008d9e30_TEST PASS\n");
    return 0;
}
