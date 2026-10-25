// std::__merge_adaptive(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* middle, list<CAnimSet>::const_iterator* last, int len1, int len2, list<CAnimSet>::const_iterator* buffer, int buffer_size, CAnimSetItPriorityLessThan comp) -- merge two adjacent sorted runs by priority, through the buffer when one run fits, else split, rotate and recurse
// Leaf: ??$__merge_adaptive@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00711340, 384 bytes, __fastcall (first ecx, middle edx), `ret 18h` (last, len1,
//   len2, buffer, buffer_size, the empty comparator's slot).
// Label correction: catalog "StableSortMerge_AnimSet" -> STLport 4.6 __merge_adaptive of the CAnimateThing TU's
//   stable_sort chain over list<CAnimSet>::const_iterator with CAnimSetItPriorityLessThan.
// Callees: merge 0x00710310, __merge_backward 0x00710D00, __lower_bound 0x0070FCF0, __upper_bound 0x0070FD30,
//   __rotate_adaptive 0x00710DC0, itself 0x00711340 (the first recursion; VC7.1 turns the second into the loop).
// Shape: STLport verbatim. copy() is the counted random-access __copy, inlined; lower_bound / upper_bound / advance /
//   distance are inline forwarders (the `push 0` is the null distance-type pointer).
class CAnimSet {                                // retail layout
public:
    unsigned char _before[0x4C];
    long Priority;                              // +0x4C
    unsigned char _after[0x08];
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

class CAnimSetItPriorityLessThan {              // empty functor (a 4-byte argument slot)
public:
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter __fastcall merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp);                         // 0x00710310
template <class _BidirectionalIter1, class _BidirectionalIter2, class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __fastcall __merge_backward(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
                                                _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                                                _BidirectionalIter3 __result, _Compare __comp);   // 0x00710D00
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*);   // 0x0070FCF0
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*);   // 0x0070FD30
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter1 __fastcall __rotate_adaptive(_BidirectionalIter1 __first, _BidirectionalIter1 __middle, _BidirectionalIter1 __last,
                                                 _Distance __len1, _Distance __len2,
                                                 _BidirectionalIter2 __buffer, _Distance __buffer_size);   // 0x00710DC0

template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __lower_bound(__first, __last, __val, __comp, __distance_type(__first));
}
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __upper_bound(__first, __last, __val, __comp, __distance_type(__first));
}

template <class _RandomAccessIter, class _Distance>
inline void __advance(_RandomAccessIter& __i, _Distance __n, const random_access_iterator_tag&) { __i += __n; }
template <class _InputIter, class _Distance>
inline void advance(_InputIter& __i, _Distance __n) { __advance(__i, __n, random_access_iterator_tag()); }

template <class _RandomAccessIter>
inline int __distance(const _RandomAccessIter& __first, const _RandomAccessIter& __last, const random_access_iterator_tag&)
{
    return __last - __first;
}
template <class _InputIter>
inline int distance(const _InputIter& __first, const _InputIter& __last)
{
    return __distance(__first, __last, random_access_iterator_tag());
}

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
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2,
                                 _Pointer __buffer, _Distance __buffer_size, _Compare __comp)
{
    if (__len1 <= __len2 && __len1 <= __buffer_size) {
        _Pointer __buffer_end = copy(__first, __middle, __buffer);
        merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
    }
    else if (__len2 <= __buffer_size) {
        _Pointer __buffer_end = copy(__middle, __last, __buffer);
        __merge_backward(__first, __middle, __buffer, __buffer_end, __last, __comp);
    }
    else {
        _BidirectionalIter __first_cut = __first;
        _BidirectionalIter __second_cut = __middle;
        _Distance __len11 = 0;
        _Distance __len22 = 0;
        if (__len1 > __len2) {
            __len11 = __len1 / 2;
            advance(__first_cut, __len11);
            __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
            __len22 += distance(__middle, __second_cut);
        }
        else {
            __len22 = __len2 / 2;
            advance(__second_cut, __len22);
            __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
            __len11 += distance(__first, __first_cut);
        }
        _BidirectionalIter __new_middle =
            __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11, __len22, __buffer, __buffer_size);
        __merge_adaptive(__first, __first_cut, __new_middle, __len11, __len22, __buffer, __buffer_size, __comp);
        __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __buffer, __buffer_size, __comp);
    }
}

template void __fastcall __merge_adaptive(CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int, int,
                                          CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan);

}