// Behaviour test for 0x00952890 CAIStateGroup_JackDragon::UpdateGround: a self-contained model of the row's decision tree (same tests,
// same order of AI calls), driven through each branch and checked against the expected call trace. Plain C++98; no row object linked.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Def { float MinTimeBetweenSlams; long MaxHitsBeforeFlyAway; float MinSnapDistance, MaxSnapDistance, MaxTimeAtLongRange,
             MaxTimeOnGround, MaxTimeBeforeSnap, MinRoarDistance; };

struct Dragon {
    // inputs
    long frame, fps, boss_hits; float ex, ey, ez, fx, fy, fz; bool enemy_alive, stopping, hit_event, still_runnable, has_boss;
    // state (members)
    long FrameOfLastSlam, FrameWhenLastInRange, FrameWhenLanded, FramesInSnapRange; bool BattleTriggered, IsRangeAttack;
    Def def;
    char trace[512];
    void Log(const char* s) { std::strcat(trace, s); std::strcat(trace, ","); }
    void SetHeadTurning(bool on) { Log(on ? "head1" : "head0"); }
    void WaitForAnimLoop(float) { Log("wait"); }
    void FlyOutOfGround() { Log("flyout"); }
    void StayIdle(float t) { Log(t == 2.0f ? "idle2" : (t == 1.0f ? "idle1" : "idle05")); }
    void Act(const char* a) { Log(a); }
    void DoNothing() { Log("nothing"); }
    void Terminate() { Log("terminate"); }
    bool IsStillRunnable() { return still_runnable && enemy_alive && !stopping; }

    void UpdateGround()
    {
        float dx = ex, dy = ey, dz = ez;                          // dragon at the origin
        float distance = (float)std::sqrt(dx * dx + dy * dy);
        float len = (float)std::sqrt(dx * dx + dy * dy + dz * dz);
        if (len > 0) { dx /= len; dy /= len; dz /= len; }
        long frames_on_ground = frame - FrameWhenLanded;
        SetHeadTurning(true);
        if (frames_on_ground > fps * def.MaxTimeOnGround) {
            SetHeadTurning(false); WaitForAnimLoop(0.5f); FlyOutOfGround();
        } else if (distance < def.MaxSnapDistance) {
            BattleTriggered = true;
            long frames_since_slam = frame - FrameOfLastSlam;
            FrameWhenLastInRange = frame;
            if (hit_event && !IsRangeAttack) {
                Act("snap");
                if (!IsStillRunnable()) { Terminate(); return; }
                StayIdle(1.0f);
            } else if (distance > def.MinSnapDistance && dx * fx + dy * fy + dz * fz > 0.9659259915351868f) {
                if (++FramesInSnapRange > fps * def.MaxTimeBeforeSnap) {
                    Act("snapfast");
                    if (!IsStillRunnable()) { Terminate(); return; }
                    StayIdle(1.0f); FramesInSnapRange = 0;
                } else DoNothing();
            } else if (frames_since_slam > fps * def.MinTimeBetweenSlams) {
                SetHeadTurning(false); WaitForAnimLoop(0.5f);
                if (!IsStillRunnable()) { Terminate(); return; }
                FramesInSnapRange = 0;
                Act("rearup");
                if (!IsStillRunnable()) { Terminate(); return; }
                StayIdle(0.5f);
                if (!IsStillRunnable()) { Terminate(); return; }
                Act("roar");
                if (!IsStillRunnable()) { Terminate(); return; }
                SetHeadTurning(true); StayIdle(2.0f); FrameOfLastSlam = frame;
            } else { FramesInSnapRange = 0; DoNothing(); }
        } else {
            long frames_since_slam = frame - FrameOfLastSlam;
            long frames_since_in_range = frame - FrameWhenLastInRange;
            float f = (float)fps;
            if ((frames_since_in_range > f * def.MaxTimeAtLongRange || (has_boss && boss_hits > def.MaxHitsBeforeFlyAway)) && BattleTriggered) {
                FrameOfLastSlam = frame; SetHeadTurning(false); WaitForAnimLoop(0.5f); FlyOutOfGround();
            } else if (distance < 25.0f && frames_since_slam > f * def.MinTimeBetweenSlams) {
                SetHeadTurning(false); WaitForAnimLoop(0.5f);
                if (!IsStillRunnable()) { Terminate(); return; }
                BattleTriggered = true; FrameWhenLastInRange = frame;
                Act("rearup");
                if (!IsStillRunnable()) { Terminate(); return; }
                SetHeadTurning(true); StayIdle(0.5f); FrameOfLastSlam = frame;
            } else if (frames_on_ground <= 2 && distance > def.MinRoarDistance) {
                SetHeadTurning(false); WaitForAnimLoop(0.5f); Act("roar"); SetHeadTurning(true); StayIdle(1.0f);
            } else DoNothing();
        }
        if (!IsStillRunnable()) { Terminate(); return; }
    }
};

