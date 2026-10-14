#include <cstdio>
struct CThing {};
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} C3DVector() {} };
static C3DVector g_pos; static float g_radius; static long g_type; static int g_terminated; static bool* g_reached;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long type, bool, bool, bool, CThing*, bool, float)
    { g_pos = pos; g_radius = radius; g_type = type; g_reached = reached; *reached = true; return repeat; }
    void Terminate() { ++g_terminated; }
    unsigned char _pad[0x10];
};
struct CAIStateGroup_AvoidRegionExit : CAIStateGroupBase {
    C3DVector Position;
    virtual void Update() { bool reached; AIAction_MoveToPosProximity(1, Position, 2.0f, &reached, 5, true, false, true, 0, true, 0.0f); Terminate(); }
};
int main()
{
    CAIStateGroup_AvoidRegionExit g; g.Position.X = 1.5f; g.Position.Y = -2.0f; g.Position.Z = 3.25f;
    g.Update();
    if (g_pos.X != 1.5f || g_pos.Y != -2.0f || g_pos.Z != 3.25f || g_radius != 2.0f || g_type != 5 || g_terminated != 1 || g_reached == 0) { std::printf("0092bc60_TEST FAIL\n"); return 1; }
    std::printf("0092bc60_TEST PASS\n"); return 0;
}
