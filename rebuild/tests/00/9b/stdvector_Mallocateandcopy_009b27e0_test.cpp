#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#ifdef _MSC_VER
typedef unsigned int PlacementSize;
#else
typedef __SIZE_TYPE__ PlacementSize;
#endif
inline void* operator new(PlacementSize, void* where) { return where; }



namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {          // STLport pair: explicit copy constructor next to the converting one
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {   // the end-of-storage pointer over the empty allocator
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x009B27E0
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
    void reserve(size_type __n)
    {
        if (capacity() < __n) {
            const size_type __old_size = size();
            _Tp* __tmp;
            if (_M_start) {
                __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
                _M_clear();
            } else {
                __tmp = _M_end_of_storage.allocate(__n);
            }
            _M_set(__tmp, __tmp + __old_size, __tmp + __n);
        }
    }
};


}
namespace std {
template<class T,class Alloc> template<class Input>
T* vector<T,Alloc>::_M_allocate_and_copy(size_type n,Input first,Input last) {
 T* result=_M_end_of_storage.allocate(n);
 __uninitialized_copy(first,last,result,__false_type());
 return result;
}
}
template std::pair<unsigned long, unsigned long>* std::vector<std::pair<unsigned long, unsigned long> >::_M_allocate_and_copy<std::pair<unsigned long, unsigned long>*>(unsigned int,std::pair<unsigned long, unsigned long>*,std::pair<unsigned long, unsigned long>*);

typedef std::pair<unsigned long, unsigned long> Element;
int main() { std::vector<Element> v; Element src[3]={Element(1,37),Element(5,37),Element(9,37)};
 Element* result=v._M_allocate_and_copy(5,src,src+3);
 if(!result || result==src || result[0].first!=1 || result[1].first!=5 || result[2].first!=9 || result[0].second!=37 || result[2].second!=37) { puts("009b27e0_TEST FAIL"); return 1; }
 free(result);
 result=v._M_allocate_and_copy(0,src,src); if(result) { free(result); puts("009b27e0_TEST FAIL"); return 1; }
 result=v._M_allocate_and_copy(2,src,src); if(!result || src[0].first!=1 || src[2].first!=9) { puts("009b27e0_TEST FAIL"); return 1; } free(result);
 puts("009b27e0_TEST PASS"); return 0;
}