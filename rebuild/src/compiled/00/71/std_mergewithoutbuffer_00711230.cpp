// std::__merge_without_buffer(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* middle, list<CAnimSet>::const_iterator* last, int len1, int len2, CAnimSetItPriorityLessThan comp) -- merge the two sorted runs [first, middle) and [middle, last) in place without a buffer: cut the longer run in half, find the matching cut in the other run by binary search, rotate, and recurse on both halves (the second as a loop)
// Leaf: ??$__merge_without_buffer@PAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@HVCAnimSetItPriorityLessThan@@@std@@YIXPAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@0@00HHVCAnimSetItPriorityLessThan@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00711230, 262 bytes, __fastcall (first ecx, middle edx; last, len1, len2 and the 4-byte
//   comparator slot on the stack), `ret 10h`.
// Identity (proven): STLport 4.6 `_algo.c` __merge_without_buffer line for line (len1/len2 zero exit, the two-element
//   compare-and-iter_swap, the longer-run cut with lower_bound / upper_bound, rotate, first recursion, the second recursion
//   folded into the loop); its callers are the landed __inplace_stable_sort 0x00711640 (0x00711697, which names this
//   address and argument order) and itself (0x007112E9). Ego_r.pdb (Dinkumware STL) holds the same stable_sort chain over
//   list<CAnimSet>::const_iterator with CAnimSetItPriorityLessThan. The catalog label "AdaptiveSortMerge_AnimSet" is
//   descriptive.
// Callees: __lower_bound<.., int> 0x0070FCF0 and __upper_bound<.., int> 0x0070FD30 (fastcall, value by reference, comparator by
//   value, (int*)0 distance tag), __rotate<.., int, ..> 0x007103C0 (fastcall, (int*)0 and (value*)0 tags) -- the landed
//   spellings; CAnimSetItPriorityLessThan::operator() inline (CAnimSet Priority, retail +0x4C).
// Shape: STLport verbatim; advance / distance are the random-access forms (`+=`, `n += last - first`); iter_swap and rotate
//   name the value type directly where STLport derives it through _STLP_VALUE_TYPE (same code).
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
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
}   // namespace std
typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetConstIterator;   // list<CAnimSet>::const_iterator

class CAnimSetItPriorityLessThan {                      // empty functor (a 4-byte argument slot)
public:
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    CAnimSetConstIterator __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}
template <class _RandomAccessIter, class _Distance>
inline void advance(_RandomAccessIter& __i, _Distance __n) { __i += __n; }
template <class _RandomAccessIter, class _Distance>
inline void distance(const _RandomAccessIter& __first, const _RandomAccessIter& __last, _Distance& __n) { __n += __last - __first; }

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*);                                                  // 0x0070FCF0
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*);                                                  // 0x0070FD30
template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                      _Distance*, _Tp*);                                             // 0x007103C0
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __lower_bound(__first, __last, __val, __comp, (int*)0);
}
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __upper_bound(__first, __last, __val, __comp, (int*)0);
}
template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last)
{
    return __rotate(__first, __middle, __last, (int*)0, (CAnimSetConstIterator*)0);
}

template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp)
{
    if (__len1 == 0 || __len2 == 0)
        return;
    if (__len1 + __len2 == 2) {
        if (__comp(*__middle, *__first))
            iter_swap(__first, __middle);
        return;
    }
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
        __len11 = __len1 / 2;
        advance(__first_cut, __len11);
        __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
        distance(__middle, __second_cut, __len22);
    } else {
        __len22 = __len2 / 2;
        advance(__second_cut, __len22);
        __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
        distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle = rotate(__first_cut, __middle, __second_cut);
    __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22, __comp);
    __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __comp);
}

template void __fastcall __merge_without_buffer<CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int, int, CAnimSetItPriorityLessThan);
}   // namespace std