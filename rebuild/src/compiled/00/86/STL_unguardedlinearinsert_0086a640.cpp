// _STL::__unguarded_linear_insert(pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>* last, pair<unsigned long, CTextureMorph> val, CKeyPairCompareLess<unsigned long, CTextureMorph> comp) -- STLport insertion step over a texture-morph vector map: shift every larger-keyed entry up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086A640, length 67 bytes (+ int3 padding), `ret 10h`.
// Callers: __linear_insert 0x0086C5C0 (landed; 0x0086C611, it carries this body as a same-TU stand-in) and
//   0x0086C810 (a second site in the same map's sort chain).
// Callees: none.
// Label: catalog "std::pair_insertion_sort_shift" is descriptive; STLport 4.x `__unguarded_linear_insert` verbatim.
// Element: pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> (12 bytes; Ego_r.pdb CTextureMorph {NewTexture,
//   GroupID}); the key compare `a.first < b.first` is unsigned (`jae` / `jb`). Same model as the landed 0x0086C5C0.
// Shape: /Gr algorithm TU (`__fastcall` template: last in ecx, the 12-byte value and the empty comparator on the stack);
//   the value's key rides in edx, its second words are read from the argument slot for the final store.
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                   // Ego_r.pdb, 8 bytes of POD
    public:
        unsigned long NewTexture;                           // +0x00
        unsigned long GroupID;                              // +0x04
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> TTextureMorphPair;
typedef CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> TTextureMorphLess;
template void __fastcall _STL::__unguarded_linear_insert<TTextureMorphPair*, TTextureMorphPair, TTextureMorphLess>(TTextureMorphPair*, TTextureMorphPair, TTextureMorphLess);