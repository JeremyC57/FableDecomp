// std::sort(pair<unsigned long,CSkeletalMorphs::CEntry>* first, pair<unsigned long,CSkeletalMorphs::CEntry>* last, CKeyPairCompareLess comp) -- STLport introsort entry for the skeletal-morph map's 12-byte (key, entry) pairs: introsort loop with a 2 * lg(n) depth budget, then the final insertion sort
// Instantiation: std::sort<pair<unsigned long,CSkeletalMorphs::CEntry>*,CKeyPairCompareLess<unsigned long,CSkeletalMorphs::CEntry> >
// Leaf: ??$sort@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E0F0, 82 bytes, __fastcall (first ecx, last edx; the 4-byte comparator
//   slot on the stack), `ret 4`; /O2 (the divide by 12 is the 0x2AAAAAAB multiply).
// Identity (owner item 1f, sweep2): landed as the stand-in `Sort_SElement12_0081e0f0`; the landed CVectorMap::Insert
//   0x0081B220 bound std::sort<pair<unsigned long,CSoundPair>*, ..> to this address, a symbol the landed 0x00470CF1
//   defines (the sound map's sort: CSoundMap::TransferBinaryIn 0x0047042A -> CVectorMap<..,CSoundPair,..>::Sort
//   0x00470CD8 -> 0x00470CF1). This body is the morph TU's: callers are CVectorMap<unsigned long,
//   CSkeletalMorphs::CEntry, ..>::Insert 0x0081B220, ::Sort 0x0081E190 and ::Update 0x0081E1F0 (FableWin.pdb names all
//   three and this sort), the median compares keys unsigned (`jae`/`jb` @ 0x0081DDF8), the pairs are 12 bytes (Ego_r.pdb
//   CSkeletalMorphs::Morphs = CVectorMap<unsigned long, CSkeletalMorphs::CEntry>).
// Callees: __introsort_loop @ 0x0081DD90, __final_insertion_sort @ 0x0081D400 (landed as
//   FinalInsertionSort_SElement12_0081d400; both out of line).
// Shape: STLport 4.6 `sort(first, last, comp)` (the model of the landed 0x00470CF1, here under the TU's /O2): the inline
//   `__lg` count loop, the pushed 0 value-type pointer, the comparator slot reloaded once into ebx for both calls.
class CDefString {                                          // 4 bytes, an index into the def string table
public:
    long TablePos;                                          // +0x0
};

class CSkeletalMorphs {
public:
    class CEntry {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
    public:
        CDefString MorphName;                               // +0x0
        short      Priority;                                // +0x4
        bool       SupressesLowerMorphs;                    // +0x6
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
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
                                 _Compare __comp);                                                   // 0x0081DD90
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0081D400

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

typedef pair<unsigned long, CSkeletalMorphs::CEntry> _MorphEntry;
template void __fastcall sort(_MorphEntry*, _MorphEntry*, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry>);
}