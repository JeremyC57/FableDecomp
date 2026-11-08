// std::make_heap(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport make_heap front end for target points: forward to __make_heap with the value and distance type pointers
// Leaf: ??$make_heap@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B810, 113 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx -- passed through untouched), the 0x34-byte comparator by value, `ret 34h`.
// Name: STLport `make_heap<CTargetPoint*, CThingCompare_BestForTargeting2>` (the partial_sort front of the target-point
//   introsort); the catalog's "std_sort_heap_TargetPointWrapper" is descriptive.
// Callees: STLport __make_heap(first, last, comp, _Tp*, _Distance*) 0x0080B330.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport `__make_heap(first, last, comp, _STLP_VALUE_TYPE(first, ..), _STLP_DISTANCE_TYPE(first, ..))` -- the
//   two null type pointers pushed first, then the comparator's implicit copy expanded inline (thirteen dwords and the
//   CompareInXYPlane byte through cl); the twin of 0x008097D0.
#include "fable_compiler.h"
class CThing;
class CBaseObject;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


#include "engine/C3DVector.h"

namespace NPlayerTargeting {
class CTargetPoint { public:
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    FABLE_FORCEINLINE CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
    FABLE_FORCEINLINE CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
}
using NPlayerTargeting::CTargetPoint;

class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos;                         // +0x00
    C3DVector PreferredLine;                     // +0x0C
    float     AngleCompare;                      // +0x18
    float     SqrDistCompare;                    // +0x1C
    float     OverrideShortDistanceFactor;       // +0x20
    float     OverrideShortDistanceFalloff;      // +0x24
    bool      CompareInXYPlane;                  // +0x28
};

class CThingCompare_BestForTargeting2 { public:
    CThing*                         Hero;                  // +0x00
    CVectorCompare_BestForTargeting VectorCompare;         // +0x04
    float                           EnemyPreferenceFactor; // +0x30
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0080B330

template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CTargetPoint*)0, (int*)0);
}

template void __fastcall make_heap(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}