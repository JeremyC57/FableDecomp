// Behaviour test for CombineBlendedBaseAnimationsBoneList at 0x00AA0DC0: recursion, depth cut-off and blend weights.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); }
struct State { int type; float time; float progress; State* start; State* end; };
static float Lerp(float a, float b, float t) { return a + (b - a) * t; }
static void Combine(const State& a, const State& b, float blend, int out, unsigned int count, unsigned int depth)
{
    char buf[64];
    if (a.type == 0) {
        float time = Lerp(a.time, b.time, blend);
        for (unsigned int i = 0; i < count; ++i) { std::sprintf(buf, "S%d@%g;", out + (int)i, time); Log(buf); }
    } else if (a.type == 2) {
        unsigned int child = depth + 1;
        Combine(*a.start, *b.start, blend, out, count, child);
        if (child < 5) {
            Combine(*a.end, *b.end, blend, out + (int)count, count, child);
            std::sprintf(buf, "B%d,%d@%g;", out, out + (int)count, Lerp(a.progress, b.progress, blend)); Log(buf);
        }
    }
}
int main()
{
    State a0 = { 0, 1.0f, 0, 0, 0 }, b0 = { 0, 3.0f, 0, 0, 0 };
    State a1 = { 0, 2.0f, 0, 0, 0 }, b1 = { 0, 4.0f, 0, 0, 0 };
    State ta = { 2, 0, 0.0f, &a0, &a1 }, tb = { 2, 0, 1.0f, &b0, &b1 };
    Combine(ta, tb, 0.5f, 0, 2, 0);
    int ok = std::strcmp(g_log, "S0@2;S1@2;S2@3;S3@3;B0,2@0.5;") == 0;
    g_log[0] = 0;
    Combine(ta, tb, 0.5f, 0, 1, 4);                       // child depth 5: start state only
    ok = ok && std::strcmp(g_log, "S0@2;") == 0;
    std::printf(ok ? "00aa0dc0_TEST PASS\n" : "00aa0dc0_TEST FAIL\n%s\n", g_log);
    return ok ? 0 : 1;
}