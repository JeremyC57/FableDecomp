// Behaviour test for 0x00674030 std::vector<pair<EExpressionType, CExpressionSet::CEntry> >::_M_allocate_and_copy: a block of n
// entries comes from malloc (none for n == 0), [first, last) is copy-constructed into its front, and the block is returned.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif

enum EExpressionType { EXPRESSION_TYPE_NONE = -1, EXPRESSION_TYPE_0 = 0 };
struct Second { EExpressionType Type; int AnimIndex; };

namespace nav {
struct __false_type { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    struct : _Alloc { _Tp* _M_data; } _M_end_of_storage;
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}

typedef nav::pair<EExpressionType, Second> Entry;

int main()
{
    Entry src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first = (EExpressionType)(i * 10 - 1);
    }
    for (int i = 0; i < 3; ++i) { *(int*)&src[i].second.Type = 100 + i; src[i].second.AnimIndex = 200 + i; }
    nav::vector<Entry> v;
    Entry* block = v._M_allocate_and_copy(5u, (const Entry*)src, (const Entry*)(src + 3));
    if (block == 0) { std::printf("00674030_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if ((int)block[i].first != i * 10 - 1 || *(int*)&block[i].second.Type != 100 + i || (int)block[i].second.AnimIndex != 200 + i) {
            std::printf("00674030_TEST FAIL\n"); return 1;
        }
    std::free(block);
    if (v._M_allocate_and_copy(0u, (const Entry*)src, (const Entry*)src) != 0) { std::printf("00674030_TEST FAIL\n"); return 1; }
    std::printf("00674030_TEST PASS\n");
    return 0;
}