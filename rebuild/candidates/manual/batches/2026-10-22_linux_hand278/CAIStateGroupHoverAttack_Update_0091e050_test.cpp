// Behaviour test for CAIStateGroup_HoverAttack::Update (0x0091E050): model copy with scripted stubs.
#include <cstdio>

struct V { float X, Y, Z; };
static bool g_targetAlive, g_registered, g_attack, g_runnable;
static int g_moveScript[16], g_moveIdx, g_moves, g_actions, g_terminated, g_ready, g_actionResult;
static V g_targetPos, g_myPos;
static bool g_hasAction;
struct Group {
    float minDistance;
    bool IsAlive() { return g_targetAlive; }
    bool IsRegistered() { return g_registered; }
    bool IsToAttack() { return g_attack; }
    bool IsRunnableLeaf() { return g_runnable; }
    int Move() { ++g_moves; int r = g_moveScript[g_moveIdx]; if (g_moveIdx < 15) ++g_moveIdx; return r; }
    void Terminate() { ++g_terminated; }
    void Update()
    {
        if (IsRegistered()) ++g_ready;
        while (IsAlive() && IsToAttack() && IsRunnableLeaf()) {
            bool inRange = false;
            for (;;) {
                if (!IsAlive() || !IsRegistered() || !IsToAttack()) { Terminate(); return; }
                int result = Move();
                if (IsAlive()) {
                    float dx = g_targetPos.X - g_myPos.X, dy = g_targetPos.Y - g_myPos.Y;
                    inRange = dx * dx + dy * dy < minDistance * minDistance;
                }
                if (result == 4) { if (inRange) break; continue; }
                if (result == 2 || inRange) break;
                Terminate(); return;
            }
            if (!IsAlive() || !IsRegistered()) { Terminate(); return; }
            if (!g_hasAction) { Terminate(); return; }
            ++g_actions;
            if (g_actionResult != 2) { Terminate(); return; }
            if (g_actions >= 2) g_runnable = false;
        }
        Terminate();
    }
};

static int fail(int n) { std::printf("0091e050_TEST FAIL %d\n", n); return 1; }
static void reset()
{
    g_targetAlive = g_registered = g_attack = g_runnable = g_hasAction = true;
    g_moveIdx = g_moves = g_actions = g_terminated = g_ready = 0; g_actionResult = 2;
    for (int i = 0; i < 16; ++i) g_moveScript[i] = 2;
    g_myPos.X = g_myPos.Y = g_myPos.Z = 0.0f; g_targetPos.X = 10.0f; g_targetPos.Y = 0.0f; g_targetPos.Z = 0.0f;
}

int main()
{
    Group g; g.minDistance = 5.0f;
    // two successful arrive+act passes then not runnable
    reset(); g.Update();
    if (g_ready != 1 || g_actions != 2 || g_moves != 2 || g_terminated != 1) return fail(1);
    // keep moving (4) until in range
    reset(); g_moveScript[0] = 4; g_moveScript[1] = 4; g_moveScript[2] = 2; g_runnable = true;
    g.Update();
    if (g_moves != 4 || g_actions != 2) return fail(2);
    // move failure while out of range terminates
    reset(); g_moveScript[0] = 1; g.Update();
    if (g_actions != 0 || g_terminated != 1) return fail(3);
    // move failure while in XY range still acts
    reset(); g_moveScript[0] = 1; g_moveScript[1] = 1; g_targetPos.X = 3.0f; g_targetPos.Z = 100.0f; g.Update();
    if (g_actions != 2) return fail(4);
    // no action -> terminate
    reset(); g_hasAction = false; g.Update();
    if (g_actions != 0 || g_terminated != 1) return fail(5);
    // not registered -> no ready, terminate in hover loop
    reset(); g_registered = false; g.Update();
    if (g_ready != 0 || g_moves != 0 || g_terminated != 1) return fail(6);
    std::printf("0091e050_TEST PASS\n");
    return 0;
}
