// Behaviour test for CNavigatorAStar::CullWayPoints(long) (0x00A1C010): a model of the row's string-pulling loop -- from
// CullFront, test lines to CullBack (walking back from the last point) and drop every point in between once a line is clear;
// a budget of line tests per call; Culled once CullFront reaches the end. The line test (IsRouteClear) is modelled by a
// predicate: points on the same layer with no wall crossing between them.
#include <stdio.h>
#include <list>
struct WayPoint { float X, Y; unsigned char NavLayer; };
struct Navigator {
    std::list<WayPoint> Route;
    std::list<WayPoint>::iterator CullFront, CullBack;
    bool Culled;
    float WallX;                                     // a wall at x = WallX blocks lines that cross it
    int Tests;
    bool IsRouteClear(const WayPoint& a, const WayPoint& b) {
        ++Tests;
        if (a.NavLayer != b.NavLayer) return false;
        return !((a.X < WallX && b.X > WallX) || (a.X > WallX && b.X < WallX));
    }
    void Start() { CullFront = Route.begin(); CullBack = Route.end(); --CullBack; Culled = false; Tests = 0; }
    void CullWayPoints(long iterations) {
        long count = 0;
        while (count <= iterations && CullFront != Route.end()) {
            bool culled = false;
            while (count <= iterations && CullFront != CullBack && !culled) {
                ++count;
                if (IsRouteClear(*CullFront, *CullBack)) {
                    ++CullFront;
                    CullFront = Route.erase(CullFront, CullBack);
                    --CullFront;
                    culled = true;
                } else {
                    --CullBack;
                }
            }
            if (count <= iterations || CullFront == CullBack || culled) {
                ++CullFront;
                CullBack = Route.end();
                --CullBack;
            }
        }
        if (CullFront == Route.end()) Culled = true;
    }
};
int main()
{
    Navigator n; n.WallX = 100.0f;
    for (int i = 0; i < 6; ++i) { WayPoint p = { (float)i, (float)(i * i), 0 }; n.Route.push_back(p); }
    n.Start(); n.CullWayPoints(300);
    int ok = n.Culled && n.Route.size() == 2 && n.Route.front().X == 0.0f && n.Route.back().X == 5.0f;
    // a wall between points 2 and 3: the route keeps the points either side of it
    n.Route.clear(); n.WallX = 2.5f;
    for (int i = 0; i < 6; ++i) { WayPoint p = { (float)i, 0.0f, 0 }; n.Route.push_back(p); }
    n.Start(); n.CullWayPoints(300);
    std::list<WayPoint>::iterator it = n.Route.begin();
    ok = ok && n.Culled && n.Route.size() == 4 && (it++)->X == 0.0f && (it++)->X == 2.0f && (it++)->X == 3.0f && it->X == 5.0f;
    // a budget of one test per call: not culled after the first call, finished after enough calls
    n.Route.clear(); n.WallX = 100.0f;
    for (int i = 0; i < 6; ++i) { WayPoint p = { (float)i, 0.0f, (unsigned char)(i == 5) }; n.Route.push_back(p); }
    n.Start(); n.CullWayPoints(0);
    ok = ok && !n.Culled && n.Route.size() == 6;
    for (int k = 0; k < 20 && !n.Culled; ++k) n.CullWayPoints(0);
    ok = ok && n.Culled && n.Route.size() == 3;      // layer change at the last point keeps 0, 4, 5
    printf(ok ? "00a1c010_TEST PASS\n" : "00a1c010_TEST FAIL\n");
    return ok ? 0 : 1;
}