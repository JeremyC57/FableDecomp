// _global::__partial_sort(pair<CSkeletalMorphs::CEntry,float>* first, pair<CSkeletalMorphs::CEntry,float>* middle, pair<CSkeletalMorphs::CEntry,float>* last, pair<CSkeletalMorphs::CEntry,float>*, CIsSkeletalMorphLowerPriorityThan comp) -- STLport partial_sort core for the weighted skeletal morphs: heapify [first, middle), pop every higher-priority later element into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721400, 157 bytes (160 with padding), `ret 0Ch`.
// Label correction: the catalog "HeapSort_TextureMorph_Partial" is a descriptive auto label; the body is STLport
//   `__partial_sort(first, middle, last, _Tp*, comp)` (the depth-limit fallback of the __introsort_loop
//   0x00721C10, call site 0x00721D01).
// Callees: __make_heap @ 0x0071FE70 (this chunk), __adjust_heap @ 0x0071F650 (landed), sort_heap @ 0x00720D30.
// Convention: /Gr (link-time __fastcall): first in ecx (edi), middle in edx (ebx), then last, the value-type tag
//   pointer and the empty comparator on the stack.
// Shape: make_heap(first, middle, comp) forwards to __make_heap(first, middle, comp, (T*)0, (int*)0); the loop
//   `for (i = middle; i < last; ++i)` re-reads `last` from its argument slot; STLport's inline
//   `__pop_heap(first, middle, i, _Tp(*i), comp, (int*)0)` = `*result = *first; __adjust_heap(first, 0,
//   middle - first, value, comp)`.  The by-value `_Tp(*i)` goes through the pair's memberwise copy constructor:
//   the float weight rides on the x87 stack across the `*i = *first` store and is `fstp`ed into the argument
//   area (element model of the landed _Adjust_heap 0x0071F650).
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
typedef CIsSkeletalMorphLowerPriorityThan TCompare;

void __fastcall __make_heap(SMorphWeight* first, SMorphWeight* last, TCompare comp, SMorphWeight*, int*);   // 0x0071FE70
void __fastcall __adjust_heap(SMorphWeight* first, int holeIndex, int len, SMorphWeight val, TCompare comp);   // 0x0071F650
void __fastcall sort_heap(SMorphWeight* first, SMorphWeight* last, TCompare comp);   // 0x00720D30

inline void make_heap(SMorphWeight* first, SMorphWeight* last, TCompare comp)
{
    __make_heap(first, last, comp, (SMorphWeight*)0, (int*)0);
}

inline void __pop_heap(SMorphWeight* first, SMorphWeight* last, SMorphWeight* result, SMorphWeight value, TCompare comp, int*)
{
    *result = *first;
    __adjust_heap(first, int(0), int(last - first), value, comp);
}

void __fastcall __partial_sort(SMorphWeight* first, SMorphWeight* middle, SMorphWeight* last, SMorphWeight*, TCompare comp)
{
    make_heap(first, middle, comp);
    for (SMorphWeight* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SMorphWeight(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}