// `anonymous namespace'::AdjustMorphsOfLowerPriority(long count, vector<pair<CSkeletalMorphs::CEntry,float>>& morphs, long priority, float multiplier) -- scale the weight of every one of the first count morphs whose priority is below the given one
// Leaf: ?AdjustMorphsOfLowerPriority@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C120, length 195 bytes (+ int3 padding). `__fastcall` (count in ecx, morphs in
//         edx), `ret 8`. Ego_r.pdb: `?AdjustMorphsOfLowerPriority@?A0x990af2a9@@YIXJAAV?$vector@U?$pair@VCEntry@CSkeletalMorphs@@M@std@@...@std@@JM@Z`
//         (a file-local helper of the CTCHeroMorph TU).
// Callees: none.
// Identity: the manifest label matches the PDB name. Single caller 0x0071E094 (the hero skeletal-morph update). The element
//   is pair<CSkeletalMorphs::CEntry, float> (12 bytes: CEntry = CDefString MorphName, short Priority +4, bool +6 -- the
//   model of the landed _Push_heap 0x0071ECF0; the weight at +8).
// Shape: `for (i = 0; i < count; ++i) if (morphs[i].first.Priority < priority) morphs[i].second *= multiplier;` -- VC7.1
//   unrolls it four times (`(count - 4) >> 2` + 1 passes, then the remainder loop), re-reads the vector start for every
//   element (the float store may alias it) and loads the multiplier first (`fld [esp+..] ; fmul [edi+8]`).
// Emission: the `_Use` caller sits before the definition (VC7.1 would discard the unreferenced internal function).
struct CDefString {
    long TablePos;                                      // +0x00
};
class CSkeletalMorphs {
public:
    class CEntry {                                      // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                           // +0x00
        short Priority;                                 // +0x04
        bool SupressesLowerMorphs;                      // +0x06
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
};
}

namespace {
void __fastcall AdjustMorphsOfLowerPriority(long count, std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& morphs,
                                            long priority, float multiplier);
}

void __fastcall AdjustMorphsOfLowerPriority_Use(long count, std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& morphs,
                                                long priority, float multiplier)
{
    AdjustMorphsOfLowerPriority(count, morphs, priority, multiplier);
}

namespace {
void __fastcall AdjustMorphsOfLowerPriority(long count, std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& morphs,
                                            long priority, float multiplier)
{
    for (long i = 0; i < count; ++i)
        if (morphs[i].first.Priority < priority)
            morphs[i].second *= multiplier;
}
}