// std::__rotate_adaptive(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* middle, list<CAnimSet>::const_iterator* last, int len1, int len2, list<CAnimSet>::const_iterator* buffer, int buffer_size) -- rotate [first, last) around middle through the temporary buffer when one side fits, else the in-place rotate
// Leaf: ??$__rotate_adaptive@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00710DC0, 262 bytes, __fastcall (first ecx, middle edx), `ret 14h` (last, len1,
//   len2, buffer, buffer_size).
// Label correction: catalog "StableSortMerge_Rotate" -> STLport 4.6 __rotate_adaptive of the CAnimateThing TU's
//   stable_sort chain over list<CAnimSet>::const_iterator (called by __merge_adaptive 0x00711340).
// Callees: __rotate 0x007103C0 (the in-place fallback).
// Shape: STLport verbatim: `if (len1 > len2 && len2 <= buffer_size)` copy the right run to the buffer, copy_backward the
//   left run up, copy the buffer back to first; `else if (len1 <= buffer_size)` the mirror; else rotate(first, middle,
//   last) -> __rotate(..., (int*)0, (T*)0). copy / copy_backward over the non-trivial iterator type reduce to
//   STLport's counted random-access __copy / __copy_backward loops, all inlined (`__forceinline`: VC7.1 otherwise
//   leaves the first copy_backward out of line, 250 v 264).
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

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                      _Distance*, _Tp*);   // 0x007103C0

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last)
{
    return __rotate(__first, __middle, __last, __distance_type(__first), __value_type(__first));
}

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
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter1 __fastcall __rotate_adaptive(_BidirectionalIter1 __first, _BidirectionalIter1 __middle, _BidirectionalIter1 __last,
                                                 _Distance __len1, _Distance __len2,
                                                 _BidirectionalIter2 __buffer, _Distance __buffer_size)
{
    _BidirectionalIter2 __buffer_end;
    if (__len1 > __len2 && __len2 <= __buffer_size) {
        __buffer_end = copy(__middle, __last, __buffer);
        copy_backward(__first, __middle, __last);
        return copy(__buffer, __buffer_end, __first);
    }
    else if (__len1 <= __buffer_size) {
        __buffer_end = copy(__first, __middle, __buffer);
        copy(__middle, __last, __first);
        return copy_backward(__buffer, __buffer_end, __last);
    }
    else
        return rotate(__first, __middle, __last);
}

template CAnimSetConstIterator* __fastcall __rotate_adaptive(CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*,
                                                             int, int, CAnimSetConstIterator*, int);

}