// Behaviour test for 0x00837B30 CThingTrackNode::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&): a model; checks nothing is added when the physical base fails, and the static mesh goes in at the position interpolated between the last and current positions with the graphic size scaled.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct V { float X, Y, Z; };
static V Interp(V last, V cur, float t) { V r = { (cur.X - last.X) * t + last.X, (cur.Y - last.Y) * t + last.Y, (cur.Z - last.Z) * t + last.Z }; return r; }
static bool Prepare(bool baseOk, V last, V cur, float t, float size, float scale, V& pos, float& outSize, int& adds)
{
    if (!baseOk) return false;
    pos = Interp(last, cur, t); outSize = size * scale; ++adds; return true;
}
int main()
{
    int ok = 1, adds = 0; V p; float s = 0; V a = { 0, 0, 0 }, b = { 10, 20, 30 };
    ok &= !Prepare(false, a, b, 0.5f, 2, 3, p, s, adds) && adds == 0;
    ok &= Prepare(true, a, b, 0.5f, 2, 3, p, s, adds) && adds == 1 && p.X == 5 && p.Y == 10 && p.Z == 15 && s == 6;
    std::printf(ok ? "00837B30_TEST PASS\n" : "00837B30_TEST FAIL\n"); return ok ? 0 : 1;
}
