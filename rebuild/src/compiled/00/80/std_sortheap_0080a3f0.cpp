// std::sort_heap(SVectorLongPair* first, SVectorLongPair* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- heap sort of (C3DVector, long) first-person target positions under the 44-byte position comparator: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080A3F0, length 143 bytes (+ int3 padding), `ret 2Ch`.
// Identity: the catalog label `_Sort_heap<pair<long,CDefPointer<CContextSensitiveItemDef const>>*,
// CContextSensitivePriorityCompare>` is propagated over the byte-identical skeleton. The stride is 16 (`and -16 ;
// cmp 16`) and the callee 0x00808DB0 (manifest `_Pop_heap_0<pair<unsigned long,CSoundPair>*,...>`, also
// propagated) copies the 44-byte comparator, then `*(last - 1)` (four dwords), and calls the manifest's
// `_Pop_heap<pair<C3DVector,long>*,int,pair<C3DVector,long>,(anon)::CCompareTargetedPositionsForFirstPersonTargeting>`
// @ 0x00808BD0 (`ret 30h`: the value-type pointer + the comparator). Ego_r.pdb:
// CCompareTargetedPositionsForFirstPersonTargeting (44 bytes) = CVectorCompare_BestForTargeting VectorCompare.
// Callees: __pop_heap_aux @ 0x00808DB0 (out of line, __fastcall: first/last in ecx/edx).
// Shape: the 0x007FA980 shape -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap inline;
// the comparator copy is member by member (ten dwords, the CompareInXYPlane byte from a bl hoisted out of the
// loop) because C3DVector has a user copy constructor; then `push 0` for the value-type pointer.
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CVectorCompare_BestForTargeting { public:        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                        // +0x00
    C3DVector PreferredLine;                    // +0x0c
    float     AngleCompare;                     // +0x18
    float     SqrDistCompare;                   // +0x1c
    float     OverrideShortDistanceFactor;      // +0x20
    float     OverrideShortDistanceFalloff;     // +0x24
    bool      CompareInXYPlane;                 // +0x28
};
struct CCompareTargetedPositionsForFirstPersonTargeting {   // Ego_r.pdb, 44 bytes (anonymous namespace)
    CVectorCompare_BestForTargeting VectorCompare;          // +0x00
};
struct SVectorLongPair {                        // std::pair<C3DVector, long> (16 bytes)
    C3DVector first;                            // +0x00
    long      second;                           // +0x0c
};

void __fastcall __pop_heap_aux(SVectorLongPair* first, SVectorLongPair* last, SVectorLongPair*,
                               CCompareTargetedPositionsForFirstPersonTargeting comp);   // 0x00808DB0

inline void pop_heap(SVectorLongPair* first, SVectorLongPair* last, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    __pop_heap_aux(first, last, (SVectorLongPair*)0, comp);
}

void __fastcall sort_heap(SVectorLongPair* first, SVectorLongPair* last, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}