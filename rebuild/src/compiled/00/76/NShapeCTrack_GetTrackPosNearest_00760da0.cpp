// NShape::CTrack::GetTrackPosNearest(const C3DVector* pos, C3DVector* nearest, float* fraction) -- sample the track at steps of 0.1 length units and return the sampled position (and its length fraction) nearest to pos
// Leaf: GetTrackPosNearest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760DA0, length 195 bytes (`ret 0Ch`; 208 with padding).
// Callees: NShape::CTrack::GetTrackPosAtLengthFraction(float) @ 0x00760C40 (C3DVector by hidden pointer).
// Constants: 0x0122EE58 = 0.1f, 0x0122DED8 = 1.0f; the initial best distance 1e10f is an immediate.
// Layout: NShape::CTrack TrackLength +0x24 (as the CTrack constructor 0x00761F40).
// Shape: a do/while over the fraction (`(t += step) < 1.0f`, no entry test); the squared distance is written
// dx*dx + dy*dy + dz*dz (VC7.1 evaluates the right-hand squares first: z, y, x); `d < best` keeps the copy-out in the
// taken arm.
class C3DVector { public:
    float X, Y, Z;
};
namespace NShape {
class CTrack { public:
    virtual void v00();
    unsigned char _pad04[0x20];             // Owner, Points, SelectedPoints, TrackTraverser
    float         TrackLength;              // +0x24
    C3DVector GetTrackPosAtLengthFraction(float fraction);   // 0x00760C40
    void GetTrackPosNearest(const C3DVector* pos, C3DVector* nearest, float* fraction);
};
}

void NShape::CTrack::GetTrackPosNearest(const C3DVector* pos, C3DVector* nearest, float* fraction)
{
    float step = 0.1f / TrackLength;
    float best = 1e10f;
    float t = 0.0f;
    do {
        C3DVector p = GetTrackPosAtLengthFraction(t);
        float dx = p.X - pos->X;
        float dy = p.Y - pos->Y;
        float dz = p.Z - pos->Z;
        float d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            best = d;
            *nearest = p;
            *fraction = t;
        }
    } while ((t += step) < 1.0f);
}