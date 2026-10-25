// std::vector<list<C2DLineF> >::erase(list<C2DLineF>* first, list<C2DLineF>* last) -- remove a range of per-cell line lists: assign the tail down over the range list by list, destroy the left-over lists at the end, pull the finish pointer back
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84090, 78 bytes, thiscall `ret 8`, returns `first`.
// Identity: callers 0x00A84F7B and 0x00A85273 -- the latter is vector<list<C2DLineF> >::resize 0x00A85240 (the shrink arm,
//   from CNavQuadTree::InitialiseLines 0x00A7A5E0 on a layer's line lists). The 4-byte element is the STLport list
//   (sentinel pointer) whose operator= is 0x00517720. The catalog's "Vector_DestroyBackwardIterate" is descriptive.
// Callees: list::operator= 0x00517720, __destroy_aux(first, last, __false_type) 0x00A827D0 (/Gr `__fastcall`, `ret 4`).
// Shape: STLport `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;` --
//   copy() takes the __false_type arm into the inline random-access `__copy` loop (`for (n = last - first; n > 0; --n)
//   *result++ = *first++;`, the source addressed as result + (last - first)), then _Destroy's empty tag is homed in the
//   LIVE `first` argument slot (`first` is re-read from it for the return), which VC7.1 only does when the
//   tag-taking __destroy_aux has a visible body that never touches the tag: hence the same-TU noinline stand-in. With a
//   plain `inline` the operator=-calling __copy loop stays out of line (a `__cdecl` call), so it is `__forceinline`.
extern "C" void __cdecl free(void* block);              // stand-in body only

class C2DLineF;

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp, class _Alloc = int> class list {
public:
    _List_node_base* _M_node;                           // +0x00 sentinel
    list& operator=(const list& __x);                   // 0x00517720
    ~list();                                            // 0x005127A0
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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

template <class _Tp> inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

// Same-TU stand-in for __destroy_aux 0x00A827D0 (retail compiled it in this TU with the list destructor inline); it never
// touches the tag, which lets VC7.1 home the tag in the live `first` slot as retail.
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first) {
        _List_node_base* __head = __first->_M_node;
        _List_node_base* __cur = __head->_M_next;
        while (__cur != __head) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            free(__tmp);
        }
        __head->_M_next = __head;
        __head->_M_prev = __head;
        if (__head)
            free(__head);
    }
}
#pragma auto_inline(on)

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy((const _Tp*)__last, (const _Tp*)this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template list<C2DLineF>* vector<list<C2DLineF> >::erase(list<C2DLineF>* __first, list<C2DLineF>* __last);
}