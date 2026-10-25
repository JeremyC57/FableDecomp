// std::__unguarded_linear_insert(pair<EExpressionType, CExpressionSet::CEntry>* last, pair<EExpressionType, CEntry> val, CKeyPairCompareLess<EExpressionType, CEntry> comp) -- STLport insertion step over a creature expression set's vector map: shift every larger-keyed entry up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F090, length 67 bytes, `ret 10h`.
// Callers: the map's __linear_insert 0x0086FA00 (landed; 0x0086FA51, it carries this body as a same-TU stand-in) and
//   0x0086FB20 (a second site in the same sort chain).
// Callees: none.
// Label: catalog "std::pair_insertion_sort_shift" is descriptive; STLport 4.x `__unguarded_linear_insert` verbatim.
// Element: 12 bytes {signed key; 8-byte POD value} (`jge` / `jl`), the long-keyed map the landed 0x0086F3E0 / 0x0086FA00 /
//   0x0086F600 name descriptively pair<long, CLongMapValue8>. Identified here as Ego_r.pdb CExpressionSet::Expressions =
//   CVectorMap<EExpressionType, CExpressionSet::CEntry {EExpressionType Type; long AnimIndex;}, CKeyPairCompareLess<...> >:
//   the map's binary reader / writer 0x0086F680 / 0x0086F590 are called from CCreatureDef::Transfer 0x00675E20 (its
//   CCountedPointer<CExpressionSet> PExpressions), the manifest names the same map's __adjust_heap 0x0086F450
//   `_Adjust_heap<pair<EExpressionType, CExpressionSet::CEntry>*, ...>` (a signed-compare body), and the landed Add
//   0x0086F600 keys each value by its own first word (CEntry::Type). The value layout is the PDB's; the bytes need
//   only its size.
// Shape: /Gr algorithm TU (`__fastcall` template: last in ecx, the 12-byte value and the empty comparator on the stack);
//   the value's key rides in edx, its second words are read from the argument slot for the final store. Byte twin of
//   this chunk's 0x0086A640 with the signed compare.
enum EExpressionType { EXPRESSION_TYPE_NONE = 0 };

class CExpressionSet {
public:
    class CEntry {                                          // Ego_r.pdb, 8 bytes of POD
    public:
        EExpressionType Type;                               // +0x00
        long            AnimIndex;                          // +0x04
    };
};

namespace std {
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<EExpressionType, CExpressionSet::CEntry> TExpressionPair;
typedef CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> TExpressionLess;
template void __fastcall std::__unguarded_linear_insert<TExpressionPair*, TExpressionPair, TExpressionLess>(TExpressionPair*, TExpressionPair, TExpressionLess);