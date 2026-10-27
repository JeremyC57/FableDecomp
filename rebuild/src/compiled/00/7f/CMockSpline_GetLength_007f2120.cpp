// CMockSpline::GetLength() const -- the length of the polyline through the control points (0 for an empty spline): the sum of the distances between consecutive points
// Leaf: ?GetLength@CMockSpline@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F2120, 159 bytes, thiscall, plain `ret`; returns the float in st(0).
// Identity (proven): Ego_r.pdb CMockSpline is { vector<C3DVector> ControlPoints } with two methods, GetLength() const (float,
//   no arguments) and GetPositionAtIndex(...) const; this no-argument float body walks the 12-byte points of the vector
//   at this+0 and is called by the lightning TU's 0x007F21C0, which takes a float, clamps it to [0, 1] and scales it by
//   this length (the GetPositionAtIndex side). Neighbours are CTCLightningNode / CTCLightningOrb.
// Callees: none. Constant 0x0122DEDC = 0.0f.
// Shape: the first point copied to a running `previous` local (three dwords at [esp+4]); each later point's difference
//   from it, then the copy, then the sqrt of the squared length added in. The loop runs `i != size()` (retail `cmp ecx,1 ;
//   je`; `<` gives jbe) and C3DVector's memberwise user copy constructor copies straight from the begin pointer (the
//   implicit copy goes through `mov eax,esi`).
#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    const _Tp& operator[](size_type __n) const { return *(this->_M_start + __n); }
};
}

class CMockSpline {
public:
    std::vector<C3DVector> ControlPoints;               // +0x00
    float GetLength() const;
};

float CMockSpline::GetLength() const
{
    float length = 0.0f;
    if (ControlPoints.size() == 0)
        return 0.0f;
    C3DVector previous = ControlPoints[0];
    for (unsigned int i = 1; i != ControlPoints.size(); ++i) {
        C3DVector delta = ControlPoints[i] - previous;
        previous = ControlPoints[i];
        length += delta.GetLength();
    }
    return length;
}