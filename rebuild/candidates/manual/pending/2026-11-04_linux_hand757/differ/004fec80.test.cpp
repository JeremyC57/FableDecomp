// Behaviour test for 0x004FEC80 CWorldMap::UpdateDestroyedThemeAreas: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <vector>
struct Box { long l, t, r, b; void Extend(long x, long y) { if (x < l) l = x; if (y < t) t = y; if (x > r) r = x; if (y > b) b = y; } };
static std::vector<Box> reported;
static void Update(std::vector<std::pair<long,long> >& q)
{
    if (q.empty()) return;
    Box area = { q[0].first, q[0].second, q[0].first, q[0].second };
    for (size_t i = 0; i < q.size(); ++i) {
        Box e = area; e.Extend(q[i].first, q[i].second);
        if (e.r - e.l > 5 || e.b - e.t > 5) { Box o = area; o.l--; o.t--; o.r++; o.b++; reported.push_back(o); Box n = { q[i].first, q[i].second, q[i].first, q[i].second }; area = n; }
        else area = e;
    }
    area.l--; area.t--; area.r++; area.b++; reported.push_back(area); q.clear();
}
int main() {
    std::vector<std::pair<long,long> > q; q.push_back(std::make_pair(0L,0L)); q.push_back(std::make_pair(3L,3L)); q.push_back(std::make_pair(10L,0L));
    Update(q);
    int ok = q.empty() && reported.size() == 2 && reported[0].l == -1 && reported[0].r == 4 && reported[1].l == 9 && reported[1].r == 11;
    reported.clear(); Update(q); ok &= reported.empty();
    std::printf(ok ? "004fec80_TEST PASS\n" : "004fec80_TEST FAIL\n");
    return ok ? 0 : 1;
}
