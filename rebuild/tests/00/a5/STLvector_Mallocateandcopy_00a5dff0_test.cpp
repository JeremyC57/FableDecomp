// Behaviour test for 0x00A5DFF0 std::vector<_D3DVERTEXELEMENT9>::_M_allocate_and_copy(n, first, last)
#include <cstdio>
#include <cstdlib>
#include <new>

struct _D3DVERTEXELEMENT9 {
    unsigned short Stream;
    unsigned short Offset;
    unsigned char  Type;
    unsigned char  Method;
    unsigned char  Usage;
    unsigned char  UsageIndex;
};

namespace std {
struct __false_type { __false_type() {} };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    allocator<_Tp> _M_end_of_storage;
    _Tp* _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}   // namespace std

int main()
{
    _D3DVERTEXELEMENT9 src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Stream = (unsigned short)i;
        src[i].Offset = (unsigned short)(i * 12);
        src[i].Type = 2;
        src[i].Method = 0;
        src[i].Usage = (unsigned char)(i + 3);
        src[i].UsageIndex = 1;
    }
    std::vector<_D3DVERTEXELEMENT9> v;
    _D3DVERTEXELEMENT9* p = v._M_allocate_and_copy(4, src, src + 3);     // spare capacity left uninitialised
    bool ok = p != 0;
    for (int i = 0; ok && i < 3; ++i)
        ok = p[i].Stream == i && p[i].Offset == i * 12 && p[i].Type == 2 && p[i].Usage == i + 3 && p[i].UsageIndex == 1;
    std::free(p);
    ok = ok && v._M_allocate_and_copy(0, src, src) == 0;                  // n == 0 allocates nothing
    if (!ok) { std::printf("00a5dff0_TEST FAIL\n"); return 1; }
    std::printf("00a5dff0_TEST PASS\n");
    return 0;
}