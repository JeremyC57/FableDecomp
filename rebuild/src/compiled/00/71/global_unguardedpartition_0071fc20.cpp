// _global::__unguarded_partition(pair<CSkeletalMorphs::CEntry,float>* first, pair<CSkeletalMorphs::CEntry,float>* last, pair<CSkeletalMorphs::CEntry,float> pivot, CIsSkeletalMorphLowerPriorityThan comp) -- STLport quicksort partition of the weighted skeletal morphs around a by-value pivot priority; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071FC20, 127 bytes, `ret 10h`.  No callees.
// Label correction: the catalog "Partition_12byte" is a descriptive auto label; the body is STLport
//   `__unguarded_partition(first, last, pivot, comp)` (its only caller 0x00721CBB is the __introsort_loop of the
//   same instantiation, which passes the median-of-three by value).
// Convention: /Gr (link-time __fastcall): first in ecx (moved to eax), last in edx, the 12-byte pivot and the empty
//   comparator on the stack (12 + 4 = 0x10).
// Element: the landed _Adjust_heap 0x0071F650 model.  Shape: the pivot priority is read once (cx); STLport
//   `iter_swap` is inlined as `_Tp tmp = *a; *a = *b; *b = tmp;` -- VC7.1 parks tmp's last two words in the
//   pivot's dead argument slots (`[esp+18h]` / `[esp+1Ch]`).
class CDefString { public:
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

class CIsSkeletalMorphLowerPriorityThan { public:
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};

inline void __iter_swap(SMorphWeight* a, SMorphWeight* b, SMorphWeight*)
{
    SMorphWeight tmp = *a;
    *a = *b;
    *b = tmp;
}

inline void iter_swap(SMorphWeight* a, SMorphWeight* b)
{
    __iter_swap(a, b, (SMorphWeight*)0);
}

SMorphWeight* __fastcall __unguarded_partition(SMorphWeight* first, SMorphWeight* last, SMorphWeight pivot, CIsSkeletalMorphLowerPriorityThan comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}