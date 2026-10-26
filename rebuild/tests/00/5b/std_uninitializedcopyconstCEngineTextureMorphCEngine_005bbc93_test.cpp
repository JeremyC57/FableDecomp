// Behaviour test for 0x005BBC93: the STLport 4.6 template body as in the source, over a real CEngineTextureMorph
// (two longs + a vector of blend descs) with a counting allocator.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int g_allocs = 0, g_copies = 0;
static unsigned int g_lastBytes = 0;
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { ++g_allocs; g_lastBytes = __n * sizeof(_Tp); return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
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
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __value) { new (__p) _T1(__value); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(unsigned int n) : _Vector_base<_Tp, _Alloc>(n, _Alloc()) { this->_M_finish = this->_M_start + n; if (n) memset(this->_M_start, 0, n * sizeof(_Tp)); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};
}
class CCompositeBlendDesc { public: long a, b; };
class CEngineTextureMorph {
public:
    long OriginalTextureIndex;
    long MorphID;
    std::vector<CCompositeBlendDesc> Blends;
    CEngineTextureMorph(long o, long m, unsigned int nb) : OriginalTextureIndex(o), MorphID(m), Blends(nb) { }
    CEngineTextureMorph(const CEngineTextureMorph& other) : OriginalTextureIndex(other.OriginalTextureIndex), MorphID(other.MorphID), Blends(other.Blends) { ++g_copies; }
};
int main()
{
    CEngineTextureMorph src[3] = { CEngineTextureMorph(1, 10, 2), CEngineTextureMorph(2, 20, 0), CEngineTextureMorph(3, 30, 1) };
    src[0].Blends._M_start[1].a = 77;
    g_copies = 0;
    unsigned char raw[3 * sizeof(CEngineTextureMorph)];
    CEngineTextureMorph* dst = (CEngineTextureMorph*)raw;
    CEngineTextureMorph* end = std::__uninitialized_copy((const CEngineTextureMorph*)src, (const CEngineTextureMorph*)(src + 3), dst, std::__false_type());
    bool ok = end == dst + 3 && g_copies == 3 && dst[2].MorphID == 30 && dst[0].Blends.size() == 2 && dst[0].Blends._M_start[1].a == 77
        && dst[0].Blends._M_start != src[0].Blends._M_start && dst[1].Blends.size() == 0;
    ok = ok && std::__uninitialized_copy((const CEngineTextureMorph*)src, (const CEngineTextureMorph*)src, dst, std::__false_type()) == dst;
    if (!ok) { printf("005bbc93_TEST FAIL\n"); return 1; }
    printf("005bbc93_TEST PASS\n");
    return 0;
}
