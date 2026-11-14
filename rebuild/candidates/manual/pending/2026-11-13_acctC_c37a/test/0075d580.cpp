// Behaviour test for 0x0075D580 CTCCameraPointScriptedSpline::UpdateCameraParams: with any of the five splines missing the index is
// returned untouched. Otherwise the timing spline maps the index to t (X, clamped to 0..1); position and view direction come from their
// splines at t (the direction normalised, both moved into the caller's frame), the animation speed is the speed spline's X, fov / roll
// the scalar spline's Y / Z. The next index is index + the current key camera's delta, wrapped to the start node's fraction once it
// reaches the end node's fraction. When the next index is in another key camera (or the index is exactly the start fraction) the
// crossing is consumed: a crossing marked invalid only clears the mark; otherwise the next key's pause (whole seconds * frames per
// second) and event are latched. Reaching the end node with no pause sets `finished`. A pending event is broadcast as message 0x2B,
// recorded and cleared. While a pause is running it counts down by one, the index is held and the next crossing is marked invalid.
#include <cstdio>
#include <string>
#include <vector>
struct Key { float delta, pause; std::string event; };
struct Spline {
    bool splines; std::vector<Key> keys; bool nextCrossInvalid; float pauseFor, animationSpeed; long startOn, endOn;
    std::string toSend; std::vector<std::string> messages; int broadcasts; int fps;
    float timing(float i) const { return i * 2.0f - 0.25f; }              // deliberately leaves 0..1
    long keyOf(float t) const { if (t >= 1.0f) return (long)keys.size() - 1; return (long)((keys.size() - 1) * t); }
    float Update(float index, float pos[3], float dir[3], float& fov, float& roll, bool& finished) {
        if (!splines) return index;
        float t = timing(index); if (t < 0.0f) t = 0.0f; else if (t > 1.0f) t = 1.0f;
        pos[0] = t + 100.0f; pos[1] = t; pos[2] = 0.0f;                     // "transformed" position: origin (100,0,0)
        dir[0] = 0.0f; dir[1] = 1.0f; dir[2] = 0.0f;                        // (0, 3t+1, 0) normalised
        animationSpeed = t * 4.0f; fov = t * 10.0f; roll = t * 20.0f;
        long key = keyOf(index);
        float next = index + keys[key].delta;
        long nextKey = keyOf(next);
        if (next >= (float)endOn / (float)(keys.size() - 1)) next = (float)startOn / (float)(keys.size() - 1);
        if (key != nextKey || index == (float)startOn / (float)(keys.size() - 1)) {
            if (nextCrossInvalid) nextCrossInvalid = false;
            else { pauseFor = (float)((long)keys[nextKey].pause * fps); toSend = keys[nextKey].event; }
            if (nextKey == endOn && pauseFor == 0.0f) finished = true;
        }
        if (!toSend.empty()) { ++broadcasts; messages.push_back(toSend); toSend = ""; }
        if (pauseFor > 0.0f) { pauseFor -= 1.0f; next = index; nextCrossInvalid = true; }
        return next;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static Spline make() {
    Spline s; s.splines = true; s.nextCrossInvalid = false; s.pauseFor = 0.0f; s.animationSpeed = -1.0f; s.startOn = 0; s.endOn = 4;
    s.broadcasts = 0; s.fps = 30;
    for (int i = 0; i < 5; ++i) { Key k; k.delta = 0.125f; k.pause = 0.0f; s.keys.push_back(k); }
    return s;
}
int main() {
    float pos[3] = { 9, 9, 9 }, dir[3] = { 9, 9, 9 }, fov = -1, roll = -1; bool finished = false;
    Spline s = make(); s.splines = false;
    check(s.Update(0.3f, pos, dir, fov, roll, finished) == 0.3f && pos[0] == 9 && fov == -1 && s.animationSpeed == -1.0f, "missing spline");
    s = make();
    float r = s.Update(0.3125f, pos, dir, fov, roll, finished);             // t = 0.375; key 1 -> next 0.4375, key 1: no crossing
    check(r == 0.4375f && fov == 3.75f && roll == 7.5f && pos[0] == 100.375f && dir[1] == 1.0f && s.animationSpeed == 1.5f, "plain step");
    check(!finished && s.broadcasts == 0 && s.pauseFor == 0.0f, "no crossing");
    r = s.Update(0.9f, pos, dir, fov, roll, finished);                      // timing 1.55 clamps to 1
    check(fov == 10.0f && roll == 20.0f, "t clamped high");
    check(r == 0.0f && finished, "wraps to the start at the end node and finishes");
    s.Update(0.05f, pos, dir, fov, roll, finished);                         // timing -0.15 clamps to 0
    check(fov == 0.0f && s.animationSpeed == 0.0f, "t clamped low");
    s = make(); finished = false; s.keys[2].pause = 2.5f; s.keys[2].event = "CUT";
    r = s.Update(0.4375f, pos, dir, fov, roll, finished);                     // key 1 -> next 0.5625 = key 2: crossing with pause + event
    check(s.pauseFor == 59.0f && r == 0.4375f && s.nextCrossInvalid, "pause latched (2 whole seconds * 30) and counted down, index held");
    check(s.broadcasts == 1 && s.messages.size() == 1 && s.messages[0] == "CUT" && s.toSend.empty() && !finished, "event broadcast once");
    s.pauseFor = 0.0f;
    r = s.Update(0.4375f, pos, dir, fov, roll, finished);                     // the same crossing again: marked invalid, only cleared
    check(r == 0.5625f && !s.nextCrossInvalid && s.pauseFor == 0.0f && s.broadcasts == 1, "invalid crossing consumed");
    s = make(); finished = false; s.startOn = 1;
    r = s.Update(0.25f, pos, dir, fov, roll, finished);                     // index == start fraction: counts as a crossing (key 1 -> 1)
    s.keys[1].pause = 1.0f;
    s.nextCrossInvalid = false; float r2 = s.Update(0.25f, pos, dir, fov, roll, finished);
    check(r == 0.375f && r2 == 0.25f && s.pauseFor == 29.0f, "start fraction is a crossing");
    s = make(); finished = false; s.endOn = 2; s.keys[2].pause = 1.0f;
    s.Update(0.4375f, pos, dir, fov, roll, finished);                         // reaches the end node but a pause is pending
    check(!finished, "end node with a pause does not finish");
    s = make(); finished = false; s.endOn = 2;
    r = s.Update(0.4375f, pos, dir, fov, roll, finished);                     // next 0.5625 >= 2/4 -> wraps to 0; key 2 == end node
    check(finished && r == 0.0f, "end node finishes and wraps");
    if (!fails) std::printf("0075d580_TEST PASS\n");
    return fails;
}
