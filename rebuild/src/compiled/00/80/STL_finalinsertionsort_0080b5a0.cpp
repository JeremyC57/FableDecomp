// std::__final_insertion_sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport introsort finish for target points: a guarded insertion sort of the first 16 then an unguarded one of the rest (or one guarded pass when 16 or fewer)
// Leaf: ??$__final_insertion_sort@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B5A0, 115 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x34-byte comparator by value, `ret 34h`.
// Name: STLport `__final_insertion_sort<CTargetPoint*, CThingCompare_BestForTargeting2>`; the catalog's
//   "IntroSort_TargetPoint_Setup" is descriptive.
// Callees: CThingCompare_BestForTargeting2's copy constructor 0x007F9730 (landed, out of line), STLport
//   __insertion_sort(first, last, comp) 0x0080B050 (this chunk) and __unguarded_insertion_sort(first, last, comp)
//   0x0080B130.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport `if (last - first > __stl_threshold) { __insertion_sort(first, first + 16, comp);
//   __unguarded_insertion_sort(first + 16, last, comp); } else __insertion_sort(first, last, comp);` -- with three
//   comparator copies VC7.1 calls the implicit copy constructor out of line (0x007F9730), and the setup of the first
//   copy (`sub esp,34h ; lea edx,&comp ; mov ecx,esp ; push edx`) is hoisted above the branch.
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
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
    __forceinline CTargetPoint& operator=(const CTargetPoint& other)
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
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other);   // 0x007F9730
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0080B050
template <class _RandomAccessIter, class _Compare>
void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0080B130

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}

template void __fastcall __final_insertion_sort(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}