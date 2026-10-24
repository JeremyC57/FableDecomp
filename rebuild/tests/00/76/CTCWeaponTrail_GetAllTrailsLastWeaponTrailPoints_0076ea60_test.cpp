// Behaviour test for 0x0076EA60 CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints(points) const
#include <cstdio>

struct Points { int Count; int Trail; };                 // stand-in for vector<pair<C3DVector,C3DVector> >

struct Out {
    Points Items[8];
    int Size;
    void push_back(const Points& p) { Items[Size++] = p; }
};

struct WeaponTrail {
    long NoTrails;
    const int* TrailLengths;                             // the per-trail point counts (Trail lists)
    bool GetLastWeaponTrailPoints(Points& points, const int& trail) const
    {
        points.Count = trail;
        points.Trail = (int)(&trail - TrailLengths);
        return trail != 0;
    }
    bool GetLastWeaponTrailPoints(Points& points, long trailIndex) const
    {
        if (trailIndex < NoTrails)
            return GetLastWeaponTrailPoints(points, trailIndex >= 0 ? TrailLengths[trailIndex] : TrailLengths[0]);
        return false;
    }
    void GetAllTrailsLastWeaponTrailPoints(Out& points) const
    {
        for (long i = 0; i < NoTrails; ++i) {
            Points trailPoints = { 0, -1 };
            if (GetLastWeaponTrailPoints(trailPoints, i))
                points.push_back(trailPoints);
        }
    }
};

int main()
{
    const int lengths[4] = { 3, 0, 5, 2 };
    WeaponTrail trail = { 4, lengths };
    Out out = { { { 0, 0 } }, 0 };
    trail.GetAllTrailsLastWeaponTrailPoints(out);       // the empty trail 1 is skipped
    if (out.Size != 3 || out.Items[0].Trail != 0 || out.Items[1].Trail != 2 || out.Items[2].Trail != 3 || out.Items[1].Count != 5) { std::printf("0076ea60_TEST FAIL\n"); return 1; }
    WeaponTrail none = { 0, lengths };
    Out out2 = { { { 0, 0 } }, 0 };
    none.GetAllTrailsLastWeaponTrailPoints(out2);
    if (out2.Size != 0) { std::printf("0076ea60_TEST FAIL\n"); return 1; }
    Points p = { 0, -1 };
    if (!trail.GetLastWeaponTrailPoints(p, -2L) || p.Trail != 0 || trail.GetLastWeaponTrailPoints(p, 4L)) { std::printf("0076ea60_TEST FAIL\n"); return 1; }
    std::printf("0076ea60_TEST PASS\n");
    return 0;
}