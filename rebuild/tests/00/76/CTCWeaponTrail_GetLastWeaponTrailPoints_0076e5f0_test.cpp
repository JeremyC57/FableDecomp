#include <cstdio>
struct CPointPairVector { int collected; const void* from; };
struct CWeaponTrailList { int id; };
struct CWeaponTrailListVector { CWeaponTrailList* m_pFirst; const CWeaponTrailList& operator[](long i) const { return m_pFirst[i]; } };
struct CTCWeaponTrail {
    long NoTrails; CWeaponTrailListVector Trail;
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const { points.collected = trail.id; points.from = &trail; return true; }
    bool GetLastWeaponTrailPoints(CPointPairVector& points, long trailIndex) const
    {
        if (trailIndex < NoTrails) { if (trailIndex >= 0) return GetLastWeaponTrailPoints(points, Trail[trailIndex]); return GetLastWeaponTrailPoints(points, Trail[0]); }
        return false;
    }
};
int main()
{
    CWeaponTrailList lists[3]; lists[0].id = 10; lists[1].id = 11; lists[2].id = 12;
    CTCWeaponTrail w; w.NoTrails = 3; w.Trail.m_pFirst = lists; CPointPairVector p; p.collected = 0; p.from = 0;
    if (!w.GetLastWeaponTrailPoints(p, 2L) || p.collected != 12 || p.from != &lists[2]) { std::printf("0076e5f0_TEST FAIL\n"); return 1; }
    if (!w.GetLastWeaponTrailPoints(p, -5L) || p.collected != 10) { std::printf("0076e5f0_TEST FAIL\n"); return 1; }
    p.collected = 0; if (w.GetLastWeaponTrailPoints(p, 3L) || p.collected != 0) { std::printf("0076e5f0_TEST FAIL\n"); return 1; }
    std::printf("0076e5f0_TEST PASS\n");
    return 0;
}