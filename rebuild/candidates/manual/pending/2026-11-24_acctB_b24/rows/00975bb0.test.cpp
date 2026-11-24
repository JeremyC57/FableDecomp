// Behaviour test for 0x00975BB0 CAIStateGroup_JackOfBladesStage1::Update: a model of the row's decisions over a fake world.
// Checked: the entry exits (Terminate only when not runnable), the facing angle toward the target (atan2(dx, dy) in turns wrapped
// into [0, 1); no facing for a zero offset: IsNonZero against DELTA_NOTIONAL_ZERO squared), and the combat loop: the rush (distance
// above RushTriggerDistance: rush to me + normalised(line) * RushToDistance), the approach (above IdealShootDistance + 3), the melee /
// leap-back choice inside FrontMiddleLimit, the shot schedule (time since the last shot above TimeBetweenShots, line of sight, shot
// type = shot_counter % 3, near shot under ideal + 3, far shot under the rush trigger, counter and frame only on a finished shot),
// the taunt by SpeechCounter % 6 (cases 0..3 speak, 4 and 5 stay silent, the counter always advances), and the phase-change exit.
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>

struct State {
    bool Runnable, LineOfSight, KnockedDown, Talking, ShotFinishes;
    float MeX, MeY, TargetX, TargetY;
    float Ideal, RushTrigger, RushDistance, TimeBetween, FrontMiddle;
    long FPS, Frame, ReadyAfterLoops, Loops, SpeechCounter;
    float Facing; bool FacingSet;
    float RushX, RushY;
};
struct World : State {                                        // VC7.1 does not zero-fill `World()` for a non-POD struct
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
static void Face(World& w)
{
    float dx = w.TargetX - w.MeX, dy = w.TargetY - w.MeY;
    if (dx * dx + dy * dy > 1e-4f * 1e-4f) { w.Facing = Wrap((float)std::atan2(dx, dy) / 6.2831855f); w.FacingSet = true; }
}
static float Dist(const World& w) { float dx = w.TargetX - w.MeX, dy = w.TargetY - w.MeY; return std::sqrt(dx * dx + dy * dy); }

static void Update(World& w)
{
    if (!w.Runnable) { w.Trace += 'T'; return; }
    Face(w);
    long frame_of_last_shot = w.Frame, shot_counter = 0;
    while (w.Loops < w.ReadyAfterLoops) {
        ++w.Loops;
        float d = Dist(w);
        bool moved = false;
        if (d > w.RushTrigger) {
            float f = 1.0f / d;
            w.RushX = w.MeX + (w.TargetX - w.MeX) * f * w.RushDistance;
            w.RushY = w.MeY + (w.TargetY - w.MeY) * f * w.RushDistance;
            w.MeX = w.RushX; w.MeY = w.RushY; w.Trace += 'R';
        } else if (d > w.Ideal + 3.0f) {
            w.Trace += 'M'; moved = true;
        }
        w.Frame += w.FPS;                                   // one second per loop
        float since = (float)(w.Frame - frame_of_last_shot) / (float)w.FPS;
        d = Dist(w);
        if (d < w.FrontMiddle) {
            w.Trace += w.KnockedDown ? 'L' : 'C';
        } else if (since > w.TimeBetween) {
            if (w.LineOfSight) {
                w.Trace += (char)('0' + shot_counter % 3);
                if (d < w.Ideal + 3.0f) { w.Trace += 'n'; if (w.ShotFinishes) { frame_of_last_shot = w.Frame; ++shot_counter; } }
                else if (d < w.RushTrigger) { w.Trace += 'f'; if (w.ShotFinishes) { frame_of_last_shot = w.Frame; ++shot_counter; } }
            } else { w.Trace += 'm'; moved = true; }
        } else if (w.KnockedDown) {
            if (!w.Talking) {
                long r = w.SpeechCounter % 6;
                if (r <= 3) w.Trace += (char)('a' + r);
                ++w.SpeechCounter;
            }
            w.Trace += 'B';
        } else if (!moved) {
            w.Trace += 'K';
        }
    }
    w.Trace += 'P';
}

static World Base()
{
    World w;
    w.Runnable = true; w.LineOfSight = true; w.ShotFinishes = true;
    w.MeX = 0; w.MeY = 0; w.TargetX = 0; w.TargetY = 10;
    w.Ideal = 8; w.RushTrigger = 20; w.RushDistance = 5; w.TimeBetween = 1.5f; w.FrontMiddle = 2;
    w.FPS = 30; w.Frame = 100; w.ReadyAfterLoops = 6;
    return w;
}

int main()
{
    int bad = 0;
    World w = Base(); w.Runnable = false; Update(w);
    if (w.Trace != "T" || w.FacingSet) bad |= 1;

    w = Base(); Update(w);                                  // shots every second loop, types cycle 0, 1, 2
    if (w.Trace != "K0nK1nK2nP") bad |= 2;
    if (!w.FacingSet || std::fabs(w.Facing - 0.0f) > 1e-6f) bad |= 4;

    w = Base(); w.TargetX = -10; w.TargetY = 0; Face(w);    // west: atan2(-10, 0) = -pi/2 -> 0.75 turn
    if (std::fabs(w.Facing - 0.75f) > 1e-5f) bad |= 8;
    w = Base(); w.TargetY = 0; Face(w);
    if (w.FacingSet) bad |= 16;

    w = Base(); w.TargetY = 22; w.TimeBetween = 0.5f; w.ReadyAfterLoops = 1; Update(w);   // rush 5 units along +Y, then a far shot
    if (w.Trace != "R0fP" || std::fabs(w.RushY - 5.0f) > 1e-4f || std::fabs(w.RushX) > 1e-4f) bad |= 32;

    w = Base(); w.TargetY = 15; w.ReadyAfterLoops = 2; w.LineOfSight = false; Update(w);
    if (w.Trace != "MMmP") bad |= 64;

    w = Base(); w.TargetY = 1; w.ReadyAfterLoops = 2; Update(w);
    if (w.Trace != "CCP") bad |= 128;
    w.Trace = ""; w.Loops = 0; w.KnockedDown = true; Update(w);
    if (w.Trace != "LLP") bad |= 256;

    w = Base(); w.KnockedDown = true; w.ShotFinishes = false; w.TimeBetween = 100; w.ReadyAfterLoops = 7; Update(w);
    if (w.Trace != "aBbBcBdBBBaBP") bad |= 512;
    if (w.SpeechCounter != 7) bad |= 1024;

    if (bad) { std::printf("00975bb0_TEST FAIL %d\n", bad); return 1; }
    std::printf("00975bb0_TEST PASS\n");
    return 0;
}
