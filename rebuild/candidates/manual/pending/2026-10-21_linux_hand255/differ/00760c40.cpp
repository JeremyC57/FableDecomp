// Grade: DIFFER(343v342) -- control flow, FPU sequence and loop shape match; frame 0x38 v 0x30 (retail overlaps the found-arm delta*remaining temps with the dead first-point z slot), first-point copy goes through esi instead of eax, reciprocal kept in st instead of stored to the dead fraction slot.
// NShape::CTrack::GetTrackPosAtLengthFraction(float fraction) const -- walk the track's polyline accumulating segment lengths; on the segment where the covered length reaches fraction * TrackLength return the interpolated point, otherwise (past the end) the first point
// Leaf: GetTrackPosAtLengthFraction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760C40, length 342 bytes (`ret 8`, C3DVector by hidden pointer).
//   Ego_r.pdb: ?GetTrackPosAtLengthFraction@CTrack@NShape@@QBE?AVC3DVector@@M@Z. No callees.
// Constants: 0x0122DED8 = 1.0f.
// Layout: NShape::CShape points vector (first/last/end) at +0x08 (as landed 0x00760BC0), NShape::CTrack TrackLength
//   +0x24 (as landed 0x00761F40 / 0x00760DA0; Ego_r.pdb +0x2c).
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float f) const { float inv = 1.0f / f; return C3DVector(X * inv, Y * inv, Z * inv); }
    float Length() const { return sqrt(X * X + Y * Y + Z * Z); }
};
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() const { return m_pFirst; }
    T* end() const { return m_pLast; }
};
struct CTCShapeManager;
namespace NShape {
struct CTrack {
    virtual void v00();
    CTCShapeManager*          Owner;                // +0x04
    CRetailVector<C3DVector>  Points;               // +0x08
    CRetailVector<C3DVector*> SelectedPoints;       // +0x14
    long                      TrackTraverser;       // +0x20
    float                     TrackLength;          // +0x24
    C3DVector GetTrackPosAtLengthFraction(float fraction) const;
};
}

C3DVector NShape::CTrack::GetTrackPosAtLengthFraction(float fraction) const
{
    const C3DVector* it = Points.begin();
    float x = it->X;
    float y = it->Y;
    float z = it->Z;
    C3DVector prev = *it;
    float covered = 0.0f;
    for (++it; it != Points.end(); ) {
        const C3DVector& point = *it++;
        C3DVector delta = point - prev;
        float segment = delta.Length();
        float reached = covered + segment;
        if (reached / TrackLength < fraction) {
            covered = reached;
            prev = point;
        } else {
            return prev + (delta * (fraction * TrackLength - covered)) / segment;
        }
    }
    return C3DVector(x, y, z);
}
