// Behaviour test for 0x007F2120 CMockSpline::GetLength: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
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

int main()
{
    CMockSpline* s = (CMockSpline*)calloc(1, sizeof(CMockSpline));
    int ok = s->GetLength() == 0.0f;
    C3DVector* p = (C3DVector*)malloc(4 * sizeof(C3DVector));
    p[0].X = 0; p[0].Y = 0; p[0].Z = 0;
    p[1].X = 3; p[1].Y = 4; p[1].Z = 0;       // 5
    p[2].X = 3; p[2].Y = 4; p[2].Z = 12;      // 12
    p[3].X = 1; p[3].Y = 2; p[3].Z = 1;       // sqrt(4 + 4 + 121) = sqrt(129)
    s->ControlPoints._M_start = p; s->ControlPoints._M_finish = p + 1;
    ok &= s->GetLength() == 0.0f;
    s->ControlPoints._M_finish = p + 3;
    ok &= s->GetLength() == 17.0f;
    s->ControlPoints._M_finish = p + 4;
    float l = s->GetLength(); float want = 17.0f + (float)sqrt(129.0);
    ok &= l > want - 1e-4f && l < want + 1e-4f;
    if (ok) printf("007f2120_TEST PASS\n");
    return ok ? 0 : 1;
}