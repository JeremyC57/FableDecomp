// NShape::CTrack::GetMinDistanceToTrack(const C3DVector* pos, float* distance) -- smallest distance from pos (flattened to z = 0) to the track: per segment, the perpendicular xy distance when pos lies inside the segment's xy box, else the nearer end-point distance; false when the track has fewer than two points
// Grade: DIFFER(606v603) -- body shape right (box tests, perpendicular dot, end-point min); retail keeps pos in ebx and puts best in the dead pos slot (0x44 frame), ours copies cur through ebx/ebp and needs a 0x48 frame
// Leaf: GetMinDistanceToTrack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760E70, length 603 bytes (`ret 8`).
// Callees: C3DVector::Normalise 0x00A14440 (in place). Constants: 0x0122DEDC = 0.0f; the initial best 1e10f is an immediate.
// Layout: NShape::CTrack Points (std::vector<C3DVector>, STLport) at +0x08 (as GetTrackPosNearest 0x00760DA0 / ctor 0x00761F40).
#include <math.h>
#pragma intrinsic(sqrt, fabs)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                                       // 0x00A14440
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
template <class T> inline const T& Min2(const T& a, const T& b) { return b < a ? b : a; }
template <class T> inline const T& Max2(const T& a, const T& b) { return a < b ? b : a; }
struct CPointVector {                                       // std::vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
namespace NShape {
struct CTrack {
    virtual void v00();
    void*        Owner;                                     // +0x04
    CPointVector Points;                                    // +0x08
    bool GetMinDistanceToTrack(const C3DVector* pos, float* distance);
};
}

bool NShape::CTrack::GetMinDistanceToTrack(const C3DVector* pos, float* distance)
{
    if (Points.size() > 1) {
        C3DVector flat(pos->X, pos->Y, 0.0f);
        float best = 1e10f;
        C3DVector* it = Points.begin();
        C3DVector prev = *it;
        for (++it; it != Points.end(); ++it) {
            C3DVector cur = *it;
            float minX = prev.X;
            float minY = prev.Y;
            float maxX = prev.X;
            float maxY = prev.Y;
            if (cur.X < prev.X)
                minX = cur.X;
            if (cur.Y < prev.Y)
                minY = cur.Y;
            if (cur.X > prev.X)
                maxX = cur.X;
            if (cur.Y > prev.Y)
                maxY = cur.Y;
            float d;
            if (flat.X >= minX && flat.X < maxX && flat.Y >= minY && flat.Y < maxY) {
                C3DVector dir(cur.X - prev.X, cur.Y - prev.Y, 0.0f);
                dir.Normalise();
                C3DVector perp(dir.Y, -dir.X, 0.0f);
                d = fabs(Dot(perp, *pos - prev));
            } else {
                float d1 = (prev - flat).GetLength();
                float d2 = (cur - flat).GetLength();
                d = Min2(d1, d2);
            }
            if (d <= best)
                best = d;
            prev = cur;
        }
        *distance = best;
        return true;
    }
    return false;
}
