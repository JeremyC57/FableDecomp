// Behaviour test for 0x006823D0 CTCCreatureOpinionOfHero::FrameUpdate (a model of the row's logic):
// 1. a live unknown-threat source is killed when it is older than 30 seconds of frames, or when the creature perceives the hero
//    (perceived thing == hero, aware, ignoring FOV / line of sight);
// 2. each transient opinion offset runs in (capping at +-2 and converting the overshoot into capped-peak frames taken from the
//    run-out), holds its capped peak, runs out (clamped to +-2), and is erased when spent;
// 3. every 20 seconds (frame + unique id) the known deeds the hero's deed log no longer holds are dropped;
// 4. the times-hit count decays by frames / fps / 60 once more than 30 frames passed, never below zero.
#include <cstdio>
#include <cmath>
#include <list>
#include <set>
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
static float GFFloatAbs(float f) { return f < 0 ? -f : f; }
static void GFLimit(float& v, float lo, float hi) { if (v < lo) v = lo; else if (v > hi) v = hi; }
struct Offset { int Opinion; float RunIn; long FramesToRunIn; long FramesOfCappedPeak; float RunOut; long FramesToRunOut; };
struct Source { bool present, dead, killed; };
struct Perceive { bool has; const void* perceived; bool aware, ignoreFov; };
struct Model {
    long frame, fps; unsigned long uniqueLo;
    Source source; long frameLastUpdatedUnknownThreat; Perceive perceive; const void* hero;
    float opinion[4]; std::list<Offset> offsets;
    std::list<long> knownDeeds[92]; std::set<long> deedLog; int deedLookups;
    float timesHit; long frameToDecay;
    void FrameUpdate() {
        if (source.present && !source.dead) {
            if ((float)(frame - frameLastUpdatedUnknownThreat) > (float)fps * 30.0f)
                source.killed = true;
            else if (perceive.has && hero != 0 && perceive.perceived == hero && perceive.aware && perceive.ignoreFov)
                source.killed = true;
        }
        std::list<Offset>::iterator end = offsets.end();
        for (std::list<Offset>::iterator it = offsets.begin(), next = it; it != end; it = next) {
            ++next;
            Offset& o = *it;
            if (o.FramesToRunIn > 0) {
                opinion[o.Opinion] += o.RunIn;
                if (GFFloatAbs(opinion[o.Opinion]) - 2.0f > DELTA_NOTIONAL_ZERO) {
                    float over = GFFloatAbs(opinion[o.Opinion]) - 2.0f;
                    if (GFFloatAbs(o.RunOut) > DELTA_NOTIONAL_ZERO) {
                        long frames = (long)GFFloatAbs(over / o.RunOut);
                        if (frames > o.FramesToRunOut) frames = o.FramesToRunOut;
                        o.FramesToRunOut -= frames;
                        o.FramesOfCappedPeak += frames;
                    }
                    GFLimit(opinion[o.Opinion], -2.0f, 2.0f);
                }
                --o.FramesToRunIn;
            } else if (o.FramesOfCappedPeak > 0) {
                --o.FramesOfCappedPeak;
            } else if (o.FramesToRunOut > 0) {
                opinion[o.Opinion] += o.RunOut;
                GFLimit(opinion[o.Opinion], -2.0f, 2.0f);
                --o.FramesToRunOut;
            } else {
                next = offsets.erase(it);
            }
        }
        if (((unsigned long)frame + uniqueLo) % (unsigned long)(fps * 20) == 0) {
            if (hero) {
                for (long i = 0; i < 92; ++i) {
                    std::list<long>& deeds = knownDeeds[i];
                    std::list<long>::iterator deedsEnd = deeds.end();
                    for (std::list<long>::iterator deed = deeds.begin(), nextDeed = deed; deed != deedsEnd; deed = nextDeed) {
                        ++nextDeed; ++deedLookups;
                        if (deedLog.find(*deed) == deedLog.end()) nextDeed = deeds.erase(deed);
                    }
                }
            }
        }
        if (timesHit != 0.0f) {
            long frames = frame - frameToDecay;
            if (frames > 30) {
                timesHit -= (float)frames / fps * (1.0f / 60.0f);
                if (timesHit < 0.0f) timesHit = 0.0f;
                frameToDecay = frame + 1;
            }
        }
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static Model Base() {
    Model m; m.frame = 1001; m.fps = 30; m.uniqueLo = 0; m.source.present = false; m.source.dead = false; m.source.killed = false;
    m.frameLastUpdatedUnknownThreat = 1000; m.perceive.has = false; m.perceive.perceived = 0; m.perceive.aware = false; m.perceive.ignoreFov = false;
    static int heroObject; m.hero = &heroObject; for (int i = 0; i < 4; ++i) m.opinion[i] = 0; m.deedLookups = 0; m.timesHit = 0; m.frameToDecay = 0;
    return m;
}
static Offset O(int op, float in, long fin, long peak, float out, long fout) { Offset o; o.Opinion = op; o.RunIn = in; o.FramesToRunIn = fin; o.FramesOfCappedPeak = peak; o.RunOut = out; o.FramesToRunOut = fout; return o; }
int main() {
    // 1. unknown-threat source
    { Model m = Base(); m.source.present = true; m.frameLastUpdatedUnknownThreat = m.frame - 900; m.FrameUpdate(); check(!m.source.killed, "exactly 30 s old is kept");
      m.frameLastUpdatedUnknownThreat = m.frame - 901; m.FrameUpdate(); check(m.source.killed, "older than 30 s is killed"); }
    { Model m = Base(); m.source.present = true; m.perceive.has = true; m.perceive.perceived = m.hero; m.perceive.aware = true; m.perceive.ignoreFov = true;
      m.FrameUpdate(); check(m.source.killed, "hero perceived -> killed");
      m.source.killed = false; m.perceive.ignoreFov = false; m.FrameUpdate(); check(!m.source.killed, "needs ignore-FOV perception");
      m.perceive.ignoreFov = true; m.perceive.aware = false; m.FrameUpdate(); check(!m.source.killed, "needs awareness");
      m.perceive.aware = true; static int other; m.perceive.perceived = &other; m.FrameUpdate(); check(!m.source.killed, "must perceive the hero");
      m.perceive.perceived = m.hero; m.source.dead = true; m.FrameUpdate(); check(!m.source.killed, "a dead source is left alone");
      m.source.dead = false; m.perceive.has = false; m.FrameUpdate(); check(!m.source.killed, "no perceive component"); }
    // 2. transient offsets
    { Model m = Base(); m.opinion[1] = 1.5f; m.offsets.push_back(O(1, 1.0f, 2, 0, -0.25f, 10));
      m.FrameUpdate();                                         // 2.5 -> over 0.5 -> 2 frames moved from run-out to capped peak, clamp to 2
      Offset& o = m.offsets.front();
      check(near(m.opinion[1], 2.0f) && o.FramesToRunIn == 1 && o.FramesOfCappedPeak == 2 && o.FramesToRunOut == 8, "run-in caps the peak");
      m.FrameUpdate();                                         // 3.0 -> over 1.0 -> 4 more frames
      check(near(m.opinion[1], 2.0f) && o.FramesToRunIn == 0 && o.FramesOfCappedPeak == 6 && o.FramesToRunOut == 4, "second run-in frame");
      for (int i = 0; i < 6; ++i) m.FrameUpdate();
      check(near(m.opinion[1], 2.0f) && o.FramesOfCappedPeak == 0 && o.FramesToRunOut == 4, "capped peak held");
      m.FrameUpdate(); check(near(m.opinion[1], 1.75f) && o.FramesToRunOut == 3, "run-out");
      for (int i = 0; i < 3; ++i) m.FrameUpdate();
      check(near(m.opinion[1], 1.0f) && m.offsets.size() == 1, "run-out finished, still listed");
      m.FrameUpdate(); check(m.offsets.empty() && near(m.opinion[1], 1.0f), "spent offset erased"); }
    { Model m = Base(); m.opinion[2] = -1.9f; m.offsets.push_back(O(2, -0.5f, 1, 0, 0.00001f, 5)); m.offsets.push_back(O(0, 0.0f, 0, 0, -3.0f, 1));
      m.FrameUpdate();
      check(near(m.opinion[2], -2.0f) && m.offsets.front().FramesToRunOut == 5 && m.offsets.front().FramesOfCappedPeak == 0, "tiny run-out rate: clamp only");
      check(near(m.opinion[0], -2.0f) && m.offsets.back().FramesToRunOut == 0, "run-out clamps at -2"); }
    { Model m = Base(); m.opinion[3] = 1.0f; m.offsets.push_back(O(3, 3.0f, 1, 0, -0.1f, 4)); m.FrameUpdate();   // over 2.0 -> 20 frames wanted, only 4 there
      check(m.offsets.front().FramesToRunOut == 0 && m.offsets.front().FramesOfCappedPeak == 4 && near(m.opinion[3], 2.0f), "overshoot limited to the run-out frames"); }
    // 3. known deeds, every fps * 20 frames of (frame + unique id)
    { Model m = Base(); m.frame = 590; m.uniqueLo = 10; m.knownDeeds[0].push_back(1); m.knownDeeds[0].push_back(2); m.knownDeeds[91].push_back(3); m.deedLog.insert(2);
      m.FrameUpdate();
      check(m.knownDeeds[0].size() == 1 && m.knownDeeds[0].front() == 2 && m.knownDeeds[91].empty() && m.deedLookups == 3, "forgotten deeds dropped");
      m.knownDeeds[5].push_back(9); m.frame = 591; m.FrameUpdate(); check(m.knownDeeds[5].size() == 1 && m.deedLookups == 3, "off-period frame: no sweep");
      m.frame = 1190; m.hero = 0; m.FrameUpdate(); check(m.knownDeeds[5].size() == 1, "no hero: no sweep"); }
    // 4. times-hit decay
    { Model m = Base(); m.timesHit = 1.0f; m.frameToDecay = m.frame - 30; m.FrameUpdate(); check(m.timesHit == 1.0f && m.frameToDecay == m.frame - 30, "30 frames: no decay yet");
      m.frameToDecay = m.frame - 60; m.FrameUpdate(); check(near(m.timesHit, 1.0f - 60.0f / 30.0f / 60.0f) && m.frameToDecay == m.frame + 1, "decay by frames / fps / 60");
      m.timesHit = 0.01f; m.frameToDecay = m.frame - 600; m.FrameUpdate(); check(m.timesHit == 0.0f, "never below zero");
      m.frameToDecay = 0; m.FrameUpdate(); check(m.frameToDecay == 0, "zero count: timer untouched"); }
    if (fails == 0) std::printf("006823d0_TEST PASS\n");
    return fails;
}
