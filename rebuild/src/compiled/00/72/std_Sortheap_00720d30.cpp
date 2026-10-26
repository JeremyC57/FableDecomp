// std::_Sort_heap(SMorphWeight* first, SMorphWeight* last, CIsSkeletalMorphLowerPriorityThan pred) -- Dinkumware _Sort_heap over weighted skeletal morphs (CSkeletalMorphs::CEntry, float) by priority: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720D30, length 169 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...>` is
// propagated over the byte-identical skeleton. The only callee is the landed `_Adjust_heap` @ 0x0071F650 of
// pair<CSkeletalMorphs::CEntry, float> under CIsSkeletalMorphLowerPriorityThan (stride 12: 0x2AAAAAAB / sar 1);
// element model reused (Ego_r.pdb CEntry: CDefString MorphName, short Priority, bool SupressesLowerMorphs).
// Callees: _Adjust_heap @ 0x0071F650 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 14h`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly. The
// `_Ty(*(_Last - 1))` temporary keeps MorphName in ecx, spills the Priority/flag dword to the frame and holds the
// float weight on the x87 stack (the pair's memberwise user copy constructor); `*_Dest = *_First` is the
// implicit assignment (three dword moves through ebp) and the value argument is stored field by field (`fstp`).
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

void __fastcall _Adjust_heap(SMorphWeight* first, long hole, long bottom, SMorphWeight value, CIsSkeletalMorphLowerPriorityThan pred);   // 0x0071F650

inline void _Pop_heap(SMorphWeight* first, SMorphWeight* last, SMorphWeight* dest, SMorphWeight value,
                      CIsSkeletalMorphLowerPriorityThan pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(SMorphWeight* first, SMorphWeight* last, CIsSkeletalMorphLowerPriorityThan pred, SMorphWeight*)
{
    _Pop_heap(first, last - 1, last - 1, SMorphWeight(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SMorphWeight* first, SMorphWeight* last, CIsSkeletalMorphLowerPriorityThan pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SMorphWeight*)0);
}