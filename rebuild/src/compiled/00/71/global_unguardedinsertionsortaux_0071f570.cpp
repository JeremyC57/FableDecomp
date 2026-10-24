// _global::__unguarded_insertion_sort_aux(pair<CSkeletalMorphs::CEntry,float>* first, pair<CSkeletalMorphs::CEntry,float>* last, pair<CSkeletalMorphs::CEntry,float>*, CIsSkeletalMorphLowerPriorityThan comp) -- STLport unguarded insertion sort of the weighted skeletal morphs by priority (the tail pass of __final_insertion_sort)
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F570, 101 bytes (112 with padding), `ret 8`.  No callees.
// Label correction: the catalog "Insertion_InsertSorted_12byte" is a descriptive auto label; the body is STLport
//   `__unguarded_insertion_sort_aux(first, last, _Tp*, comp) { for (i = first; i != last; ++i)
//   __unguarded_linear_insert(i, _Tp(*i), comp); }` with the linear insert (0x0071EC60 out of line elsewhere)
//   inlined.  Callers: 0x0071E049 (CTCHeroMorph texture/skeletal morph update, after __insertion_sort of the
//   first 16) and two more __final_insertion_sort sites.
// Convention: /Gr (link-time __fastcall): first in ecx, last in edx (spilled to the frame), the value-type tag
//   pointer and the empty comparator on the stack (`ret 8`).
// Element: the landed _Adjust_heap 0x0071F650 model (CSkeletalMorphs::CEntry + float weight, trivial assignment,
//   memberwise copy constructor).  The by-value `_Tp(*i)` lives in the frame: its first two dwords in ebx/edi,
//   the float weight as a dword in the frame slot; the inner shift loop uses edx as the copy register and
//   re-reads `last` from its spill.
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

inline void __unguarded_linear_insert(SMorphWeight* last, SMorphWeight val, CIsSkeletalMorphLowerPriorityThan comp)
{
    SMorphWeight* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(SMorphWeight* first, SMorphWeight* last, SMorphWeight*, CIsSkeletalMorphLowerPriorityThan comp)
{
    for (SMorphWeight* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SMorphWeight(*i), comp);
}