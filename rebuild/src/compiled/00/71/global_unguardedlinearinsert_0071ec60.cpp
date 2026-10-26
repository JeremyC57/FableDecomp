// _global::__unguarded_linear_insert(pair<CSkeletalMorphs::CEntry,float>* last, pair<CSkeletalMorphs::CEntry,float> val, CIsSkeletalMorphLowerPriorityThan comp) -- STLport insertion-sort step over weighted skeletal morphs: shift every higher-priority predecessor up one slot, drop `val` into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071EC60, 66 bytes, `ret 10h`.  No callees.
// Label correction: the catalog "Insertion_InsertSorted_12byte" is a descriptive auto label; the body is STLport
//   `__unguarded_linear_insert(last, val, comp)` of the CSkeletalMorphs priority sort (Ego_r.pdb names the
//   instantiation over pair<CSkeletalMorphs::CEntry,float> with CIsSkeletalMorphLowerPriorityThan).  Callers:
//   0x00720CA1 / 0x00721343 (the __linear_insert / __insertion_sort of the same instantiation).
// Convention: the STLport algorithm TUs are /Gr (link-time __fastcall): `last` in ecx, the 12-byte value and the
//   empty comparator on the stack (12 + 4 = 0x10).
// Element (same model as the landed _Adjust_heap 0x0071F650 / _Push_heap 0x0071ECF0): CSkeletalMorphs::CEntry
//   { CDefString MorphName +0; short Priority +4; bool SupressesLowerMorphs +6 } then the float weight at +8.
//   Assignment is the trivial 3-dword copy, so the value's second dword is loaded once (edx) and serves both
//   the signed word compare (`cmp dx,[next+4] ; jl`) and the final drop.
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

void __fastcall __unguarded_linear_insert(SMorphWeight* last, SMorphWeight val, CIsSkeletalMorphLowerPriorityThan comp)
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