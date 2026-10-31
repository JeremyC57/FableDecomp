// Behaviour test for NShape::CTrack::GetTrackPosAtLengthFraction (0x00760C40): same logic as src/00760c40.cpp over a
// plain point array.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V Sub(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct Track {
    const V* first; const V* last; float TrackLength;
    V GetTrackPosAtLengthFraction(float fraction) const
    {
        const V* it = first;
        V start = *it;
        float covered = 0.0f;
        V prev = *it;
        for (++it; it != last; ) {
            const V& point = *it++;
            V delta = Sub(point, prev);
            float segment = (float)std::sqrt(delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z);
            float reached = covered + segment;
            if (reached / TrackLength < fraction) {
                covered = reached;
                prev = point;
            } else {
                float t = fraction * TrackLength - covered;
                float inv = 1.0f / segment;
                return V(prev.X + delta.X * t * inv, prev.Y + delta.Y * t * inv, prev.Z + delta.Z * t * inv);
            }
        }
        return start;
    }
};
static bool Near(const V& v, float x, float y, float z)
{
    return std::fabs(v.X - x) < 1e-4f && std::fabs(v.Y - y) < 1e-4f && std::fabs(v.Z - z) < 1e-4f;
}
int main()
{
    V pts[3] = { V(0, 0, 0), V(10, 0, 0), V(10, 10, 0) };
    Track t; t.first = pts; t.last = pts + 3; t.TrackLength = 20.0f;
    if (!Near(t.GetTrackPosAtLengthFraction(0.25f), 5, 0, 0)) { std::printf("00760c40_TEST FAIL\n"); return 1; }
    if (!Near(t.GetTrackPosAtLengthFraction(0.75f), 10, 5, 0)) { std::printf("00760c40_TEST FAIL\n"); return 1; }
    if (!Near(t.GetTrackPosAtLengthFraction(0.0f), 0, 0, 0)) { std::printf("00760c40_TEST FAIL\n"); return 1; }
    if (!Near(t.GetTrackPosAtLengthFraction(2.0f), 0, 0, 0)) { std::printf("00760c40_TEST FAIL\n"); return 1; }   // past the end: first point
    Track one; one.first = pts; one.last = pts + 1; one.TrackLength = 1.0f;
    if (!Near(one.GetTrackPosAtLengthFraction(0.5f), 0, 0, 0)) { std::printf("00760c40_TEST FAIL\n"); return 1; }
    std::printf("00760c40_TEST PASS\n");
    return 0;
}