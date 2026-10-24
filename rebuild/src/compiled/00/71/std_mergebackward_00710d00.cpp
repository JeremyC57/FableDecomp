// std::__merge_backward(list<CAnimSet>::const_iterator* first1, last1, first2, last2, result, CAnimSetItPriorityLessThan comp) -- merge two sorted runs backwards into the range ending at result
// Leaf: ??$__merge_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710D00, length 182 bytes (+ int3 padding). __fastcall (first1 ecx, last1
//         edx), `ret 10h`, result in eax.
// Callees: std::copy_backward<4-byte pointer> @ 0x007108A0 (COMDAT-folded; its manifest label
//          "_Copy_impl<pair<float,EConversationAttitude>*>" is a fold sibling).
// Identity: the manifest label "StableSortMerge_Pair_EConversationAttitude" is a fold sibling's; the comparator offset
//   (+0x54 of the node) pins this instance. Called by the buffered __merge_adaptive 0x00711340.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `__merge_backward` verbatim. copy_backward is the STLport chain (copy_backward -> __copy_backward_aux
//   (pointer __true_type) -> __copy_backward_ptrs (__false_type) -> the counted random-access __copy_backward): VC7.1
//   inlines the two early-out calls and keeps the two in-loop calls out of line (0x007108A0), as retail does.
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
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp>
inline _Tp* __copy_backward_aux(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __true_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __fastcall __merge_backward(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
                                                _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                                                _BidirectionalIter3 __result, _Compare __comp)
{
    if (__first1 == __last1)
        return copy_backward(__first2, __last2, __result);
    if (__first2 == __last2)
        return copy_backward(__first1, __last1, __result);
    --__last1;
    --__last2;
    while (true) {
        if (__comp(*__last2, *__last1)) {
            *--__result = *__last1;
            if (__first1 == __last1)
                return copy_backward(__first2, ++__last2, __result);
            --__last1;
        }
        else {
            *--__result = *__last2;
            if (__first2 == __last2)
                return copy_backward(__first1, ++__last1, __result);
            --__last2;
        }
    }
}

template CAnimSetConstIterator* __fastcall __merge_backward<CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*,
    CAnimSetItPriorityLessThan>(CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*,
    CAnimSetConstIterator*, CAnimSetItPriorityLessThan);
}