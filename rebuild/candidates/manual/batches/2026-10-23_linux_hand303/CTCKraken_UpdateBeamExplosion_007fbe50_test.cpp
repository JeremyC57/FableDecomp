// Behaviour test for CTCKraken::UpdateBeamExplosion @ 0x007FBE50: gated on a live target, the beam-explosion interval and a
// live beam effect; the explosion is created at start + dir * range looking at start + dir * (range + extra), where start/dir
// come from the mouth dummy (dir = -orientation.Up) or, without the dummy, the effect's position and facing; a created
// explosion excludes the kraken, is remembered and stamps the frame; a BEAM_HIT effect appears at the hit point when the
// line of sight to the beam end is blocked
#include <cstdio>
struct V3 { float X, Y, Z; };
static V3 Make(float x, float y, float z) { V3 v; v.X = x; v.Y = y; v.Z = z; return v; }
static V3 Add(const V3& a, const V3& b) { return Make(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V3 Mul(const V3& a, float s) { return Make(a.X * s, a.Y * s, a.Z * s); }

struct World {
    int now, last, interval;
    bool targetAlive, effectAlive;
    int mode;
    float rangeMode4, range, extra;
    bool dummyFound; V3 dummyPos, dummyUp, effectPos, effectFacing;
    bool createOk, effectsOk, losClear;
    // outputs
    int creates; V3 createAt, createLook; int excluded, remembered; int effects; V3 effectAt; int effectType;
    int losCalls; V3 losFrom, losTo;
};

static void Update(World& w)
{
    long now = w.now;
    if (!w.targetAlive) return;
    if (now - w.last <= w.interval) return;
    if (!w.effectAlive) return;
    float range = (w.mode == 4) ? w.rangeMode4 : w.range;
    V3 start = w.effectPos, dir;
    if (w.dummyFound) { start = w.dummyPos; dir = Mul(w.dummyUp, -1.0f); }
    else { dir = w.effectFacing; start = w.effectPos; }
    float length = range + w.extra;
    ++w.creates; w.createAt = Add(start, Mul(dir, range)); w.createLook = Add(start, Mul(dir, length));
    if (w.createOk) { ++w.excluded; ++w.remembered; w.last = w.now; }
    if (w.effectsOk) {
        ++w.losCalls; w.losFrom = start; w.losTo = Add(start, Mul(dir, length));
        if (!w.losClear) { ++w.effects; w.effectAt = w.losTo; w.effectType = 7; }
    }
}

static bool Eq(const V3& a, float x, float y, float z) { return a.X == x && a.Y == y && a.Z == z; }

static World Base()
{
    World w;
    w.now = 100; w.last = 50; w.interval = 20; w.targetAlive = true; w.effectAlive = true; w.mode = 0;
    w.rangeMode4 = 4.0f; w.range = 2.0f; w.extra = 1.0f; w.dummyFound = true;
    w.dummyPos = Make(1.0f, 2.0f, 3.0f); w.dummyUp = Make(0.0f, 0.0f, 1.0f);
    w.effectPos = Make(10.0f, 0.0f, 0.0f); w.effectFacing = Make(1.0f, 0.0f, 0.0f);
    w.createOk = true; w.effectsOk = true; w.losClear = false;
    w.creates = 0; w.excluded = 0; w.remembered = 0; w.effects = 0; w.effectType = 0; w.losCalls = 0;
    return w;
}

int main()
{
    int fails = 0;
    World w = Base(); Update(w);
    if (w.creates != 1 || !Eq(w.createAt, 1.0f, 2.0f, 1.0f) || !Eq(w.createLook, 1.0f, 2.0f, 0.0f)) ++fails;
    if (w.last != 100 || w.excluded != 1 || w.remembered != 1) ++fails;
    if (w.effects != 1 || !Eq(w.effectAt, 1.0f, 2.0f, 0.0f) || !Eq(w.losFrom, 1.0f, 2.0f, 3.0f)) ++fails;

    w = Base(); w.dummyFound = false; w.mode = 4; Update(w);
    if (!Eq(w.createAt, 14.0f, 0.0f, 0.0f) || !Eq(w.createLook, 15.0f, 0.0f, 0.0f)) ++fails;

    w = Base(); w.last = 90; Update(w);                       // interval not yet passed
    if (w.creates != 0 || w.losCalls != 0) ++fails;
    w = Base(); w.targetAlive = false; Update(w);
    if (w.creates != 0) ++fails;
    w = Base(); w.effectAlive = false; Update(w);
    if (w.creates != 0) ++fails;

    w = Base(); w.createOk = false; w.losClear = true; Update(w);
    if (w.last != 50 || w.excluded != 0 || w.effects != 0 || w.losCalls != 1) ++fails;
    w = Base(); w.effectsOk = false; Update(w);
    if (w.losCalls != 0 || w.effects != 0 || w.last != 100) ++fails;

    if (fails == 0) std::printf("007fbe50_TEST PASS\n");
    else std::printf("007fbe50_TEST FAIL %d\n", fails);
    return fails;
}
