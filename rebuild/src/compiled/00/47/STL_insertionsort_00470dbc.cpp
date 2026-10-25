// std::__insertion_sort(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport insertion sort over the sound map's 12-byte (key, sound pair) entries: every entry after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Instantiation: std::__insertion_sort<pair<unsigned long,CSoundPair>*,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$__insertion_sort@PAU?$pair@KVCSoundPair@@@std@@V?$CKeyPairCompareLess@KVCSoundPair@@@@@std@@YIXPAU?$pair@KVCSoundPair@@@0@0V?$CKeyPairCompareLess@KVCSoundPair@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470DBC, 76 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
//   4-byte comparator slot on the stack); size-optimised frame-pointer TU (`push ebp ; mov ebp,esp ; ... leave`).
// Retyped (sweep1, 2026-10-25; FINDINGS chunk 457: "0x470DBC comparator is the empty CKeyPairCompareLess"): the element is
//   CSoundMap::SoundMap's pair<unsigned long, CSoundPair> (Ego_r.pdb CSoundPair: FirstSound, LastSound -- the nested
//   two-dword `second` the earlier model called SPayload8) and the comparator the empty CKeyPairCompareLess<unsigned long,
//   CSoundPair>, the types of its landed callee std::__linear_insert 0x00470E08 (hand457, whose Leaf is
//   ??$__linear_insert@PAU?$pair@KVCSoundPair@@@std@@U12@V?$CKeyPairCompareLess@KVCSoundPair@@@@@std@@) and caller
//   std::__final_insertion_sort 0x00470D36 (the sound-map sort tail). The earlier SULongKeyedRecord / SULongKeyLess
//   stand-ins (a 4-byte "context" comparator) gave the same bytes under a placeholder Leaf.
// Shape: `last` lives in the 4-byte frame word (`[ebp-4]`), the comparator is pushed straight from its slot (`push
//   [ebp+8]`), STLport's explicit pair copy constructor builds the by-value `*i` in the argument area with the payload
//   read through a second induction pointer (`lea edi,[esi+4]`). Frame pointer: optimize("y", off) + ("s", on).
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
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00470E08

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template void __fastcall __insertion_sort(_SoundEntry*, _SoundEntry*, CKeyPairCompareLess<unsigned long, CSoundPair>);
}
