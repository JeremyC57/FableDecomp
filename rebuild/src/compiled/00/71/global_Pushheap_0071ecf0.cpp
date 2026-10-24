// _global::_Push_heap(pair<CSkeletalMorphs::CEntry,float>*, long, long, pair<CSkeletalMorphs::CEntry,float>, CIsSkeletalMorphLowerPriorityThan) -- heap sift-up of weighted skeletal morphs by priority
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071ECF0, 108 bytes, `ret 0x14`.  No callees.
// Called by the landed _Adjust_heap @ 0x0071F650 (same instantiation; its model is reused here).
// CSkeletalMorphs::CEntry (PDB, 8 bytes): CDefString MorphName +0, short Priority +4, bool
// SupressesLowerMorphs +6; the pair adds the float weight at +8.  The comparator is
// `a.first.Priority < b.first.Priority` (`cmp word ptr [parent+4],bx ; jge`, signed): the value's
// word at +4 is loaded once into ebx at entry and its whole dword is reused by the final drop.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 12-byte pair by
// value and the empty comparator (4 + 12 + 4 = 0x14).  Element assignment is three dword moves
// (trivial operator=).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N0071ecf0 {
struct CDefString {
    long TablePos;
};

struct CSkeletalMorphsEntry {                   // CSkeletalMorphs::CEntry
    CDefString MorphName;                       // +0x0
    short Priority;                             // +0x4
    bool SupressesLowerMorphs;                  // +0x6
};

struct SMorphWeight {                           // std::pair<CSkeletalMorphs::CEntry, float>
    CSkeletalMorphsEntry first;                 // +0x0
    float second;                               // +0x8
    SMorphWeight(const SMorphWeight& other) : first(other.first), second(other.second) { }
};

struct CIsSkeletalMorphLowerPriorityThan {
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};

void __fastcall _Push_heap(SMorphWeight* first, long hole, long top, SMorphWeight value, CIsSkeletalMorphLowerPriorityThan comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}