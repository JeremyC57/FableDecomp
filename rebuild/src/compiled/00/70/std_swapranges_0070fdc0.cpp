// std::swap_ranges(list<CAnimSet>::const_iterator* first1, list<CAnimSet>::const_iterator* last1, list<CAnimSet>::const_iterator* first2) -- exchange two equal-length runs element by element and return the end of the second
// Leaf: ??$swap_ranges@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070FDC0, length 39 bytes. __fastcall (first1 ecx, last1 edx), `ret 4`.
// Callees: none.
// Identity: the manifest label "_Copy_impl<CRGBColour*,CRGBColour*>" is wrong (a swap, not a copy). Single caller: the
//   STLport random-access __rotate 0x007103C0 (call site 0x00710408, its `__k == __l` case) of the stable_sort chain.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `for (; first1 != last1; ++first1, ++first2) iter_swap(first1, first2);` with __iter_swap's
//   `_Tp tmp = *i1; *i1 = *i2; *i2 = tmp;` inlined (both loads before the two stores).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CAnimSet {                                        // retail layout (Ego_r.pdb minus 8: 12-byte vectors)
public:
    unsigned char _before[0x4C];                        // UpdateMode, Anims, InternalAlphas, ... AnimDelay
    long Priority;                                      // +0x4C
    unsigned char _after[0x08];                         // Type, the flag bits
};

namespace std {
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
}
typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetConstIterator;   // list<CAnimSet>::const_iterator

struct CAnimSetItPriorityLessThan {                     // empty functor (a 4-byte argument slot)
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2, _Tp*)
{
    _Tp __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 __fastcall swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2)
{
    for ( ; __first1 != __last1; ++__first1, ++__first2)
        iter_swap(__first1, __first2);
    return __first2;
}

template CAnimSetConstIterator* __fastcall swap_ranges<CAnimSetConstIterator*, CAnimSetConstIterator*>(CAnimSetConstIterator*,
    CAnimSetConstIterator*, CAnimSetConstIterator*);
}