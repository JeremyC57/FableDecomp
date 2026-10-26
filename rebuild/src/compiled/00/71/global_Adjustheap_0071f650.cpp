// _global::_Adjust_heap(pair<CSkeletalMorphs::CEntry,float>*, long, long, pair<CSkeletalMorphs::CEntry,float>, CIsSkeletalMorphLowerPriorityThan) -- heap sift-down of weighted skeletal morphs by priority, then _Push_heap @ 0x0071ECF0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F650, 152 bytes.
// Callee: 0x0071ECF0 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the 12-byte pair by value and the 4-byte comparator; `ret 0x14`.
// CSkeletalMorphs::CEntry (PDB, 8 bytes): CDefString MorphName +0, short Priority +4, bool
// SupressesLowerMorphs +6; the pair adds the float weight at +8.  The comparator is
// `a.first.Priority < b.first.Priority` (signed word compare).  Element assignment is a 3-dword block
// copy (trivial operator=), while the by-value pair is built in the _Push_heap slot word by word
// through `mov eax,esp`: the copy constructor is user-provided (modelled on the pair).
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
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

void __fastcall _Push_heap(SMorphWeight* first, long hole, long top, SMorphWeight value, CIsSkeletalMorphLowerPriorityThan pred);   // 0x0071ECF0

void __fastcall _Adjust_heap(SMorphWeight* first, long hole, long bottom, SMorphWeight value, CIsSkeletalMorphLowerPriorityThan pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}