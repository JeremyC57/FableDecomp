// CVectorMap::Sort() -- sort the skeletal-morph map's (key, entry) pairs with its Compare and clear the Dirty flag
// Instantiation: CVectorMap<unsigned long,CSkeletalMorphs::CEntry,CKeyPairCompareLess<unsigned long,CSkeletalMorphs::CEntry> >::Sort
// Leaf: ?Sort@?$CVectorMap@KVCEntry@CSkeletalMorphs@@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081E190, 25 bytes, thiscall, no arguments, no xrefs (an unreferenced COMDAT).
// Identity (owner item 1f, sweep2): FableWin.pdb's name set holds CVectorMap<unsigned long, CSkeletalMorphs::CEntry,
//   CKeyPairCompareLess<..> >::{Insert, Sort, Update, IsDirty, SetDirty, ..}; the callee is that map's sort 0x0081E0F0
//   (std::sort<pair<unsigned long,CSkeletalMorphs::CEntry>*, ..>, the morph TU: CSkeletalMorphs::TransferBinaryIn/Out
//   0x0081AED0/0x0081ADC0 neighbour it). The landed name is propagated across byte-identical CVectorMap bodies
//   (FINDINGS chunk 380; the CSoundPair twin 0x00470CD8 was renamed the same way in sweep1).
// Layout: retail STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0xC, bool Dirty at +0xD (the
//   model of the landed CVectorMap<unsigned long,CSoundPair,..>::Sort 0x00470CD8).
// Callee: std::sort<pair<unsigned long,CSkeletalMorphs::CEntry>*, CKeyPairCompareLess<..> > @ 0x0081E0F0 (STLport,
//   __fastcall: first in ecx, last in edx, the empty comparator by value on the stack, `ret 4`).
// Shape: `xor eax,eax ; mov al,[esi+0Ch] ; push eax` is the by-value copy of the one-byte empty comparator.
class CDefString {                                                 // 4 bytes, an index into the def string table
public:
    long TablePos;                                                 // +0x0
};

class CSkeletalMorphs {
public:
    class CEntry {                                                 // Ego_r.pdb, 8 bytes (a class: mangled V)
    public:
        CDefString MorphName;                                      // +0x0
        short      Priority;                                       // +0x4
        bool       SupressesLowerMorphs;                           // +0x6
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0081E0F0
}

template <class K, class V> class CKeyPairCompareLess {           // empty (FableWin.pdb mangles it as a class)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                             // +0x00
    value_type* Last;                                              // +0x04
    value_type* EndOfStorage;                                      // +0x08
    C           Compare;                                           // +0x0C
    bool        Dirty;                                             // +0x0D
    void Sort();
    void Update();
};

template <class K, class V, class C>
void CVectorMap<K, V, C>::Sort()
{
    std::sort(First, Last, Compare);
    Dirty = false;
}

template class CVectorMap<unsigned long, CSkeletalMorphs::CEntry, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry> >;
