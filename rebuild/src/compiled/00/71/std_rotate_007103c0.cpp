// std::__rotate(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* middle, list<CAnimSet>::const_iterator* last, int*, list<CAnimSet>::const_iterator*) -- rotate [first, last) so that middle becomes the first element (cycle-leader by gcd); returns first + (last - middle)
// Leaf: ??$__rotate@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007103C0, 305 bytes, __fastcall (first ecx, middle edx), `ret 0Ch` (last, the
//   distance and value-type tag pointers).
// Label correction: catalog "RotateBuffer_GCD" -> STLport 4.6 random-access __rotate of the CAnimateThing TU's
//   stable_sort chain over list<CAnimSet>::const_iterator (the landed swap_ranges 0x0070FDC0 is its `k == l` case;
//   callers 0x007108D8 (tail jump from rotate), 0x00710CE9, 0x00710EBC, 0x007112D0 in __rotate_adaptive /
//   __merge_without_buffer).
// Callees: swap_ranges 0x0070FDC0.
// Shape: STLport verbatim: n, k, l and result = first + (last - middle) up front; `k == 0` returns last, `k == l`
//   swap_ranges and returns result; d = __gcd(n, k) inlined (`cdq ; idiv` loop); for each of the d cycles a
//   tmp = *first, then the `k < l` walk (`if (p > first + l) {...}` then `*p = *(p + k)`) or the `k >= l` walk, and
//   `*p = tmp; ++first`.
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

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 __fastcall swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2);   // 0x0070FDC0

template <class _EuclideanRingElement>
inline _EuclideanRingElement __gcd(_EuclideanRingElement __m, _EuclideanRingElement __n)
{
    while (__n != 0) {
        _EuclideanRingElement __t = __m % __n;
        __m = __n;
        __n = __t;
    }
    return __m;
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                      _Distance*, _Tp*)
{
    _Distance __n = __last - __first;
    _Distance __k = __middle - __first;
    _Distance __l = __n - __k;
    _RandomAccessIter __result = __first + (__last - __middle);

    if (__k == 0)
        return __last;

    if (__k == __l) {
        swap_ranges(__first, __middle, __middle);
        return __result;
    }

    _Distance __d = __gcd(__n, __k);

    for (_Distance __i = 0; __i < __d; __i++) {
        _Tp __tmp = *__first;
        _RandomAccessIter __p = __first;

        if (__k < __l) {
            for (_Distance __j = 0; __j < __l / __d; __j++) {
                if (__p > __first + __l) {
                    *__p = *(__p - __l);
                    __p -= __l;
                }

                *__p = *(__p + __k);
                __p += __k;
            }
        }
        else {
            for (_Distance __j = 0; __j < __k / __d - 1; __j++) {
                if (__p < __last - __k) {
                    *__p = *(__p + __k);
                    __p += __k;
                }

                *__p = *(__p - __l);
                __p -= __l;
            }
        }

        *__p = __tmp;
        ++__first;
    }

    return __result;
}

template CAnimSetConstIterator* __fastcall __rotate(CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int*, CAnimSetConstIterator*);

}