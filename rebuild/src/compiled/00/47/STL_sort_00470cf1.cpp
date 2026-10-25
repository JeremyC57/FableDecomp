// _STL::sort(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport introsort entry for the sound map's 12-byte (key, sound pair) entries: introsort loop with a 2 * lg(n) depth budget, then the final insertion sort
// Instantiation: _STL::sort<pair<unsigned long,CSoundPair>*,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$sort@PAU?$pair@KVCSoundPair@@@_STL@@V?$CKeyPairCompareLess@KVCSoundPair@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470CF1, 69 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; the 4-byte comparator slot on the stack), `ret 4`, no frame pointer.
// Identity: called from CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<..> >::Sort @ 0x00470CD8 (landed as
//   global_TransferToGui_00470cd8), reached from CSoundMap::TransferBinaryIn @ 0x0047042A. The catalog name
//   `std::sort_introsort_init_12byte` is descriptive.
// Callees: __introsort_loop @ 0x00470E9E (landed), __final_insertion_sort @ 0x00470D36 (both out of line).
// Shape: STLport 4.6 `sort(first, last, comp)`: `__lg(last - first)` (a signed divide by 12, `push 0Ch ; pop ecx`) is
//   inlined (`__forceinline` under "s") as the `sar eax,1` count loop, the value-type pointer is the pushed 0, the
//   comparator slot is re-pushed from the argument area for both calls. "s" alone: no frame pointer.
#pragma optimize("s", on)
// (no frame pointer: "s" alone)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
template <class _Size> __forceinline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                                   // 0x00470E9E
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00470D36

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template void __fastcall sort(_SoundEntry*, _SoundEntry*, CKeyPairCompareLess<unsigned long, CSoundPair>);
}