#include <cstdio>
#include <cmath>
// Model of CTCCreatureFlyInCircles::QueueSwoop(target, start, middle, end): remember the swoop target, switch to the swoop mode
// (2) with no transition, keep `middle` as the target offset, restart the swoop progress, set the per-tick delta to
// 2 / (|end - middle| + |middle - start|), rebuild the swoop spline as the three points offset by the target's position, take the
// creature out of the map-who (when it has that component) and reset its physics position to `start`.
struct V3 { float X, Y, Z; };
static V3 Add(const V3& a, const V3& b) { V3 r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
static V3 Sub(const V3& a, const V3& b) { V3 r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static float Mag(const V3& a) { return (float)std::sqrt(a.X * a.X + a.Z * a.Z + a.Y * a.Y); }
struct Target { V3 pos; };
struct Creature { bool hasMapwho; bool inMap; V3 physicsPos; int resets; };
struct Fly {
    Creature* creature; long Mode; bool Transitioning; const Target* PSwoopTarget;
    V3 spline[8]; int points; V3 SwoopTargOffset; float SwoopProgress, SwoopDelta;
    void QueueSwoop(const Target& target, const V3& start, const V3& middle, const V3& end)
    {
        PSwoopTarget = &target;
        Mode = 2;
        Transitioning = false;
        SwoopTargOffset = middle;
        V3 targetPos = PSwoopTarget->pos;
        SwoopProgress = 0.0f;
        SwoopDelta = 2.0f / (Mag(Sub(end, middle)) + Mag(Sub(middle, start)));
        points = 0;
        spline[points++] = Add(targetPos, start);
        spline[points++] = Add(targetPos, middle);
        spline[points++] = Add(targetPos, end);
        if (creature->hasMapwho)
            creature->inMap = false;
        creature->physicsPos = start; ++creature->resets;
    }
};
static bool Eq(const V3& a, float x, float y, float z) { return a.X == x && a.Y == y && a.Z == z; }
int main()
{
    int bad = 0;
    Creature c = { true, true, { 0, 0, 0 }, 0 };
    Fly f; f.creature = &c; f.Mode = 1; f.Transitioning = true; f.PSwoopTarget = 0; f.points = 5; f.SwoopProgress = 0.7f; f.SwoopDelta = 9.0f;
    Target t = { { 10.0f, 20.0f, 30.0f } };
    V3 start = { 0.0f, 0.0f, 0.0f }, middle = { 3.0f, 4.0f, 0.0f }, end = { 3.0f, 4.0f, 12.0f };
    f.QueueSwoop(t, start, middle, end);
    if (f.PSwoopTarget != &t || f.Mode != 2 || f.Transitioning || f.SwoopProgress != 0.0f) bad = 1;
    if (!Eq(f.SwoopTargOffset, 3.0f, 4.0f, 0.0f)) bad = 1;
    if (f.SwoopDelta != 2.0f / (12.0f + 5.0f)) bad = 1;
    if (f.points != 3 || !Eq(f.spline[0], 10.0f, 20.0f, 30.0f) || !Eq(f.spline[1], 13.0f, 24.0f, 30.0f) || !Eq(f.spline[2], 13.0f, 24.0f, 42.0f)) bad = 1;
    if (c.inMap || c.resets != 1 || !Eq(c.physicsPos, 0.0f, 0.0f, 0.0f)) bad = 1;
    Creature d = { false, true, { 1, 1, 1 }, 0 };
    f.creature = &d;
    V3 s2 = { 1.0f, 2.0f, 3.0f }, m2 = { 1.0f, 2.0f, 5.0f }, e2 = { 1.0f, 8.0f, 5.0f };
    f.QueueSwoop(t, s2, m2, e2);
    if (!d.inMap || d.resets != 1 || !Eq(d.physicsPos, 1.0f, 2.0f, 3.0f)) bad = 1;
    if (f.points != 3 || !Eq(f.spline[0], 11.0f, 22.0f, 33.0f) || !Eq(f.spline[2], 11.0f, 28.0f, 35.0f)) bad = 1;
    if (f.SwoopDelta != 2.0f / (6.0f + 2.0f)) bad = 1;
    if (bad) { std::printf("007dda30_TEST FAIL\n"); return 1; }
    std::printf("007dda30_TEST PASS\n");
    return 0;
}