static Dragon Make()
{
    Dragon d;
    std::memset(&d, 0, sizeof(d));
    d.frame = 1000; d.fps = 30; d.enemy_alive = true; d.still_runnable = true; d.has_boss = true;
    d.FrameWhenLanded = 990; d.FrameOfLastSlam = 0; d.FrameWhenLastInRange = 995;
    d.def.MinTimeBetweenSlams = 5.0f; d.def.MaxHitsBeforeFlyAway = 3; d.def.MinSnapDistance = 2.0f; d.def.MaxSnapDistance = 10.0f;
    d.def.MaxTimeAtLongRange = 4.0f; d.def.MaxTimeOnGround = 20.0f; d.def.MaxTimeBeforeSnap = 1.0f; d.def.MinRoarDistance = 30.0f;
    d.ex = 5.0f; d.fx = 1.0f;
    return d;
}

static int fails = 0;
static void Expect(const Dragon& d, const char* want, const char* what)
{
    if (std::strcmp(d.trace, want) != 0) { std::printf("fail %s: got %s want %s\n", what, d.trace, want); ++fails; }
}

int main()
{
    { Dragon d = Make(); d.FrameWhenLanded = 0; d.UpdateGround(); Expect(d, "head1,head0,wait,flyout,", "time on ground"); }
    { Dragon d = Make(); d.hit_event = true; d.UpdateGround(); Expect(d, "head1,snap,idle1,", "hit snap");
      if (!d.BattleTriggered || d.FrameWhenLastInRange != 1000) { std::printf("fail snap state\n"); ++fails; } }
    { Dragon d = Make(); d.hit_event = true; d.IsRangeAttack = true; d.FrameOfLastSlam = 999; d.FramesInSnapRange = 30; d.UpdateGround();
      Expect(d, "head1,snapfast,idle1,", "snap fast"); if (d.FramesInSnapRange != 0) { std::printf("fail counter\n"); ++fails; } }
    { Dragon d = Make(); d.FrameOfLastSlam = 999; d.FramesInSnapRange = 3; d.UpdateGround(); Expect(d, "head1,nothing,", "facing wait");
      if (d.FramesInSnapRange != 4) { std::printf("fail counter2\n"); ++fails; } }
    { Dragon d = Make(); d.fx = -1.0f; d.UpdateGround(); Expect(d, "head1,head0,wait,rearup,idle05,roar,head1,idle2,", "slam");
      if (d.FrameOfLastSlam != 1000) { std::printf("fail slam frame\n"); ++fails; } }
    { Dragon d = Make(); d.fx = -1.0f; d.FrameOfLastSlam = 999; d.FramesInSnapRange = 7; d.UpdateGround(); Expect(d, "head1,nothing,", "no slam");
      if (d.FramesInSnapRange != 0) { std::printf("fail reset\n"); ++fails; } }
    { Dragon d = Make(); d.ex = 20.0f; d.BattleTriggered = true; d.FrameWhenLastInRange = 0; d.UpdateGround(); Expect(d, "head1,head0,wait,flyout,", "long range");
      if (d.FrameOfLastSlam != 1000) { std::printf("fail fly frame\n"); ++fails; } }
    { Dragon d = Make(); d.ex = 20.0f; d.BattleTriggered = true; d.boss_hits = 5; d.UpdateGround(); Expect(d, "head1,head0,wait,flyout,", "boss hits"); }
    { Dragon d = Make(); d.ex = 20.0f; d.UpdateGround(); Expect(d, "head1,head0,wait,rearup,head1,idle05,", "rear up near"); }
    { Dragon d = Make(); d.ex = 40.0f; d.FrameWhenLanded = 999; d.UpdateGround(); Expect(d, "head1,head0,wait,roar,head1,idle1,", "landing roar"); }
    { Dragon d = Make(); d.ex = 40.0f; d.UpdateGround(); Expect(d, "head1,nothing,", "idle far"); }
    { Dragon d = Make(); d.hit_event = true; d.still_runnable = false; d.UpdateGround(); Expect(d, "head1,snap,terminate,", "terminate"); }
    { Dragon d = Make(); d.ex = 40.0f; d.stopping = true; d.UpdateGround(); Expect(d, "head1,nothing,terminate,", "final check"); }
    if (fails) { std::printf("00952890_TEST FAIL\n"); return 1; }
    std::printf("00952890_TEST PASS\n");
    return 0;
}