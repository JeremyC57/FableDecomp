// Behaviour test for CAIStateGroup_JackDragon::FlyOutOfGround (0x00951B20): model copy with scripted stubs.
#include <cstdio>

struct V { float X, Y, Z; };
static bool g_bodgeAlive, g_enemyAlive, g_stopping, g_stopAfter;
static int g_killed, g_terminated, g_actions, g_routeLen;
static unsigned int g_seed;
static V g_route[8];
static unsigned int rotr13(unsigned int v) { return (v >> 13) | (v << 19); }
struct Dragon {
    long CurrentState, GoLeft;
    V Prev, Out1, Out2, Out3L, Out3R, Out4L, Out4R;
    bool Stopping() { return g_stopping; }
    void Push(const V& v) { g_route[g_routeLen++] = v; }
    void FlyOutOfGround()
    {
        if (g_bodgeAlive) { ++g_killed; g_bodgeAlive = false; }
        if (!g_enemyAlive) { ++g_terminated; return; }
        if (Stopping()) { ++g_terminated; return; }
        g_routeLen = 0;
        g_seed = g_seed * 0x24A1u + 0x24DFu;
        g_seed = rotr13(g_seed);
        if ((long)(g_seed % 100u) > 50) GoLeft = 1; else GoLeft = 0;
        if (GoLeft) { Push(Out1); Push(Out2); Push(Out3L); Push(Out4L); Prev = Out3L; }
        else { Push(Out1); Push(Out2); Push(Out3R); Push(Out4R); Prev = Out3R; }
        ++g_actions;
        if (g_stopAfter) g_stopping = true;
        bool stop = !g_enemyAlive || Stopping();
        if (stop) ++g_terminated; else CurrentState = 7;
    }
};

static int fail(int n) { std::printf("00951b20_TEST FAIL %d\n", n); return 1; }
static V mk(float x) { V v; v.X = x; v.Y = x + 1.0f; v.Z = x + 2.0f; return v; }

int main()
{
    Dragon d;
    d.Out1 = mk(1); d.Out2 = mk(2); d.Out3L = mk(3); d.Out3R = mk(4); d.Out4L = mk(5); d.Out4R = mk(6);
    // enemy dead: kill bodge, terminate, no action
    g_bodgeAlive = true; g_enemyAlive = false; g_stopping = false; g_stopAfter = false;
    g_killed = g_terminated = g_actions = 0; d.CurrentState = 0;
    d.FlyOutOfGround();
    if (g_killed != 1 || g_terminated != 1 || g_actions != 0) return fail(1);
    // run through both sides across several seeds
    int lefts = 0, rights = 0;
    for (unsigned int s = 0; s < 64; ++s) {
        g_seed = s * 977u; g_enemyAlive = true; g_stopping = false; g_terminated = 0; d.CurrentState = 0;
        d.FlyOutOfGround();
        if (g_routeLen != 4 || d.CurrentState != 7 || g_terminated != 0) return fail(2);
        if (d.GoLeft) { ++lefts; if (g_route[2].X != 3.0f || g_route[3].X != 5.0f || d.Prev.X != 3.0f) return fail(3); }
        else { ++rights; if (g_route[2].X != 4.0f || g_route[3].X != 6.0f || d.Prev.Z != 6.0f) return fail(4); }
    }
    if (lefts == 0 || rights == 0) return fail(5);
    // stopping after the action: terminate instead of choosing a state
    g_stopAfter = true; g_stopping = false; g_terminated = 0; d.CurrentState = 0;
    d.FlyOutOfGround();
    if (g_terminated != 1 || d.CurrentState != 0) return fail(6);
    std::printf("00951b20_TEST PASS\n");
    return 0;
}
