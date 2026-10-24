// Behaviour test for 0x0075F980 CCardinalSpline::AppendControlPoint: each appended point lands at the end
// (in place or through the growth path) and its index (size - 1) is returned.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

template <class _Tp> class CRetailVector {                  // STLport-shaped vector with a growth stand-in
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    int  m_overflows;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), m_overflows(0) { }
    ~CRetailVector() { for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp(); std::free(_M_start); }
    void _M_insert_overflow(_Tp* pos, const _Tp& x)
    {
        ++m_overflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? n * 2 : 1;
        _Tp* s = (_Tp*)std::malloc(cap * sizeof(_Tp));
        _Tp* d = s;
        for (_Tp* p = _M_start; p != pos; ++p, ++d) { new (d) _Tp(*p); p->~_Tp(); }
        new (d) _Tp(x); ++d;
        std::free(_M_start);
        _M_start = s; _M_finish = d; _M_end_of_storage = s + cap;
    }
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) _Tp(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};

struct C3DVector { float X, Y, Z; };

struct CCardinalSpline {
    CRetailVector<C3DVector> ControlPoints;
    long AppendControlPoint(const C3DVector& point)
    {
        ControlPoints.push_back(point);
        return (long)ControlPoints.size() - 1;
    }
};

int main()
{
    CCardinalSpline s;
    int ok = 1;
    for (int i = 0; i < 5; ++i) {
        C3DVector p = { (float)i, (float)(i * 2), (float)(i * 3) };
        if (s.AppendControlPoint(p) != i) ok = 0;
    }
    if (s.ControlPoints.size() != 5u || s.ControlPoints._M_start[3].Y != 6.0f || s.ControlPoints._M_start[4].Z != 12.0f) ok = 0;
    if (s.ControlPoints.m_overflows != 4) ok = 0;            // capacities 1, 2, 4, 8
    if (!ok) { std::printf("0075f980_TEST FAIL\n"); return 1; }
    std::printf("0075f980_TEST PASS\n");
    return 0;
}