// Behaviour test for 0x00A520B0: _STL::uninitialized_fill_n<C3DTriangle2*, unsigned int> (placement fill n triangles)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};
static C3DTriangle2 Tri(unsigned short a, unsigned short b, unsigned short c)
{
    C3DTriangle2 t;
    t.VertexIndex[0] = a; t.VertexIndex[1] = b; t.VertexIndex[2] = c;
    return t;
}
static bool IsTri(const C3DTriangle2& t, unsigned short a, unsigned short b, unsigned short c)
{
    return t.VertexIndex[0] == a && t.VertexIndex[1] == b && t.VertexIndex[2] == c;
}

namespace _STL {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}

int main()
{
    C3DTriangle2* raw = (C3DTriangle2*)malloc(6 * sizeof(C3DTriangle2));
    C3DTriangle2 v = Tri(3, 4, 5);
    C3DTriangle2* end = _STL::uninitialized_fill_n(raw, 6u, v);
    Check(end == raw + 6);
    for (int i = 0; i < 6; ++i)
        Check(IsTri(raw[i], 3, 4, 5));
    Check(_STL::uninitialized_fill_n(raw, 0u, Tri(9, 9, 9)) == raw && IsTri(raw[0], 3, 4, 5));
    free(raw);
    if (g_fail) {
        printf("00a520b0_TEST FAIL\n");
        return 1;
    }
    printf("00a520b0_TEST PASS\n");
    return 0;
}