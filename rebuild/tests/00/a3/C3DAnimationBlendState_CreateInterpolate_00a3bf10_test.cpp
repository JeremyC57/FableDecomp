// Behaviour test for C3DAnimationBlendState::CreateInterpolate (0x00A3BF10): transition states (type 2) interpolate their start
// and end states recursively and lerp the progress; plain states copy the animation index and lerp the time only when both
// indices agree (else index -1, time left default).
#include <stdio.h>
#include <math.h>

struct State {
    int type; State* start; State* end; float progress; long index; float time;
};
static State g_pool[64]; static int g_used;
static State* New(int type) { State* s = &g_pool[g_used++]; s->type = type; s->start = s->end = 0; s->progress = 0.0f; s->index = -1; s->time = 0.0f; return s; }
static State* CreateInterpolate(const State& a, const State& b, float t)
{
    if (a.type == 2) {
        State* trans = New(2);
        trans->start = CreateInterpolate(*a.start, *b.start, t);
        trans->end = CreateInterpolate(*a.end, *b.end, t);
        const float start = a.progress;
        trans->progress = (start + (b.progress - start) * t);
        return trans;
    }
    State* s = New(0);
    if (a.index == b.index) { s->index = a.index; const float start = a.time; s->time = start + (b.time - start) * t; }
    return s;
}
static bool Near(float a, float b) { return fabs(a - b) < 0.0001f; }
int main()
{
    int fails = 0;
    State a0 = { 0, 0, 0, 0, 4, 1.0f }, b0 = { 0, 0, 0, 0, 4, 3.0f };
    State* s = CreateInterpolate(a0, b0, 0.25f);
    if (s->type != 0 || s->index != 4 || !Near(s->time, 1.5f)) { printf("plain FAIL\n"); ++fails; }
    State c0 = { 0, 0, 0, 0, 5, 9.0f };
    s = CreateInterpolate(a0, c0, 0.5f);
    if (s->index != -1 || s->time != 0.0f) { printf("mismatch FAIL\n"); ++fails; }
    State ta = { 2, &a0, &a0, 0.2f, -1, 0 }, tb = { 2, &b0, &c0, 0.6f, -1, 0 };
    s = CreateInterpolate(ta, tb, 0.5f);
    if (s->type != 2 || !Near(s->progress, 0.4f) || !Near(s->start->time, 2.0f) || s->start->index != 4 || s->end->index != -1) { printf("transition FAIL\n"); ++fails; }
    if (fails) { printf("00a3bf10_TEST FAIL\n"); return 1; }
    printf("00a3bf10_TEST PASS\n");
    return 0;
}