// C3DFrustrum::UpdateFrustrum() -- refit the camera's melee-tracking frustrum: the focus is the weighted mean of the tracked points (integer weight total), and the field of view is the turn fraction of atan(widest perpendicular offset / apex-to-focus distance), the offsets measured along (focus - apex) x up
// Leaf: ?UpdateFrustrum@C3DFrustrum@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D0E0, 393 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb ?UpdateFrustrum@C3DFrustrum@?A0x49b2e1a3@@QAEXXZ (the NCameraModes::CBase TU's anonymous namespace);
//   callers 0x0084E416 (C3DFrustrum::AddPoint 0x0084E390) and 0x0084E6CF / 0x0084E7B1 (NCameraModes::CBase::
//   UpdateTrackMeleeOpponents 0x0084E590). The catalog's "CThingCreatureBase_ComputeMeleeMetrics" is descriptive.
// Callees: _ftol 0x00BFEA70 (the long weight total); sqrt / atan inline (fsqrt, `fld1 ; fpatan`).
// Data: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1/(2pi) 0x01237F08.
// Layout: Ego_r.pdb C3DFrustrum (56 bytes, retail 52 with STLport's 12-byte vector): Apex +0x00, Up +0x0c,
//   Points vector<pair<C3DVector,float> > +0x18 (16-byte elements), FOV +0x24, Focus +0x28.
// Shape: the weighted sum accumulates `pos * weight` temporaries on the x87 stack (sum.Z homed in a frame word); the
//   weight total is a long (`fild ; fadd ; _ftol` per point); Focus = sum / total through an inline operator/ that
//   forms its own reciprocal (a `sum * (1.0f / total)` multiply pops the reciprocal instead of keeping it: `fstp ;
//   fxch` v retail `fst ; fmul st(2)`); the side
//   axis is the cross product toFocus ^ Up; each point's squared offset is a named float compared `>` the running
//   maximum; FOV = atan((float)sqrt(max / |toFocus|^2)) * 1/(2pi) (without the float cast fld1/fpatan run before
//   the dead x87 slots are popped).
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl atan(double);
#pragma intrinsic(sqrt, atan)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }   // dot product
    C3DVector operator^(const C3DVector& o) const                                       // cross product
    {
        return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X);
    }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> class vector {                       // STLport: 12 bytes
public:
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

namespace {
class C3DFrustrum {
public:
    C3DVector                                       Apex;     // +0x00
    C3DVector                                       Up;       // +0x0c
    std::vector<std::pair<C3DVector, float> >       Points;   // +0x18
    float                                           FOV;      // +0x24
    C3DVector                                       Focus;    // +0x28
    void UpdateFrustrum();
};

void C3DFrustrum::UpdateFrustrum()
{
    C3DVector sum(0.0f, 0.0f, 0.0f);
    long totalWeight = 0;
    std::vector<std::pair<C3DVector, float> >::iterator it;
    for (it = Points.begin(); it != Points.end(); ++it) {
        sum += it->first * it->second;
        totalWeight += it->second;
    }
    Focus = sum / totalWeight;
    C3DVector toFocus = Focus - Apex;
    C3DVector side = toFocus ^ Up;
    float maxDistanceSquared = 0.0f;
    for (it = Points.begin(); it != Points.end(); ++it) {
        float distance = (it->first - Apex) * side;
        float distanceSquared = distance * distance;
        if (distanceSquared > maxDistanceSquared)
            maxDistanceSquared = distanceSquared;
    }
    FOV = (float)atan((float)sqrt(maxDistanceSquared / toFocus.GetLengthSquared())) * 0.15915493667125702f;
}
}

void (C3DFrustrum::*g_keepUpdateFrustrum)() = &C3DFrustrum::UpdateFrustrum;   // keeps the body emitted