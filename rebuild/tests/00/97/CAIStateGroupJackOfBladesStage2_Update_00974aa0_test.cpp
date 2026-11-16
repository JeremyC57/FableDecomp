// Behaviour test for 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update: a model of the row's decisions over a fake world
// (runnable / alive flags, positions, scale, frame counter, boss phase readiness, knocked-down hero, talk component). Checked:
// the entry exit (Terminate only), the arena reset and the facing angle (cutscene marker first, else the target; atan2(dx, dy)
// in turns wrapped into [0, 1); no facing for a zero offset), the float-up failure exit, the scale-up loop (step = (battle -
// scale) / (fps * time), per-frame scale + step, early stop when the target dies), the one-second wait (fps DoNothing frames, a
// dead target terminates), the pillar summon choice, the attack schedule (counter from 6, counter % 7: 0 counts a fireball round
// and falls into 1-2 left fireball, 3-4 combo fireball, 5 flash then minions / idle after more than one round, 6 idle), the idle
// after each attack while the boss is not ready, the taunt by SpeechCounter % 7 (cases 0..4 only) with the boast when the hero is
// knocked down, and the phase-change end (kill helpers, phase 4, not damageable / not hit-responsive, Terminate).
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

struct State {
    bool Runnable, RunnableAfterLookAt, MarkerAlive, TargetAlive, ArenaAlive, Stopping, FloatUpOk, KnockedDown, HasTalk, Talking;
    float MarkerX, MarkerY, TargetX, TargetY, MeX, MeY;
    float Scale, BattleScale, ScaleUpTime; long FPS, Frame;
    long Pillars, Helpers, Actions, ReadyAfter, DieAtScaleFrame, SpeechCounter, ShotType;
    bool Damageable, RespondHits;
    float Facing; bool FacingSet;
};
struct World : State {                                        // VC7.1 does not zero-fill `World()` for a non-POD struct
    std::vector<float> Scales;
    std::string Trace;
    World() { std::memset(static_cast<State*>(this), 0, sizeof(State)); }
};

