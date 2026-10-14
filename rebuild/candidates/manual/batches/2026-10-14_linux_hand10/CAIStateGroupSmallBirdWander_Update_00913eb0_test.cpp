#include <cstdio>
// Model of CAIStateGroup_SmallBirdWander::Update @ 0x00913EB0: random flight position -> fly there -> terminate.
struct C3DVector { float X, Y, Z; };
static bool g_randomOk; static int g_executed; static C3DVector g_flownTo; static float g_radius; static bool g_ground;
struct Group {
    bool Terminated;
    const C3DVector& GetPosition() { static C3DVector p = {1, 2, 3}; return p; }
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool ground)
    { g_radius = radius; g_ground = ground; out->X = from.X + 10; out->Y = from.Y; out->Z = from.Z; return g_randomOk; }
    void ExecuteFlyingPath(const C3DVector& t) { ++g_executed; g_flownTo = t; }
    void Terminate() { Terminated = true; }
    void Update()
    {
        C3DVector target;
        if (!GetRandomFlightPos(GetPosition(), 15.0f, &target, true)) { Terminate(); return; }
        ExecuteFlyingPath(target);
        Terminate();
    }
};
int main()
{
    Group g; g.Terminated = false; g_randomOk = true; g_executed = 0;
    g.Update();
    if (!g.Terminated || g_executed != 1 || g_flownTo.X != 11 || g_radius != 15.0f || !g_ground) { std::printf("00913eb0_TEST FAIL\n"); return 1; }
    g.Terminated = false; g_randomOk = false; g.Update();
    if (!g.Terminated || g_executed != 1) { std::printf("00913eb0_TEST FAIL\n"); return 1; }
    std::printf("00913eb0_TEST PASS\n"); return 0;
}
