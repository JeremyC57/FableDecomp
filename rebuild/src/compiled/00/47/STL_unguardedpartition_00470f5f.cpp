// _STL::__unguarded_partition(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, pair<unsigned long,CSoundPair> pivot, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport Hoare partition of the sound map's 12-byte entries around a by-value pivot: returns the first entry of the upper part
// Instantiation: _STL::__unguarded_partition<pair<unsigned long,CSoundPair>*,pair<unsigned long,CSoundPair>,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$__unguarded_partition@PAU?$pair@KVCSoundPair@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSoundPair@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470F5F, 70 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; the 12-byte pivot and the 4-byte comparator slot on the stack), `ret 10h`.
// Identity: called by the landed __introsort_loop @ 0x00470E9E with the median-of-three pivot. The catalog name
//   `std::sort_partition_12byte` is descriptive.
// Shape: STLport 4.6 `__unguarded_partition` with `iter_swap` (the SGI `__iter_swap(a, b, value_type)`) inlined: the
//   pivot key is kept in ecx, so VC7.1 homes the swap temporary in the pivot's own argument slot (`[ebp+8..10h]`);
//   the shared `++first` heads the rotated loop. Frame pointer: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
__forceinline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers
template <class _ForwardIter1, class _ForwardIter2>
__forceinline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template _SoundEntry* __fastcall __unguarded_partition(_SoundEntry*, _SoundEntry*, _SoundEntry,
                                                       CKeyPairCompareLess<unsigned long, CSoundPair>);
}