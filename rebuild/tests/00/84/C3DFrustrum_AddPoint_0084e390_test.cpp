// Behaviour test for 0x0084E390 C3DFrustrum::AddPoint(point, weight): push_back of (point, weight) onto Points, in place
// while there is room, else through _M_insert_overflow (old size + max(old size, 1)).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
#include <new>
#include <stdlib.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise (landed 0x0084F4D0 model)
};

struct __true_type { __true_type() { } };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct _STLP_alloc_proxy { T _M_data; };

template <class T> class vector {                        // STLport vector: start / finish / end of storage
public:
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x0084F5E0
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*> _M_end_of_storage;
};

template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    unsigned int old_size = _M_finish - _M_start;
    unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != pos; ++p, ++new_finish)
        new (new_finish) T(*p);
    for (unsigned int i = 0; i < fill_len; ++i, ++new_finish)
        new (new_finish) T(x);
    if (!atend)
        for (T* p = pos; p != _M_finish; ++p, ++new_finish)
            new (new_finish) T(*p);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage._M_data = new_start + len;
}
}

namespace {
class C3DFrustrum {
public:
    void AddPoint(const C3DVector& point, float weight);
    C3DVector Apex;                                      // +0x00
    C3DVector Up;                                        // +0x0c
    std::vector<std::pair<C3DVector, float> > Points;    // +0x18
    float     FOV;                                       // +0x24
    C3DVector Focus;                                     // +0x28
};

void C3DFrustrum::AddPoint(const C3DVector& point, float weight)
{
    Points.push_back(std::pair<C3DVector, float>(point, weight));
}
}

void (C3DFrustrum::*g_keepAddPoint)(const C3DVector&, float) = &C3DFrustrum::AddPoint;   // keeps the internal-linkage body emitted; not part of the retail function


static C3DVector* MakeVec(float x, float y, float z)
{
    C3DVector* v = (C3DVector*)malloc(sizeof(C3DVector)); v->X = x; v->Y = y; v->Z = z; return v;
}
int main()
{
    C3DFrustrum* f = (C3DFrustrum*)malloc(sizeof(C3DFrustrum)); memset(f, 0, sizeof(*f));
    int ok = 1;
    for (int k = 0; k < 5; ++k)
        f->AddPoint(*MakeVec((float)k, (float)(k * 2), (float)(k * 3)), 0.5f * k);
    std::vector<std::pair<C3DVector, float> >& pts = f->Points;
    if (pts._M_finish - pts._M_start != 5) ok = 0;
    if (pts._M_end_of_storage._M_data - pts._M_start != 8) ok = 0;      // 1, 2, 4, 8
    for (int k = 0; k < 5 && ok; ++k) {
        const std::pair<C3DVector, float>& p = pts._M_start[k];
        if (p.first.X != (float)k || p.first.Y != (float)(k * 2) || p.first.Z != (float)(k * 3)) ok = 0;
        if (p.second < 0.5f * k - 0.0001f || p.second > 0.5f * k + 0.0001f) ok = 0;
    }
    printf(ok ? "0084e390_TEST PASS\n" : "0084e390_TEST FAIL\n");
    return ok ? 0 : 1;
}