// std::partial_sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* middle, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport partial_sort front end for the player-targeting candidate points: forward to __partial_sort with the value-type tag
// Leaf: ??$partial_sort@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x0080BCB0, 116 bytes (+ int3 padding), `ret 38h`: __fastcall (the algorithm TUs are /Gr), first
// in ecx, middle in edx (passed through untouched), last and the 52-byte comparator by value on the stack (4 + 0x34).
// Caller: 0x0080C003, the depth-limit fallback of the introsort loop 0x0080BD30. Callee: __partial_sort 0x0080BAB0
// (first, middle, last, value-type pointer, comparator).
// Label: catalogued `Resize_TargetPointVector`, a wrong descriptive name (nothing is resized: the body only forwards).
// Element: Ego_r.pdb NPlayerTargeting::CTargetPoint (28 bytes); comparator: Ego_r.pdb CThingCompare_BestForTargeting2
// (52 bytes: Hero, the 44-byte CVectorCompare_BestForTargeting with the CompareInXYPlane byte at +0x2C,
// EnemyPreferenceFactor +0x30), as in the landed sort_heap 0x0080B9F0.
// Shape: STLport `partial_sort` -- `__partial_sort(first, middle, last, _STLP_VALUE_TYPE(first), comp);` -- the
// comparator copy is memberwise into the argument area (C3DVector's user copy constructor makes the implicit copy
// non-trivial; with a single copy it expands inline, unlike the two copies of sort 0x0080C020).
class CThing;
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace NPlayerTargeting {
class CTargetPoint {                                      // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;
class CVectorCompare_BestForTargeting {                   // 0x2c bytes
public:
    C3DVector AnchorPos;                                  // +0x00
    C3DVector PreferredLine;                              // +0x0C
    float AngleCompare;                                   // +0x18
    float SqrDistCompare;                                 // +0x1C
    float OverrideShortDistanceFactor;                    // +0x20
    float OverrideShortDistanceFalloff;                   // +0x24
    bool CompareInXYPlane;                                // +0x28
};
class CThingCompare_BestForTargeting2 {                   // 0x34 bytes
public:
    CThing* Hero;                                         // +0x00
    CVectorCompare_BestForTargeting VectorCompare;        // +0x04
    float EnemyPreferenceFactor;                          // +0x30
};
namespace std {
template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Tp*, Compare comp);  // 0x0080BAB0

template <class RandomAccessIter, class Compare>
void __fastcall partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, (CTargetPoint*)0, comp);
}
template void __fastcall partial_sort(CTargetPoint*, CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}