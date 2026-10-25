// Behaviour test for 0x00A56AA0 (source body copied from src/00a56aa0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

namespace _STL {

struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V v) : A(a), _M_data(v) { }
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<T*, Alloc> _M_end_of_storage;     // +0x08
    _Vector_base(const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T, Alloc>(a) { }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend);   // 0x0066E370
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace _STL

template <class T> class CArray : public _STL::vector<T> {
public:
    CArray() { }
};

class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                    // +0x00
    float             Tension;                          // +0x0c
    bool              EndPointsConnected;               // +0x10
    CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected);
};

CCardinalSpline::CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected)
    : Tension(tension), EndPointsConnected(endPointsConnected)
{
    ControlPoints.push_back(start);
    ControlPoints.push_back(end);
}

// ---- test harness ----
static int g_overflow = 0;
namespace _STL {
template <> void vector<C3DVector, allocator<C3DVector> >::_M_insert_overflow(C3DVector* position, const C3DVector& x, const __false_type&, size_type fill_len, bool atend)
{
    ++g_overflow;
    unsigned int n = (unsigned int)(position - this->_M_start);
    C3DVector* p = (C3DVector*)malloc((n + fill_len + 4) * sizeof(C3DVector));
    if (n) memcpy(p, this->_M_start, n * sizeof(C3DVector));
    for (size_type i = 0; i < fill_len; ++i) new (p + n + i) C3DVector(x);
    this->_M_start = p;
    this->_M_finish = p + n + fill_len;
    this->_M_end_of_storage._M_data = atend ? this->_M_finish + 4 : this->_M_finish;
}
}
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    unsigned char raw[2 * sizeof(C3DVector)];
    C3DVector& a = *(C3DVector*)raw;
    C3DVector& b = *(C3DVector*)(raw + sizeof(C3DVector));
    a.X = 1.0f; a.Y = 2.0f; a.Z = 3.0f;
    b.X = -4.0f; b.Y = 5.5f; b.Z = 6.25f;
    CCardinalSpline s(a, b, 0.75f, true);
    if (!Near(s.Tension, 0.75f) || !s.EndPointsConnected || g_overflow != 1) ok = 0;
    if (s.ControlPoints._M_finish - s.ControlPoints._M_start != 2) ok = 0;
    C3DVector* p = s.ControlPoints._M_start;
    if (!Near(p[0].X, 1.0f) || !Near(p[0].Y, 2.0f) || !Near(p[0].Z, 3.0f)) ok = 0;
    if (!Near(p[1].X, -4.0f) || !Near(p[1].Y, 5.5f) || !Near(p[1].Z, 6.25f)) ok = 0;
    if (sizeof(C3DVector) != 12 || (sizeof(void*) == 4 && sizeof(CCardinalSpline) != 20)) ok = 0;
    printf("00a56aa0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}