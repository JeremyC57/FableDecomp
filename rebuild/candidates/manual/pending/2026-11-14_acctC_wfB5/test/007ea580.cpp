// Behaviour test for 0x007EA580 CTCTargeted::GetAllTargetPoints: every "TARGET_CURSOR_POINT" helper point of the thing's graphic
// appearance is appended to the list with its index (a helper that cannot be read falls back to the default targeting position); a
// thing without such helpers (or without a graphic appearance) contributes exactly one entry: the default targeting position, index 0.
// Existing entries of the list are kept.
#include <cstdio>
#include <vector>
#include <utility>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static bool same(const V& a, const V& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
struct Appearance {
    int numHelpers; V helpers[4]; bool readable[4];
    int GetNoHelperPoints() const { return numHelpers; }
    bool GetHelperPoint(long index, V& out) const { if (!readable[index]) return false; out = helpers[index]; return true; }
};
struct Targeted {
    const Appearance* appearance; V defaultPos; float lastScale; mutable int defaultCalls;
    V DefaultTargetingPosition(float scale) const { ++defaultCalls; return V(defaultPos.X * scale, defaultPos.Y * scale, defaultPos.Z * scale); }
    long GetNumTargetPoints() const { if (appearance) return appearance->GetNoHelperPoints(); return 0; }
    V GetTargetPoint(long index, float scale) const {
        if (index >= 0) { V point; if (appearance->GetHelperPoint(index, point)) return point; }
        return DefaultTargetingPosition(scale);
    }
    void GetAllTargetPoints(std::vector<std::pair<V, long> >& points, float scale) const {
        long num_points = GetNumTargetPoints();
        long i = 0;
        if (num_points != 0) {
            for (; i != num_points; ++i)
                points.push_back(std::pair<V, long>(GetTargetPoint(i, scale), i));
        } else {
            points.push_back(std::pair<V, long>(GetTargetPoint(-1, scale), 0));
        }
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Appearance a; a.numHelpers = 3;
    a.helpers[0] = V(1, 2, 3); a.helpers[1] = V(4, 5, 6); a.helpers[2] = V(7, 8, 9); a.helpers[3] = V(10, 11, 12);
    a.readable[0] = a.readable[1] = a.readable[2] = a.readable[3] = true;
    Targeted t; t.appearance = &a; t.defaultPos = V(100, 200, 300); t.defaultCalls = 0;
    std::vector<std::pair<V, long> > points;
    t.GetAllTargetPoints(points, 1.0f);
    check(points.size() == 3 && t.defaultCalls == 0, "three helpers listed");
    check(same(points[0].first, V(1, 2, 3)) && points[0].second == 0, "helper 0");
    check(same(points[1].first, V(4, 5, 6)) && points[1].second == 1, "helper 1");
    check(same(points[2].first, V(7, 8, 9)) && points[2].second == 2, "helper 2");
    // appended, not replaced
    a.numHelpers = 1; t.GetAllTargetPoints(points, 1.0f);
    check(points.size() == 4 && same(points[3].first, V(1, 2, 3)) && points[3].second == 0 && same(points[0].first, V(1, 2, 3)), "appends to the list");
    // an unreadable helper falls back to the default position but keeps its index
    points.clear(); a.numHelpers = 2; a.readable[1] = false; t.GetAllTargetPoints(points, 0.5f);
    check(points.size() == 2 && same(points[0].first, V(1, 2, 3)) && same(points[1].first, V(50, 100, 150)) && points[1].second == 1 && t.defaultCalls == 1,
          "unreadable helper -> default position");
    // no helpers: one default entry with index 0
    points.clear(); a.numHelpers = 0; t.defaultCalls = 0; t.GetAllTargetPoints(points, 2.0f);
    check(points.size() == 1 && same(points[0].first, V(200, 400, 600)) && points[0].second == 0 && t.defaultCalls == 1, "no helpers -> default point");
    // no appearance at all: the same single default entry (and the appearance is never dereferenced)
    points.clear(); t.appearance = 0; t.defaultCalls = 0; t.GetAllTargetPoints(points, 1.0f);
    check(points.size() == 1 && same(points[0].first, V(100, 200, 300)) && points[0].second == 0 && t.defaultCalls == 1, "no appearance -> default point");
    if (fails == 0) std::printf("007ea580_TEST PASS\n");
    return fails;
}
