// _STL::__make_heap(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, pair<long, CSkeletalMorph>* last, CKeyPairCompareLess<long, CSkeletalMorph> comp, pair<long, CSkeletalMorph>*, int*) -- STLport heap build over the skeletal-morph vector map: sift down every parent from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B5C0, length 98 bytes (+ int3 padding), `ret 0Ch`.
// Callers: the map's __partial_sort 0x0086D160 (this chunk; 0x0086D174) and 0x0086C6B9.
// Callees: __adjust_heap(first, holeIndex, len, value, comp) @ 0x0086B070 (landed; __fastcall, first in ecx, hole in edx,
//   len, the 16-byte value and the comparator on the stack, `ret 18h`).
// Label: catalog "std::make_heap_CTextureMorphEntry_16byte" is descriptive; the element is the skeletal-morph pair.
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (16 bytes; Ego_r.pdb CSkeletalMorph {GroupID, CDefString Name,
//   bool AllowVariation}), as the landed __adjust_heap 0x0086B070 and this chunk's 0x0086B3B0.
// Shape: STLport 4.x `__make_heap` verbatim (as this chunk's 0x0067EAD0) in a /Gr algorithm TU (`__fastcall` template:
//   first in ecx, last in edx, the comparator and the two type-tag pointers on the stack); the by-value
//   `_Tp(*(__first + __parent))` is built in the outgoing argument block by the pair's user copy constructor (key word,
//   then the 12-byte second through `lea edx,[edi+4]`) after the comparator push.
class CDefString {                                          // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                          // +0x00
};
class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                                  // Ego_r.pdb, 12 bytes (implicit, trivial copy)
    public:
        long       GroupID;                                 // +0x00
        CDefString Name;                                    // +0x04
        bool       AllowVariation;                          // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0086B070

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
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphLess;
template void __fastcall _STL::__make_heap<TSkeletalMorphPair*, TSkeletalMorphLess, TSkeletalMorphPair, int>(TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphLess, TSkeletalMorphPair*, int*);