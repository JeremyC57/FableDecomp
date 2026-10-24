// _STL::__introsort_loop(pair<CCharString, CMapValue4>* first, pair<CCharString, CMapValue4>* last, pair<CCharString, CMapValue4>*, int depth_limit, CKeyPairCompareLess<CCharString, CMapValue4> comp) -- STLport introsort recursion over a CCharString-keyed vector map: partition around the median of three, recurse right, loop left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41D50, length 171 bytes, `ret 0Ch`.
// Callers: sort 0x00A41E20 (landed: `__introsort_loop(first, last, (T*)0, __lg(n) * 2, comp)`) and itself (0x00A41DCD).
// Callees: 0x00A40590 __median (out of line here: its five string compares are too big to inline), 0x00A40660
//   __unguarded_partition, 0x00A41B60 __partial_sort, CCharString copy ctor 0x0099EC30 (the pivot built in the argument
//   area).
// Element: an 8-byte vector-map entry STLport pair<CCharString, value> (key data pointer +0, a 4-byte value +4 copied as a
//   plain word; the value type is not recoverable -- the landed LowerBound 0x00A404D0 of the same map models it opaque).
// Label correction: catalogued as `Introsort_Partition_CCharStringPair`; the body is STLport `__introsort_loop`.
// Shape: STLport `__introsort_loop` verbatim in a /Gr TU (`__fastcall`: first in ecx -> ebx, last in edx -> edi; the null
//   value-type pointer, the depth limit and the empty comparator on the stack, the comparator word kept in ebp and pushed
//   once above the depth test for both arms); `last - first > __stl_threshold` is `and eax,-8 ; cmp eax,80h`; the
//   decremented depth is stored back to its slot; the pivot `_Tp(__median(...))` runs pair's memberwise user copy ctor
//   with the argument-area pointer homed in the dead comparator slot.
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

struct CMapValue4 {                                  // 4-byte map value (type not recoverable)
    long m_value;
};

template <class K, class V>
class CKeyPairCompareLess {                          // empty key comparator (Ego_r.pdb: 1 byte)
public:
    CKeyPairCompareLess() { }
};

namespace _STL {
template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00A40590

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00A40660
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A41B60

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (pair<CCharString, CMapValue4>*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(pair<CCharString, CMapValue4>*, pair<CCharString, CMapValue4>*,
                                          pair<CCharString, CMapValue4>*, int, CKeyPairCompareLess<CCharString, CMapValue4>);
}