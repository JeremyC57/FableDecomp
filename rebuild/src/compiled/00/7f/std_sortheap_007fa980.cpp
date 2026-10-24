// std::sort_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp) -- heap sort of targeting candidates under the 52-byte best-for-targeting comparator: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FA980, length 157 bytes (+ int3 padding), `ret 34h`.
// Identity: the catalog label `_Sort_heap<pair<long,CDefPointer<CContextSensitiveItemDef const>>*,
// CContextSensitivePriorityCompare>` is propagated over the byte-identical skeleton. The stride is 4 (`and -4 ;
// cmp 4`) and the callee 0x007FA7B0 (manifest "std_sort_Wrapper") is the pop-heap front end of the
// CThing** / CThingCompare_BestForTargeting2 algorithm family (0x007FA160 _Push_heap, 0x007FA320 _Adjust_heap,
// 0x007FA290 __unguarded_insertion_sort_aux, all that instantiation): it takes first/last in ecx/edx, the unused
// value-type pointer and the 52-byte comparator on the stack (`ret 38h`), and calls the pop core 0x007FA680 with
// `*(last - 1)`.
// Callees: __pop_heap_aux @ 0x007FA7B0 (out of line, __fastcall).
// Shape: /Gr algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap inline; the
// comparator is copied into the argument area member by member (dwords through ecx/edx, the CompareInXYPlane byte
// from a bl hoisted out of the loop), which is the implicit copy of a comparator whose C3DVector members have a
// user copy constructor (model of the landed sibling 0x007FA290, Ego_r.pdb layout); then `push 0` for the
// value-type pointer. The byte count `last - first` lives in edi beside `last` in esi.
class CThing;
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CVectorCompare_BestForTargeting {        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                        // +0x00
    C3DVector PreferredLine;                    // +0x0c
    float     AngleCompare;                     // +0x18
    float     SqrDistCompare;                   // +0x1c
    float     OverrideShortDistanceFactor;      // +0x20
    float     OverrideShortDistanceFalloff;     // +0x24
    bool      CompareInXYPlane;                 // +0x28
};
struct CThingCompare_BestForTargeting2 {        // Ego_r.pdb, 52 bytes
    CThing* Hero;                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                // +0x30
};

void __fastcall __pop_heap_aux(CThing** first, CThing** last, CThing**, CThingCompare_BestForTargeting2 comp);   // 0x007FA7B0

inline void pop_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp)
{
    __pop_heap_aux(first, last, (CThing**)0, comp);
}

void __fastcall sort_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}