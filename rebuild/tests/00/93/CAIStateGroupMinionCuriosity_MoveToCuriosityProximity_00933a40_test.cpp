#include <cstdio>
struct CThing;
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
static long g_results[4] = { 4, 4, 2, 0 }; static int g_calls = 0; static float g_radius = 0; static float g_x = 0;
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10];
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long mt, bool a, bool b, bool c, CThing* l, bool d, float e)
    { g_radius = radius; g_x = pos.X; if (repeat != 2 || mt != 3 || !a || b || !c || l || !d || e != 0 || *reached) return -1; return g_results[g_calls++]; } };
struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase { C3DVector CuriosityPos; bool MoveToCuriosityProximity(float radius); };
bool CAIStateGroup_MinionCuriosity::MoveToCuriosityProximity(float radius)
{ bool reached = false; long result; do { result = AIAction_MoveToPosProximity(2, CuriosityPos, radius, &reached, 3, true, false, true, 0, true, 0.0f); } while (result == 4); return result == 2; }
int main()
{
    CAIStateGroup_MinionCuriosity g; g.CuriosityPos.X = 9; g.CuriosityPos.Y = 0; g.CuriosityPos.Z = 0;
    if (!g.MoveToCuriosityProximity(2.5f) || g_calls != 3 || g_radius != 2.5f || g_x != 9) { std::printf("00933a40_TEST FAIL\n"); return 1; }
    if (g.MoveToCuriosityProximity(1.0f) || g_calls != 4) { std::printf("00933a40_TEST FAIL\n"); return 1; }
    std::printf("00933a40_TEST PASS\n"); return 0;
}