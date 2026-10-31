// NShape::CTrack::GetTrackPosAtLengthFraction(float fraction) const -- walk the track's polyline accumulating segment lengths; on the segment where the covered length reaches fraction * TrackLength return the interpolated point, otherwise (past the end) the first point
// Leaf: ?GetTrackPosAtLengthFraction@CTrack@NShape@@QBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760C40, length 342 bytes (`ret 8`, C3DVector by hidden pointer). No callees.
//   Ego_r.pdb: ?GetTrackPosAtLengthFraction@CTrack@NShape@@QBE?AVC3DVector@@M@Z. Constant 0x0122DED8 = 1.0f.
// Layout: NShape::CTrack : NShape::CShape (Ego_r.pdb; both class-keyed V). CShape {vptr, Owner +0x04, Points +0x08,
//   SelectedPoints +0x14} with retail STLport 12-byte vectors (Ego_r +0x08 / +0x18), CTrack TrackTraverser +0x20 and
//   TrackLength +0x24 (Ego_r +0x28 / +0x2C), as the landed CShape::GetBounds 0x00760BC0 / CTrack::GetTrackPosNearest 0x00760DA0.
// Shape: the first point and the previous point are named C3DVector copies through a user copy constructor (memberwise
//   float copies: retail homes all six words, keeps first.Y on the x87 stack across the call-free loop and copies the
//   rest as integers); `covered` is declared between them (its zero store sits between the prev.Y and prev.Z stores).
//   The distance is the inline Length() (fsqrt intrinsic); `reached / TrackLength` becomes a hoisted reciprocal. The
//   inline operator/ rescales its own parameter (`f = 1.0f / f`), which homes the reciprocal in the dead `fraction`
//   slot and multiplies from memory (a separate `inv` local stays on the x87 stack).
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float f) const { f = 1.0f / f; return C3DVector(X * f, Y * f, Z * f); }
    float Length() const { return sqrt(X * X + Y * Y + Z * Z); }
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {     // STLport 4.6: {start, finish, end_of_storage}
public:
    typedef const T* const_iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
class CTCShapeManager;
namespace NShape {
class CShape {
public:
    virtual ~CShape();
    CTCShapeManager*         Owner;                 // +0x04
    std::vector<C3DVector>   Points;                // +0x08
    std::vector<C3DVector*>  SelectedPoints;        // +0x14 (vector<vector<C3DVector>::iterator>)
};
class CTrack : public CShape {
public:
    long  TrackTraverser;                           // +0x20
    float TrackLength;                              // +0x24
    C3DVector GetTrackPosAtLengthFraction(float fraction) const;
};
}

C3DVector NShape::CTrack::GetTrackPosAtLengthFraction(float fraction) const
{
    std::vector<C3DVector>::const_iterator it = Points.begin();
    C3DVector first = *it;
    float covered = 0.0f;
    C3DVector prev = *it;
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
    return first;
}