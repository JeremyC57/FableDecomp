#include <cstdio>
#include <cmath>
// Behaviour copy of CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPos (0x00970930).
struct V { float X, Y, Z; };
struct Nav { bool wanderOk, reachOk; V lastCentre; float lastRadius; };
static bool Hidden(bool enemyAlive, V enemy, V cover, float coverRadius, Nav& nav, V& result) {
    if (!enemyAlive) return false;
    V away = { cover.X - enemy.X, cover.Y - enemy.Y, 0.0f };
    float len = (float)std::sqrt(away.X * away.X + away.Y * away.Y + away.Z * away.Z);
    if (len != 0) { away.X /= len; away.Y /= len; away.Z /= len; }
    float radius = coverRadius * 2.0f; if (radius < 1.5f) radius = 1.5f;
    V target = { cover.X + away.X * radius * 0.75f, cover.Y + away.Y * radius * 0.75f, cover.Z + away.Z * radius * 0.75f };
    nav.lastCentre = target; nav.lastRadius = radius * 0.15f;
    if (!nav.wanderOk) return false;
    result = target;
    return nav.reachOk;
}
static V Make(float x, float y, float z) { V v; v.X = x; v.Y = y; v.Z = z; return v; }
int main() {
    Nav n; n.wanderOk = true; n.reachOk = true; n.lastCentre = Make(0, 0, 0); n.lastRadius = 0; V r;
    V origin = Make(0, 0, 0), east = Make(10, 0, 0), north = Make(0, 4, 0);
    if (!Hidden(true, origin, east, 2.0f, n, r) || std::fabs(n.lastCentre.X - 13.0f) > 1e-4f || std::fabs(n.lastRadius - 0.6f) > 1e-4f) { std::printf("00970930_TEST FAIL\n"); return 1; }
    if (!Hidden(true, origin, north, 0.1f, n, r) || std::fabs(n.lastCentre.Y - 5.125f) > 1e-4f) { std::printf("00970930_TEST FAIL\n"); return 1; }   // radius clamps to 1.5
    n.reachOk = false; if (Hidden(true, origin, east, 2.0f, n, r)) { std::printf("00970930_TEST FAIL\n"); return 1; }
    if (Hidden(false, origin, east, 2.0f, n, r)) { std::printf("00970930_TEST FAIL\n"); return 1; }
    std::printf("00970930_TEST PASS\n"); return 0;
}
