// Behaviour test for 0x0076E4D0 CTCWeaponTrail::GetLastWeaponTrailPoints(vector<pair<C3DVector,C3DVector> >&,
// const list<CWeaponTrailEntry>&) const (C++03): the newest min(count, steps + 1) entries are appended oldest first as
// (Top, Bottom) pairs; false (nothing appended) for fewer than two entries.
#include <cstdio>
#include <list>
#include <vector>
#include <utility>

struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct Entry { V3 Top; V3 Bottom; float CreationTime; Entry(float k) : Top(k, 0, 0), Bottom(0, k, 0), CreationTime(k) { } };
static long g_WeaponTrailInterpolationSteps = 2;

static bool GetLastWeaponTrailPoints(std::vector<std::pair<V3, V3> >& points, const std::list<Entry>& trail)
{
    long count = (long)trail.size();
    if (count > 1) {
        long steps = g_WeaponTrailInterpolationSteps + 1;
        long n = count < steps ? count : steps;
        std::list<Entry>::const_iterator it = trail.end();
        for (long k = n; k > 0; --k) --it;           // advance(it, -n)
        for (; it != trail.end(); ++it)
            points.push_back(std::pair<V3, V3>(it->Top, it->Bottom));
        return true;
    }
    return false;
}
int main()
{
    bool ok = true;
    std::list<Entry> trail;
    std::vector<std::pair<V3, V3> > points;
    trail.push_back(Entry(1.0f));
    if (GetLastWeaponTrailPoints(points, trail) || !points.empty()) ok = false;          // one entry: false
    for (int i = 2; i <= 6; ++i) trail.push_back(Entry((float)i));
    if (!GetLastWeaponTrailPoints(points, trail) || points.size() != 3) ok = false;     // steps 2 -> the newest 3
    if (ok && (points[0].first.X != 4.0f || points[2].first.X != 6.0f || points[1].second.Y != 5.0f)) ok = false;
    g_WeaponTrailInterpolationSteps = 10;                                                // more than the trail: all 6
    points.clear();
    if (!GetLastWeaponTrailPoints(points, trail) || points.size() != 6 || points[0].first.X != 1.0f) ok = false;
    if (!ok) { std::printf("0076e4d0_TEST FAIL\n"); return 1; }
    std::printf("0076e4d0_TEST PASS\n");
    return 0;
}