// std::sort_heap(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- order a heap of target points by repeatedly popping the best to the back
// Flags: /Gr
// Leaf: sort_heap
// Retail: Fable.exe @ 0x0080B9F0, 191 bytes of code (/O2). Callee: STLport __pop_heap_aux<CTargetPoint*,
// CTargetPoint,CThingCompare_BestForTargeting2> 0x0080B430 (`__fastcall`: first ecx, last edx, the null
// value-type pointer then the 52-byte comparator on the stack, `ret 38h`).
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with the inline pop_heap calling the
// out-of-line `__pop_heap_aux(first, last, _STLP_VALUE_TYPE(first), comp)` -- the `push 0` after the
// comparator copy is that value-type pointer, and its position (BEFORE the comparator) is STLport's
// parameter order (Dinkumware's _Pop_heap_0 takes the pointer last). `last - first` is the signed divide by
// 28 (`imul 92492493h`). The comparator (Ego_r.pdb CThingCompare_BestForTargeting2: Hero +0, the 44-byte
// CVectorCompare_BestForTargeting +4 with CompareInXYPlane at +0x2C, EnemyPreferenceFactor +0x30) is copied
// memberwise into the argument area every pass (C3DVector's user copy ctor makes the copy non-trivial); the
// bool is cached in bl across the loop.
// Label correction: catalog `_Sort_heap<CTargetPoint*,CThingCompare_BestForTargeting2>` is the Ego_r
// (Dinkumware) spelling; retail is STLport sort_heap.
namespace N0080b9f0 {
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;
struct CVectorCompare_BestForTargeting {                  // 0x2c bytes
    C3DVector AnchorPos;                                  // +0x00
    C3DVector PreferredLine;                              // +0x0C
    float AngleCompare;                                   // +0x18
    float SqrDistCompare;                                 // +0x1C
    float OverrideShortDistanceFactor;                    // +0x20
    float OverrideShortDistanceFalloff;                   // +0x24
    bool CompareInXYPlane;                                // +0x28
};
struct CThingCompare_BestForTargeting2 {                  // 0x34 bytes
    CThing* Hero;                                         // +0x00
    CVectorCompare_BestForTargeting VectorCompare;        // +0x04
    float EnemyPreferenceFactor;                          // +0x30
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x0080B430
template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CTargetPoint*, CThingCompare_BestForTargeting2>(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}