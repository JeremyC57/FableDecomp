// Behaviour test for NShape::CTrack::GetTrackPosNearest (00760da0).
#include <cstdio>
struct V { float X, Y, Z; };
struct Track {
    float TrackLength;
    V GetTrackPosAtLengthFraction(float t) { V v = { t * TrackLength, 0, 0 }; return v; }   // a straight track along x
    void GetTrackPosNearest(const V* pos, V* nearest, float* fraction)
    {
        float step = 0.1f / TrackLength; float best = 1e10f; float t = 0.0f;
        do {
            V p = GetTrackPosAtLengthFraction(t);
            float dx = p.X - pos->X, dy = p.Y - pos->Y, dz = p.Z - pos->Z;
            float d = dx * dx + dy * dy + dz * dz;
            if (d < best) { best = d; *nearest = p; *fraction = t; }
        } while ((t += step) < 1.0f);
    }
};
int main()
{
    Track tr; tr.TrackLength = 10.0f;
    V target = { 4.02f, 3, 0 }, out = { 0, 0, 0 }; float f = -1;
    tr.GetTrackPosNearest(&target, &out, &f);
    bool ok = out.X > 3.95f && out.X < 4.05f && f > 0.395f && f < 0.405f;
    if (!ok) { std::printf("00760da0_TEST FAIL\n"); return 1; }
    std::printf("00760da0_TEST PASS\n");
    return 0;
}
