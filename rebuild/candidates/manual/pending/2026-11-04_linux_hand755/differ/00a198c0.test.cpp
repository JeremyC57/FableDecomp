// Behaviour test for CNavigatorFlyer::GetRandomNavigablePos (0x00A198C0), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static bool g_node = true, g_switchedOff = false, g_sameMap = true;
static float FindZ(V p) { return p.Z + 1.0f; }
static V Pos(V position, float distance, float dx, float dy, V heightOffset)
{
    float l = std::sqrt(dx * dx + dy * dy); dx /= l; dy /= l;
    V target = { position.X + dx * distance, position.Y + dy * distance, position.Z };
    if (g_node && !g_switchedOff && g_sameMap) {
        V q = { heightOffset.X + target.X, heightOffset.Y + target.Y, heightOffset.Z + target.Z };
        V r = { target.X, target.Y, FindZ(q) }; return r;
    }
    return position;
}
int main()
{
    V p = { 1, 2, 3 }, h = { 0, 0, 10 };
    V r = Pos(p, 5, 3, 4, h);
    int ok = std::fabs(r.X - 4) < 1e-5f && std::fabs(r.Y - 6) < 1e-5f && std::fabs(r.Z - 14) < 1e-5f;
    g_sameMap = false; r = Pos(p, 5, 3, 4, h); ok &= r.X == 1 && r.Y == 2 && r.Z == 3;
    g_sameMap = true; g_switchedOff = true; r = Pos(p, 5, 3, 4, h); ok &= r.X == 1;
    std::printf(ok ? "00a198c0_TEST PASS\n" : "00a198c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
