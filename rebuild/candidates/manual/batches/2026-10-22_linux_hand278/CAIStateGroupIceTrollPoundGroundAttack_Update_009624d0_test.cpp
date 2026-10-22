// Behaviour test for CAIStateGroup_IceTrollPoundGroundAttack::Update (0x009624D0): model copy with scripted stubs.
#include <cstdio>
#include <cmath>
#include <cstring>

struct V { float X, Y, Z; };
static bool g_alive, g_knockedDown, g_runnable;
static int g_riseResult, g_animResult, g_stabs, g_term, g_rose;
static const char* g_anim;
static V g_targetPos, g_myPos, g_facing;
static void Normalise(V& v) { float m = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); v.X /= m; v.Y /= m; v.Z /= m; }
static void Update()
{
    if (g_alive) {
        if (g_knockedDown) { ++g_rose; if (g_riseResult == 1) { ++g_term; return; } }
        if (g_alive) {
            V d; d.X = g_targetPos.X - g_myPos.X; d.Y = g_targetPos.Y - g_myPos.Y; d.Z = 0.0f;
            const float eps = 1e-4f;
            if (d.X * d.X + d.Y * d.Y > eps * eps) Normalise(d);
            V f = g_facing; f.Z = 0.0f;
            if (f.X * f.X + f.Y * f.Y > eps * eps) Normalise(f);
            float dot = f.X * d.X + f.Y * d.Y + f.Z * d.Z;
            g_anim = dot < std::cos(1.0471976115464443) ? "LOOK_BACKWARD" : "LOOK_FORWARD";
            if (g_animResult == 1) { ++g_term; return; }
            if (g_runnable) ++g_stabs;
        }
    }
    ++g_term;
}
static int fail(int n) { std::printf("009624d0_TEST FAIL %d\n", n); return 1; }
static void reset()
{
    g_alive = g_runnable = true; g_knockedDown = false; g_riseResult = g_animResult = 2; g_stabs = g_term = g_rose = 0; g_anim = "";
    g_myPos.X = 0; g_myPos.Y = 0; g_myPos.Z = 0; g_facing.X = 1; g_facing.Y = 0; g_facing.Z = 0.5f;
}
int main()
{
    reset(); g_targetPos.X = 10; g_targetPos.Y = 1; g_targetPos.Z = 50; Update();
    if (std::strcmp(g_anim, "LOOK_FORWARD") != 0 || g_stabs != 1 || g_term != 1) return fail(1);
    reset(); g_targetPos.X = -10; g_targetPos.Y = 0; g_targetPos.Z = 0; Update();
    if (std::strcmp(g_anim, "LOOK_BACKWARD") != 0 || g_stabs != 1) return fail(2);
    reset(); g_targetPos.X = 1; g_targetPos.Y = 2; Update();          // ~63 degrees off: backward
    if (std::strcmp(g_anim, "LOOK_BACKWARD") != 0) return fail(3);
    reset(); g_targetPos.X = 1; g_targetPos.Y = 1.5f; Update();       // ~56 degrees off: forward
    if (std::strcmp(g_anim, "LOOK_FORWARD") != 0) return fail(4);
    reset(); g_knockedDown = true; g_riseResult = 1; Update();
    if (g_rose != 1 || std::strcmp(g_anim, "") != 0 || g_term != 1) return fail(5);
    reset(); g_animResult = 1; g_targetPos.X = 5; Update();
    if (g_stabs != 0 || g_term != 1) return fail(6);
    reset(); g_runnable = false; Update();
    if (g_stabs != 0 || g_term != 1) return fail(7);
    std::printf("009624d0_TEST PASS\n");
    return 0;
}
