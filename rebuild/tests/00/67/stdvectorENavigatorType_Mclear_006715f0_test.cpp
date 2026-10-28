// Test for 0x006715F0 vector<ENavigatorType>::_M_clear: the source object frees the block (null start: no free).
#include <stdio.h>
#include <stdlib.h>
enum ENavigatorType { NAVIGATOR_TYPE_0 };          // 4-byte enum

namespace std {
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
protected:
    void _M_clear();                                   // protected, as in the source (STLport 4.6)
public:
};
}
template <class V> struct ClearAccess : public V { using V::_M_clear; };   // the test reaches the protected member
int main()
{
    std::vector<ENavigatorType> v;
    v._M_start = (ENavigatorType*)malloc(3 * sizeof(ENavigatorType));
    v._M_finish = v._M_start + 2; v._M_end_of_storage._M_data = v._M_start + 3;
    ENavigatorType* old = v._M_start;
    static_cast<ClearAccess<std::vector<ENavigatorType> >&>(v)._M_clear();
    if (v._M_start != old) return 1;                   // _M_clear leaves the pointers to the caller
    std::vector<ENavigatorType> e; e._M_start = e._M_finish = e._M_end_of_storage._M_data = 0;
    static_cast<ClearAccess<std::vector<ENavigatorType> >&>(e)._M_clear();
    puts("006715f0_TEST PASS");
    return 0;
}
