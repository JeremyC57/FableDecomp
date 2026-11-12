#include <cstdio>
#include <cmath>
// behaviour test: CAIStateGroup_OnHeroPush::Update -- with a push component: normalise the push direction into PushDir, face
// along it when the push comes from behind (forward . push >= 0) or against it otherwise, play the pushed animation only on the
// first frames of the push (FrameSincePushed <= 1; FORWARD from behind, BACKWARD from the front), post the hero's deed when a
// living hero carries the deed log, then drop the push component and terminate; no push component: nothing at all.
struct Vec { float X, Y, Z; };
static float Angle(float x, float y)
{
    float a = (float)std::atan2(x, y) * 0.15915494f;
    if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; }
    return a;
}
struct World {
    bool hasPush; Vec pushDir; long frames; Vec facing; bool heroAlive, heroHasLog;
    // results
    Vec dir; float angle; const char* anim; int deeds; bool removed, terminated;
};
static void Update(World& w)
{
    if (!w.hasPush) return;
    Vec d = w.pushDir;
    float n = 1.0f / std::sqrt(d.Z * d.Z + d.X * d.X + d.Y * d.Y);
    d.X *= n; d.Y *= n; d.Z *= n;
    w.dir = d;
    float dot = w.facing.Y * d.Y + 0.0f * d.Z + w.facing.X * d.X;
    bool fromBehind = dot >= 0.0f;
    w.angle = fromBehind ? Angle(d.X, d.Y) : Angle(-d.X, -d.Y);
    if (w.frames <= 1) w.anim = fromBehind ? "PUSHED_FORWARD" : "PUSHED_BACKWARD";
    if (w.heroAlive && w.heroHasLog) ++w.deeds;
    w.removed = true;
    w.terminated = true;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    World w = { true, { 0.0f, 3.0f, 4.0f }, 0, { 0.0f, 1.0f, 0.0f }, true, true, { 0, 0, 0 }, -1.0f, 0, 0, false, false };
    Update(w);
    if (!Near(w.dir.Y, 0.6f) || !Near(w.dir.Z, 0.8f) || !Near(w.angle, 0.0f) || !w.anim || w.anim[7] != 'F' || w.deeds != 1
        || !w.removed || !w.terminated) { std::printf("0092b900_TEST FAIL\n"); return 1; }
    World f = { true, { 0.0f, -2.0f, 0.0f }, 5, { 0.0f, 1.0f, 0.0f }, false, true, { 0, 0, 0 }, -1.0f, 0, 0, false, false };
    Update(f);   // from the front: faces against the push (angle of (0, 1) = 0), no animation after frame 1, dead hero: no deed
    if (!Near(f.angle, 0.0f) || f.anim != 0 || f.deeds != 0 || !f.terminated) { std::printf("0092b900_TEST FAIL\n"); return 1; }
    World s = { true, { 1.0f, 0.0f, 0.0f }, 1, { 0.0f, 1.0f, 0.0f }, true, true, { 0, 0, 0 }, -1.0f, 0, 0, false, false };
    Update(s);   // sideways counts as from behind (dot == 0): quarter turn, FORWARD animation on frame 1
    if (!Near(s.angle, 0.25f) || !s.anim || s.anim[7] != 'F') { std::printf("0092b900_TEST FAIL\n"); return 1; }
    World n = { false, { 1.0f, 0.0f, 0.0f }, 0, { 0.0f, 1.0f, 0.0f }, true, true, { 0, 0, 0 }, -1.0f, 0, 0, false, false };
    Update(n);
    if (n.terminated || n.removed || n.deeds) { std::printf("0092b900_TEST FAIL\n"); return 1; }
    std::printf("0092b900_TEST PASS\n");
    return 0;
}