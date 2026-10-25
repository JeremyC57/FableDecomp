// std::__pop_heap_aux(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, pair<unsigned long,CSoundPair>*, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport pop_heap front end for the sound map's 12-byte entries: move the back entry out, put the root there and sift the old back entry down
// Instantiation: std::__pop_heap_aux<pair<unsigned long,CSoundPair>*,pair<unsigned long,CSoundPair>,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$__pop_heap_aux@PAU?$pair@KVCSoundPair@@@std@@U12@V?$CKeyPairCompareLess@KVCSoundPair@@@@@std@@YIXPAU?$pair@KVCSoundPair@@@0@00V?$CKeyPairCompareLess@KVCSoundPair@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471150, 74 bytes, __fastcall (first ecx, last edx; the unused value-type
//   pointer and the empty comparator on the stack), `ret 8`; frame pointer + `leave`, `idiv` by 12, `movsd` copies.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x00471150 is the CSoundPair __pop_heap_aux ...
//   landed with a CTextureMorph element"); its caller is the landed sound-map std::__partial_sort 0x00470FA5 (whose reloc
//   binding already names this Leaf -> 00471150), its callee the sound map's __adjust_heap 0x00471055 (the same
//   0x00470FA5 binds it as __adjust_heap<pair<unsigned long,CSoundPair>*,int,..>). Element = CSoundMap::SoundMap's
//   pair<unsigned long, CSoundPair> (Ego_r.pdb CSoundPair: FirstSound, LastSound). The catalog's
//   _Pop_heap_0<pair<float,CSubtitledMessage::CCameraTag> > (a 0x18-byte element) is propagated.
// Shape: STLport 4.6 __pop_heap_aux with __pop_heap inlined (`__forceinline` under "s"): the _Tp(*(last - 1)) copy is
//   built in the argument area by pair's member-wise copy constructor, the root is block-copied into the back slot.
//   Frame pointer: `optimize("y", off)` + `optimize("s", on)`, as the module's other landed bodies (0x004710F0).
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
template <class _Tp> __forceinline int* __distance_type(const _Tp*) { return (int*)0; }  // _STLP_DISTANCE_TYPE for pointers

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x00471055

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

typedef pair<unsigned long, CSoundPair> _Entry;
template void __fastcall __pop_heap_aux(_Entry*, _Entry*, _Entry*, CKeyPairCompareLess<unsigned long, CSoundPair>);
}
