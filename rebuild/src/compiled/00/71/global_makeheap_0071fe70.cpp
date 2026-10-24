// _global::__make_heap(pair<CSkeletalMorphs::CEntry,float>* first, pair<CSkeletalMorphs::CEntry,float>* last, CIsSkeletalMorphLowerPriorityThan comp, pair<CSkeletalMorphs::CEntry,float>*, int*) -- STLport make_heap core for the weighted skeletal morphs: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071FE70, 101 bytes, `ret 0Ch`.
// Label correction: the catalog "MakeHeap_CSkeletalMorphs_CEntry" names the right container; the element is
//   pair<CSkeletalMorphs::CEntry,float> under CIsSkeletalMorphLowerPriorityThan (Ego_r.pdb `_Make_heap` of that
//   instantiation).  Callers: 0x007205D9 and the __partial_sort 0x00721400.
// Callee: __adjust_heap(first, holeIndex, len, T value, comp) @ 0x0071F650 (landed, same instantiation).
// Convention: /Gr (link-time __fastcall): first in ecx (kept in ebp), last in edx, the comparator and the two
//   type-tag pointers on the stack.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the
//   by-value pair is built in the argument area by its memberwise copy constructor, the source pointer
//   strength-reduced to `first + parent` (edi).
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

void __fastcall __adjust_heap(SMorphWeight* first, int holeIndex, int len, SMorphWeight val,
                              CIsSkeletalMorphLowerPriorityThan comp);   // 0x0071F650

void __fastcall __make_heap(SMorphWeight* first, SMorphWeight* last, CIsSkeletalMorphLowerPriorityThan comp, SMorphWeight*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SMorphWeight(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}