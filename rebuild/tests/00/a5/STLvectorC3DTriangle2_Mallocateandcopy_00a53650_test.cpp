// Behaviour test for 0x00A53650: _STL::vector<C3DTriangle2>::_M_allocate_and_copy (malloc n triangles, copy a range in, return the block)
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
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}

int main()
{
    C3DTriangle2 src[3];
    src[0] = Tri(1, 2, 3); src[1] = Tri(4, 5, 6); src[2] = Tri(7, 8, 9);
    _STL::vector<C3DTriangle2> v;
    C3DTriangle2* block = v._M_allocate_and_copy(5u, src, src + 3);
    Check(block != 0 && block != src);
    Check(IsTri(block[0], 1, 2, 3) && IsTri(block[1], 4, 5, 6) && IsTri(block[2], 7, 8, 9));
    free(block);
    Check(v._M_allocate_and_copy(0u, src, src) == 0);
    if (g_fail) {
        printf("00a53650_TEST FAIL\n");
        return 1;
    }
    printf("00a53650_TEST PASS\n");
    return 0;
}