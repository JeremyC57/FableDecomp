// Behaviour test for (anonymous)::CThingFilter_IsEvadeExplosion::operator() (0x0080DC90), g++ model.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V g_lineOut; static V g_seenTravel, g_seenLine; static bool g_away;
static bool Filter(bool rolling, V pos, V prev)
{
    if (!rolling) return false;
    float dx = pos.X - prev.X, dy = pos.Y - prev.Y;
    float len = std::sqrt(dy * dy + dx * dx);
    if (!(len > 1e-4f)) return false;
    V travel = { dx / len, dy / len, 0.0f };
    V line = g_lineOut; line.Z = 0.0f;
    if (!(line.Y * line.Y + line.X * line.X > 1e-8f)) return false;
    float l = std::sqrt(line.X * line.X + line.Y * line.Y); line.X /= l; line.Y /= l;
    g_seenTravel = travel; g_seenLine = line;
    return g_away;
}
int main()
{
    V pos = { 4, 5, 9 }, prev = { 1, 1, 2 };
    g_lineOut.X = 0; g_lineOut.Y = 2; g_lineOut.Z = 7; g_away = true;
    int ok = Filter(true, pos, prev) && std::fabs(g_seenTravel.X - 0.6f) < 1e-5f && std::fabs(g_seenTravel.Y - 0.8f) < 1e-5f && std::fabs(g_seenLine.Y - 1.0f) < 1e-5f;
    ok &= !Filter(false, pos, prev);
    ok &= !Filter(true, pos, pos);
    g_lineOut.X = 0; g_lineOut.Y = 0; g_lineOut.Z = 5; ok &= !Filter(true, pos, prev);
    g_lineOut.Y = 1; g_away = false; ok &= !Filter(true, pos, prev);
    std::printf(ok ? "0080dc90_TEST PASS\n" : "0080dc90_TEST FAIL\n");
    return ok ? 0 : 1;
}
