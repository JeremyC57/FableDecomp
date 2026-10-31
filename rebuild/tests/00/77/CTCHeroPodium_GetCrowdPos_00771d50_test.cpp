// Behaviour test for CTCHeroPodium::GetCrowdPos (0x00771D50): false at eight occupants; otherwise the best vacant position (the origin, slot 0) is returned, slot 0 marked and the count bumped.
#include <cstdio>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
template <class T1, class T2> struct Pair {
    T1 first; T2 second;
    Pair(const T1& a, const T2& b) : first(a), second(b) {}
};
class CTCHeroPodium {
public:
    long NumberOfCrowdOccupants;
    long* CrowdOccupation;
    Pair<C3DVector, long> GetBestVacantPositionForThisPos(const C3DVector&) const
    {
        return Pair<C3DVector, long>(C3DVector(0.0f, 0.0f, 0.0f), 0);
    }
    bool GetCrowdPos(const C3DVector& pos, C3DVector& crowd_pos)
    {
        if (NumberOfCrowdOccupants == 8)
            return false;
        Pair<C3DVector, long> best = GetBestVacantPositionForThisPos(pos);
        crowd_pos = best.first;
        CrowdOccupation[best.second] = 1;
        ++NumberOfCrowdOccupants;
        return true;
    }
};
int main()
{
    long slots[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    CTCHeroPodium p;
    p.NumberOfCrowdOccupants = 3;
    p.CrowdOccupation = slots;
    C3DVector from(5.0f, 6.0f, 7.0f), out(9.0f, 9.0f, 9.0f);
    bool ok = p.GetCrowdPos(from, out);
    ok = ok && out.X == 0.0f && out.Y == 0.0f && out.Z == 0.0f && slots[0] == 1 && slots[1] == 0 && p.NumberOfCrowdOccupants == 4;
    p.NumberOfCrowdOccupants = 8;
    out = C3DVector(9.0f, 9.0f, 9.0f);
    if (p.GetCrowdPos(from, out) || out.X != 9.0f || p.NumberOfCrowdOccupants != 8) ok = false;
    if (!ok) { std::printf("00771d50_TEST FAIL\n"); return 1; }
    std::printf("00771d50_TEST PASS\n");
    return 0;
}