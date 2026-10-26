// std::__adjust_heap(NPlayerTargeting::CTargetPoint* first, int holeIndex, int len, NPlayerTargeting::CTargetPoint val, CThingCompare_BestForTargeting2 comp) -- STLport heap sift-down of the targeting candidates: walk the hole to a leaf along the child the targeting predicate ranks higher, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAVCTargetPoint@NPlayerTargeting@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080AD90, 429 bytes (+ int3 padding), `ret 54h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 0x1C-byte CTargetPoint and the 0x34-byte comparator by value on the stack).
// Callees: CThingCompare_BestForTargeting2::operator() @ 0x008098B0 (comp(first[second], first[second - 1])),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the inline CTargetPoint assignment:
//   PThing, Index, TargetingPosition, FrameRejected), CBaseIntelligentPointer() @ 0x00A01B10 + the
//   CIntelligentPointer<CThing> vtable 0x01238C6C (the CTargetPoint copy built in __push_heap's argument slot, after
//   the comparator's implicit copy: twelve dwords, the CompareInXYPlane byte, EnemyPreferenceFactor), __push_heap @
//   0x0080A480 (same instantiation), ~CBaseIntelligentPointer @ 0x00A01C10 (val.PThing, called directly).
// Callers: __pop_heap 0x0080B210 and the make_heap / sort family of the targeting candidates (STLport, /Gr TU).
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1C); the model of
//   the matched __pop_heap_aux 0x0080B430.
// Shape: STLport 4.6 `__adjust_heap`; topIndex homed in the `push ecx` slot; arguments right to left (comparator
//   copy first, then the value). Retail software-pipelines the comparator copy (two loads hoisted above `sub esp,34h`,
//   each next dword loaded before the previous store): VC7.1 only does that when it knows `__comp` is not captured
//   by the out-of-line comp(...) call that takes its address. Retail (whole-program view of 0x008098B0 and its
//   callees) knew; a declaration-only operator() makes `__comp` escaped (plain load/store pairs). The TU therefore
//   carries a same-TU `__declspec(noinline)` STAND-IN body for operator() that does not capture `this` (the real
//   0x008098B0 -- GetPItem of both points, GetThingCompareMultiplier 0x007F9E70, VectorCompare.
//   GetCompareValueForPoint 0x007F9660, `(valueA - valueB) > 0.0001f` -- passes `this` on to out-of-TU callees and
//   regrows the escape). The stand-in is not graded (the leaf is __adjust_heap); see LESSONS.md.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

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
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

#pragma auto_inline(off)
__declspec(noinline) bool CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b) const
{
    return a.Index * EnemyPreferenceFactor < b.Index * EnemyPreferenceFactor;   // STAND-IN body (see header)
}
#pragma auto_inline(on)

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x0080A480

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CTargetPoint*, int, int, CTargetPoint, CThingCompare_BestForTargeting2);
}