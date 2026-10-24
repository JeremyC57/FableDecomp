// Behaviour test for 0x00A54580: _STL::vector<C3DKeyframe>::vector(unsigned int n) (allocate n keyframes and fill them with a default one)
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

static int g_quatCtor = 0;
class CQuaternion {
public:
    float X, Y, Z, W;
    CQuaternion() { ++g_quatCtor; }
};
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
};
class C3DKeyframe {
public:
    CQuaternion Rotation;
    C4DVector   Position;
    C4DVector   Scaling;
};
namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};
}

int main()
{
    {
        _STL::vector<C3DKeyframe> v(5u);
        Check(v._M_start != 0 && v._M_finish == v._M_start + 5 && v._M_end_of_storage._M_data == v._M_start + 5);
        Check(g_quatCtor == 1);                      // one default keyframe, copied five times
        free(v._M_start);
    }
    {
        _STL::vector<C3DKeyframe> e(0u);
        Check(e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage._M_data == 0);
    }
    Check(sizeof(C3DKeyframe) == 48u);
    if (g_fail) {
        printf("00a54580_TEST FAIL\n");
        return 1;
    }
    printf("00a54580_TEST PASS\n");
    return 0;
}