// _global::swap(pair<CSkeletalMorphs::CEntry,float>& a, pair<CSkeletalMorphs::CEntry,float>& b) -- STLport swap of two weighted skeletal-morph entries through a by-value temporary
// Leaf: swap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F530, 55 bytes (64 with padding), plain `ret`.  No callees.
// Label correction: the manifest "GFToScreen(C2DBoxF*)" is propagated across byte-identical bodies; this one
//   sits inside the CSkeletalMorphs priority-sort instantiation block (0x0071EC60 .. 0x0071FE70) and is STLport
//   `swap(_Tp& a, _Tp& b) { _Tp tmp = a; a = b; b = tmp; }` over a 12-byte element (no direct callers: the
//   swap is otherwise inlined by iter_swap, e.g. in __unguarded_partition 0x0071FC20).
// Convention: /Gr (link-time __fastcall): a in ecx, b in edx.
// Shape: tmp is a frame temporary (`sub esp,0Ch`): its first two words stay in eax/esi, the third is parked in
//   the frame; `a = b` goes through ebx, then the three tmp words are stored into b.  Element model of the landed
//   _Adjust_heap 0x0071F650 (memberwise copy constructor, trivial assignment).
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

void __fastcall swap(SMorphWeight& a, SMorphWeight& b)
{
    SMorphWeight tmp = a;
    a = b;
    b = tmp;
}