// Test for 0x00414EA0 std::vector<CCharString>::~vector: every element destroyed, then the block freed (none when empty).
#include <stdio.h>
static int gFrees = 0; static void* gFreed = 0;
static void free(void* block) { ++gFrees; gFreed = block; }
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
static int gDtors = 0;
class CCharStringData;
class CCharString {
public:
    ~CCharString() { ++gDtors; }
    CCharStringData* PStringData;
};
int main()
{
    bool ok = true;
    CCharString block[3];
    {
        std::vector<CCharString>* v = (std::vector<CCharString>*)new char[sizeof(std::vector<CCharString>)];
        v->_M_start = block; v->_M_finish = block + 3; v->_M_end_of_storage._M_data = block + 3;
        int before = gDtors;
        v->~vector();
        ok = ok && gDtors - before == 3 && gFrees == 1 && gFreed == block;
        v->_M_start = 0; v->_M_finish = 0; v->_M_end_of_storage._M_data = 0;
        before = gDtors;
        v->~vector();
        ok = ok && gDtors == before && gFrees == 1;
        delete[] (char*)v;
    }
    if (!ok) { printf("00414ea0_TEST FAIL\n"); return 1; }
    printf("00414ea0_TEST PASS\n");
    return 0;
}