static float Wrap(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
static bool Ready(const World& w) { return w.Actions >= w.ReadyAfter; }
static void Act(World& w, char c) { w.Trace += c; ++w.Actions; }
static void Face(World& w, float dx, float dy)
{
    if (dx * dx + dy * dy > 1e-4f * 1e-4f) { w.Facing = Wrap((float)std::atan2(dx, dy) * 0.15915494f); w.FacingSet = true; }
}
static bool Valid(World& w, bool withStopping) { return w.TargetAlive && !(withStopping && w.Stopping) && w.ArenaAlive; }

static void Update(World& w)
{
    if (!w.Runnable) { w.Trace += 'T'; return; }
    w.Trace += 'R';
    if (w.MarkerAlive) Face(w, w.MarkerX - w.MeX, w.MarkerY - w.MeY);
    else Face(w, w.TargetX - w.MeX, w.TargetY - w.MeY);
    w.Trace += 'G';
    w.Trace += 'U';
    if (!w.FloatUpOk) { w.Trace += 'T'; return; }
    float scale = w.Scale;
    float step = (w.BattleScale - scale) / ((float)w.FPS * w.ScaleUpTime);
    while (scale < w.BattleScale) {
        if (!Valid(w, true)) break;
        scale = w.Scale; scale += step; w.Scale = scale; w.Scales.push_back(scale);
        w.Trace += 'D'; ++w.Frame;
        if (w.Frame == w.DieAtScaleFrame) w.TargetAlive = false;
    }
    w.Scale = w.BattleScale; w.Scales.push_back(w.BattleScale);
    w.Trace += "p2";
    long start = w.Frame, elapsed;
    do {
        if (!Valid(w, true)) { w.Trace += 'T'; return; }
        w.Trace += 'D'; ++w.Frame;
        elapsed = w.Frame - start;
    } while ((float)elapsed < (float)w.FPS);
    w.Trace += w.Pillars > 0 ? 'M' : 'O';
    w.Trace += "p3W";
    long counter = 6, fireballs = 0;
    do {
        if (!Valid(w, false)) { w.Trace += 'T'; continue; }
        w.Trace += 'X';
        if ((w.TargetX - w.MeX) * (w.TargetX - w.MeX) + (w.TargetY - w.MeY) * (w.TargetY - w.MeY) > 1.0f) {
            w.Trace += 'L';
            if (!w.RunnableAfterLookAt) { w.Trace += 'T'; return; }
        }
        switch (counter % 7) {
        case 0: ++fireballs;
        case 1: case 2: w.ShotType = 1; Act(w, 'f'); break;
        case 3: case 4: w.ShotType = 2; Act(w, 'c'); break;
        case 5:
            Act(w, 'F');
            if (!Ready(w) && fireballs > 1) {
                if (w.Helpers == 0) w.Trace += 'm';
                Act(w, 'I');
            }
            break;
        case 6: Act(w, 'I'); break;
        }
        if (!Ready(w)) {
            Act(w, 'I');
            if (w.KnockedDown) {
                if (w.HasTalk && !w.Talking) {
                    long c = w.SpeechCounter % 7;
                    if (c <= 4) { w.Trace += 't'; w.Trace += (char)('0' + c); }
                    ++w.SpeechCounter;
                }
                Act(w, 'B');
            }
        }
        ++counter;
    } while (!Ready(w) && w.Runnable);
    if (Ready(w)) { w.Trace += "Kp4"; w.Damageable = false; w.RespondHits = false; w.Trace += 'T'; }
}

static World Base()
{
    World w;
    w.Runnable = w.RunnableAfterLookAt = w.TargetAlive = w.ArenaAlive = w.FloatUpOk = w.HasTalk = true;
    w.TargetX = 0.5f; w.TargetY = 0.5f; w.Scale = 1.0f; w.BattleScale = 2.0f; w.ScaleUpTime = 1.0f; w.FPS = 4;
    w.ReadyAfter = 1000; w.DieAtScaleFrame = -1; w.Pillars = 1; w.Helpers = 1; w.Damageable = w.RespondHits = true;
    return w;
}

int main()
{
    bool ok = true;
    World a = Base(); a.Runnable = false; Update(a);
    ok = ok && a.Trace == "T" && !a.FacingSet;
    World b = Base(); b.FloatUpOk = false; b.MarkerAlive = true; b.MarkerX = 1.0f; b.MarkerY = 0.0f; Update(b);
    ok = ok && b.Trace == "RGUT" && b.FacingSet && std::fabs(b.Facing - 0.25f) < 1e-5f;
    World c = Base(); c.FloatUpOk = false; c.TargetX = -1.0f; c.TargetY = 0.0f; Update(c);
    ok = ok && std::fabs(c.Facing - 0.75f) < 1e-5f;
    World d = Base(); d.FloatUpOk = false; d.TargetX = 0.0f; d.TargetY = 0.0f; Update(d);
    ok = ok && !d.FacingSet;
    // full run: 4 scale frames (1.25 .. 2.0), 4 wait frames, idle (case 6) + idle, left fireball (case 0) ends it (ready after 3)
    World e = Base(); e.ReadyAfter = 3; Update(e);
    ok = ok && e.Trace == "RGUDDDDp2DDDDMp3WXIIXfKp4T" && e.Scales.size() == 5 && e.Scales[0] == 1.25f && e.Scales[3] == 2.0f
            && e.ShotType == 1 && !e.Damageable && !e.RespondHits;
    // the target dies during the scale-up: the loop stops, the wait terminates at once
    World f = Base(); f.DieAtScaleFrame = 2; Update(f);
    ok = ok && f.Trace == "RGUDDp2T" && f.Scale == 2.0f;
    // no pillars, far target, knocked down hero: schedule 6,0,1,2,3,4,5 with taunts 0..4 then none for 5, 6; one round: no summon
    World g = Base(); g.Pillars = 0; g.Helpers = 0; g.TargetX = 3.0f; g.KnockedDown = true; g.ReadyAfter = 20; Update(g);
    ok = ok && g.Trace == "RGUDDDDp2DDDDOp3WXLIIt0BXLfIt1BXLfIt2BXLfIt3BXLcIt4BXLcIBXLFIBKp4T" && g.SpeechCounter == 7;
    // second fireball round: the flash at counter 19 summons (no living helpers) and idles before the post-attack idle
    World g2 = Base(); g2.Helpers = 0; g2.ReadyAfter = 29; Update(g2);
    ok = ok && g2.Trace == "RGUDDDDp2DDDDMp3WXIIXfIXfIXfIXcIXcIXFIXIIXfIXfIXfIXcIXcIXFmIIKp4T";
    // LookAt failure terminates
    World h = Base(); h.TargetX = 3.0f; h.RunnableAfterLookAt = false; Update(h);
    ok = ok && h.Trace == "RGUDDDDp2DDDDMp3WXLT";
    printf(ok ? "00974aa0_TEST PASS\n" : "00974aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}