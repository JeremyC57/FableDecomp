// std::__introsort_loop(pair<ETCInterfaceType, CTCBase*>* first, pair<ETCInterfaceType, CTCBase*>* last, pair<ETCInterfaceType, CTCBase*>*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort core over a thing's component map: quicksort around a median-of-three pivot until runs are 16 entries or shorter, heap sort at the depth limit
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD500, 196 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, then the value-type tag pointer, the depth limit and the comparator slot).
// Callers: itself (0x004CD58D) and the sort front end 0x004CD608 (landed 0x004CD5D0, which CComponentMap::Resort
//   0x004CD650 calls).
// Callees: __unguarded_partition 0x004CC690 and __partial_sort 0x004CD370 (both this chunk).
// Identity: CVectorMap<ETCInterfaceType, CTCBase*> entry pair (8 bytes, signed key); the catalog's
//   "std::heapsort_Recursive" is descriptive.
// Shape: STLport 4.x `__introsort_loop` verbatim: the length test is the byte form (`and -8 ; cmp 80h`), the depth limit
//   is decremented in its argument slot, __median returns a reference (the three keys in ecx/edx/ebp, the chosen address
//   in eax) and the pivot copy is built in the argument area.
enum ETCInterfaceType { TC_NONE = 0 };                  // the thing-component interface type (a signed key)
class CTCBase;

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<ETCInterfaceType, CTCBase*> TComponentPair;          // CThing::Components entry, 8 bytes
typedef CKeyPairCompareLess<ETCInterfaceType, CTCBase*> TComponentLess;

namespace std {
template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x004CC690
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004CD370

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {                     // __stl_threshold
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template void __fastcall __introsort_loop<TComponentPair*, TComponentPair, int, TComponentLess>(TComponentPair*, TComponentPair*, TComponentPair*, int, TComponentLess);
}