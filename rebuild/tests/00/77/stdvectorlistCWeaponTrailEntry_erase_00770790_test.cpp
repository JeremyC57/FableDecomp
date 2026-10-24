// Behaviour test for 0x00770790 std::vector<list<CWeaponTrailEntry> >::erase(first, last): list::operator= copies the
// tail down over first, __destroy_aux tears down the now-unused trailing lists, finish is pulled back, first returned.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif

static int g_assigns = 0, g_clears = 0;
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };

template <class _Tp> class list {
public:
    _List_node_base* _M_node;
    int m_size;                                          // test-only payload standing in for the node chain
    list<_Tp>& operator=(const list<_Tp>& __x) { ++g_assigns; m_size = __x.m_size; return *this; }
    void clear() { ++g_clears; m_size = 0; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->clear();
}
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    _Trivial_destructor __tag;
    __destroy_aux(__first, __last, __tag);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (_ForwardIterator)0);
}
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

struct CWeaponTrailEntry { };

int main()
{
    typedef std::list<CWeaponTrailEntry> L;
    L a[5];
    for (int k = 0; k < 5; ++k) { a[k]._M_node = 0; a[k].m_size = 10 + k; }
    std::vector<L> v;
    v._M_start = a; v._M_finish = a + 5; v._M_end_of_storage._M_data = a + 5;
    int ok = 1;
    L* r = v.erase(a + 0, a + 2);                        // drop the first two lists
    if (r != a || v._M_finish != a + 3) ok = 0;
    if (g_assigns != 3 || g_clears != 2) ok = 0;
    if (a[0].m_size != 12 || a[1].m_size != 13 || a[2].m_size != 14 || a[3].m_size != 0 || a[4].m_size != 0) ok = 0;
    r = v.erase(a + 3, a + 3);                           // empty range at the end
    if (r != a + 3 || v._M_finish != a + 3 || g_assigns != 3 || g_clears != 2) ok = 0;
    printf(ok ? "00770790_TEST PASS\n" : "00770790_TEST FAIL\n");
    return ok ? 0 : 1;
}