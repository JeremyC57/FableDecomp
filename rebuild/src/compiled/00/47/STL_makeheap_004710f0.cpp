// std::__make_heap(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, CKeyPairCompareLess<unsigned long,CSoundPair> comp, pair<unsigned long,CSoundPair>*, int*) -- STLport make_heap over the sound map's 12-byte entries: sift every parent from (len - 2) / 2 down to 0 into place
// Instantiation: std::__make_heap<pair<unsigned long,CSoundPair>*,CKeyPairCompareLess<unsigned long,CSoundPair>,pair<unsigned long,CSoundPair>,int>
// Leaf: ??$__make_heap@PAU?$pair@KVCSoundPair@@@std@@V?$CKeyPairCompareLess@KVCSoundPair@@@@U12@H@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004710F0, 96 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; the comparator slot, the value-type and distance-type pointers on the stack), `ret 0Ch`.
// Identity: called by the sound map's __partial_sort @ 0x00470FA5 (make_heap(first, middle, comp) inlined there).
//   The catalog name `std::make_heap_init_12byte` is descriptive.
// Callees: __adjust_heap(first, holeIndex, len, value, comp) @ 0x00471055 (landed, out of line).
// Shape: STLport 4.6 `__make_heap`: len = (last - first) by a signed divide by 12 (`push 0Ch ; pop ecx`) kept in the
//   4-byte frame word, parent = (len - 2) / 2, the by-value `_Tp(*(first + parent))` built in the argument area with
//   pair's member-wise copy constructor; the parent pointer walks down 12 bytes per step.
//   Frame pointer: `optimize("y", off)` + `optimize("s", on)` as the module's other landed bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x00471055

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template void __fastcall __make_heap(_SoundEntry*, _SoundEntry*, CKeyPairCompareLess<unsigned long, CSoundPair>, _SoundEntry*, int*);
}