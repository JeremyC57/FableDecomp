// std::sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport introsort of the player-targeting candidate points, best target first
// Leaf: ??$sort@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x0080C020, 116 bytes, `ret 34h`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in
// edx, the 52-byte comparator by value on the stack.
// Callers: 0x00804ACD, 0x00804ED6, 0x00804F6A (player targeting). Callees: __introsort_loop 0x0080BD30 (first, last,
// value-type pointer, depth limit, comparator) and __final_insertion_sort 0x0080B5A0 (first, last, comparator); each
// comparator copy is built in the argument area by the out-of-line implicit copy constructor 0x007F9730.
// Label: catalogued `IntroSort_TargetPoint_Setup` (descriptive). Element: Ego_r.pdb NPlayerTargeting::CTargetPoint
// (28 bytes: CIntelligentPointer<CThing> PThing, long Index, C3DVector TargetingPosition, long FrameRejected;
// `imul 92492493h` = /28); comparator: Ego_r.pdb CThingCompare_BestForTargeting2 (52 bytes), as in the landed
// sort_heap 0x0080B9F0.
// Shape: STLport `sort` -- `if (first != last) { __introsort_loop(first, last, _STLP_VALUE_TYPE(first),
// __lg(last - first) * 2, comp); __final_insertion_sort(first, last, comp); }` with `__lg` inlined
// (`for (k = 0; n != 1; n >>= 1) ++k;`).
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
template <class Size>
inline Size __lg(Size n)
{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}
template <class RandomAccessIter, class Tp, class Size, class Compare>
void __fastcall __introsort_loop(RandomAccessIter first, RandomAccessIter last, Tp*, Size depth_limit, Compare comp);  // 0x0080BD30
template <class RandomAccessIter, class Compare>
void __fastcall __final_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp);                  // 0x0080B5A0

template <class RandomAccessIter, class Compare>
void __fastcall sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first != last) {
        __introsort_loop(first, last, (CTargetPoint*)0, __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}
template void __fastcall sort(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}