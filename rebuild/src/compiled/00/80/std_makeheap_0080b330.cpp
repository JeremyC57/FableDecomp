// std::__make_heap(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp, CTargetPoint*, int*) -- STLport make_heap of the player-targeting points under the best-for-targeting comparator: sift every parent down, last parent first
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080B330, 255 bytes, /Gr `__fastcall` (first ecx -- kept in the `push ecx` frame
//   word --, last edx; the 52-byte comparator by value and the two type pointers on the stack, `ret 3Ch`).
// Identity: sole caller 0x0080B878 (the CTargetPoint partial_sort of this TU); the element is the 28-byte
//   NPlayerTargeting::CTargetPoint (the divide-by-28 multiply), the comparator CThingCompare_BestForTargeting2 as in the
//   landed sort_heap 0x0080B9F0 / __pop_heap_aux 0x0080B430. The catalog's "std_pop_heap_TargetPointCustom" is wrong.
// Callees: __adjust_heap(first, parent, len, value, comp) 0x0080AD90 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10,
//   GetPItem 0x00A01B50, SetPItem 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   comparator copied memberwise into the argument area (C3DVector's user copy constructor makes its implicit copy
//   non-trivial; the bool moves as a byte), then the target point copy-constructed in its slot (base ctor, vtable,
//   SetPItem(GetPItem()), Index, the position, FrameRejected through a pointer strength-reduced to `first + parent*28 +
//   0x14`).
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint {                                        // 0x1c bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long                        Index;                      // +0x08
    C3DVector                   TargetingPosition;          // +0x0C
    long                        FrameRejected;              // +0x18
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
};
}
using NPlayerTargeting::CTargetPoint;

class CVectorCompare_BestForTargeting { public:                    // Ego_r.pdb, 0x2c bytes
    C3DVector AnchorPos;                                    // +0x00
    C3DVector PreferredLine;                                // +0x0C
    float     AngleCompare;                                 // +0x18
    float     SqrDistCompare;                               // +0x1C
    float     OverrideShortDistanceFactor;                  // +0x20
    float     OverrideShortDistanceFalloff;                 // +0x24
    bool      CompareInXYPlane;                             // +0x28
};

class CThingCompare_BestForTargeting2 {                     // Ego_r.pdb, 0x34 bytes
public:
    CThing*                         Hero;                   // +0x00
    CVectorCompare_BestForTargeting VectorCompare;          // +0x04
    float                           EnemyPreferenceFactor;  // +0x30
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;   // 0x007F9F00 family (out of line here)
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x0080AD90

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2, CTargetPoint*, int*);
}