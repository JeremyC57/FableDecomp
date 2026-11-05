// std::__unguarded_insertion_sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport forwarder: pass the range, the value-type tag and a copy of the comparator to __unguarded_insertion_sort_aux
// Leaf: ??$__unguarded_insertion_sort@PAVCTargetPoint@NPlayerTargeting@@VCThingCompare_BestForTargeting2@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@0VCThingCompare_BestForTargeting2@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080b130, 111 bytes, /Gr __fastcall (first in ecx, last in edx, the 52-byte comparator on the stack), `ret 34h`.
// Identity (proven): bound (reloc_bindings) ??$__unguarded_insertion_sort@PAVCTargetPoint@NPlayerTargeting@@VCThingCompare_BestForTargeting2@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@0VCThingCompare_BestForTargeting2@@@Z; its caller is the landed __final_insertion_sort 0x0080B5A0.
// Callee: __unguarded_insertion_sort_aux 0x0080AB40 (__fastcall).
// Shape (family L067): `sub esp,34h` then the comparator's user copy constructor member by member (Hero, the inner
//   compare's two C3DVectors and four floats, the bool as a byte, EnemyPreferenceFactor), `push 0` for the (T*)0 tag.
class CThing;
#include "engine/C3DVector.h"
class CVectorCompare_BestForTargeting {                    // Ego_r.pdb, 44 bytes
public:
    C3DVector AnchorPos;                                  // +0x00
    C3DVector PreferredLine;                              // +0x0C
    float AngleCompare;                                   // +0x18
    float SqrDistCompare;                                 // +0x1C
    float OverrideShortDistanceFactor;                    // +0x20
    float OverrideShortDistanceFalloff;                   // +0x24
    bool CompareInXYPlane;                                // +0x28
};
class CThingCompare_BestForTargeting2 {                    // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                         // +0x00
    CVectorCompare_BestForTargeting VectorCompare;        // +0x04
    float EnemyPreferenceFactor;                          // +0x30
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other)   // Ego_r.pdb ??0..@@QAE@ABV0@@Z
        : Hero(other.Hero), VectorCompare(other.VectorCompare), EnemyPreferenceFactor(other.EnemyPreferenceFactor) { }
};
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace NPlayerTargeting {
class CTargetPoint {                                       // Ego_r.pdb, 28 bytes
public:
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
namespace std {
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x0080AB40
template <class _RandomAccessIter, class _Compare>
void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, __value_type(__first), __comp);
}
}
template void __fastcall std::__unguarded_insertion_sort(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2);