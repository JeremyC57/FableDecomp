// behaviour test of std::vector<std::pair<long, CCharString> >::~vector() 0x004E34B2: the elements are destroyed through the out-of-line
// __destroy_aux (0x004DD782, modelled here) and the storage freed; an empty vector frees nothing. Self-contained (the lane links the test alone).
#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
static int g_destroyed = 0, g_freed = 0; static void* g_freedBlock = 0; static void* g_first = 0; static void* g_last = 0;
extern "C" void __cdecl free(void* block);
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();
};
CCharString::~CCharString() { ++g_destroyed; }
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) { ++g_freed; g_freedBlock = __p; free(__p); } }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    g_first = __first; g_last = __last;
    for ( ; __first != __last; ++__first) __first->second.~CCharString();
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
FABLE_FORCEINLINE void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> FABLE_FORCEINLINE void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void set(_Tp* s, _Tp* f, _Tp* e) { this->_M_start = s; this->_M_finish = f; this->_M_end_of_storage._M_data = e; }
};
}
typedef std::pair<long, CCharString> Item;
int main()
{
    int ok = 1;
    {
        Item* buf = (Item*)malloc(4 * sizeof(Item));
        std::vector<Item> v; v.set(buf, buf + 3, buf + 4);
        v.~vector();
        if (g_destroyed != 3 || g_freed != 1 || g_freedBlock != buf || g_first != buf || g_last != buf + 3) ok = 0;
        v.set(0, 0, 0);
    }
    {
        std::vector<Item> e; e.set(0, 0, 0);
        e.~vector();
        if (g_destroyed != 3 || g_freed != 1) ok = 0;
    }
    if (sizeof(Item) != 8) ok = 0;
    printf(ok ? "AUTO_TINY_004e34b2_TEST PASS\n" : "AUTO_TINY_004e34b2_TEST FAIL\n");
    return ok ? 0 : 1;
}
