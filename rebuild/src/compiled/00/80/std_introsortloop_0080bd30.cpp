// std::__introsort_loop(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CTargetPoint*, int depth_limit, CThingCompare_BestForTargeting2 comp) -- STLport introsort recursion of the player-targeting points: partition around the median of three, recurse on the right part, loop on the left; partial_sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080BD30, 736 bytes, /Gr `__fastcall` (first ecx, last edx; the value-type
//   pointer, the depth limit and the 52-byte comparator by value on the stack, `ret 3Ch`).
// Identity: the CTargetPoint / CThingCompare_BestForTargeting2 instantiation (callers: the sort front end 0x0080C020
//   and itself); the catalog's "std_introsort_TargetPointHeapsort" is descriptive.
// Callees: CThingCompare_BestForTargeting2::operator() 0x008098B0 (same-TU stand-in here), __unguarded_partition
//   0x0080A1B0, the implicit comparator copy constructor 0x007F9730, partial_sort 0x0080BCB0 (landed), itself,
//   CBaseIntelligentPointer ctor / GetPItem / SetPItem 0x00A01B10 / 0x00A01B50 / 0x00A01B90.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first) / 2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }`
//   -- __median is inlined here (its by-value comparator copied into a local), the other comparator copies are
//   memberwise except the partial_sort one, which calls the out-of-line implicit copy constructor.
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

struct CVectorCompare_BestForTargeting {                    // Ego_r.pdb, 0x2c bytes
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
    bool operator()(const CTargetPoint& a, const CTargetPoint& b);         // 0x008098B0 (same-TU body below)
};

// Same-TU stand-in for the comparator 0x008098B0 (retail's TU holds its body: VC7.1 then knows the call leaves the
// by-value comparator alone and keeps comp.Hero in esi across it).  It reads only, like the real scorer.
#pragma auto_inline(off)
__declspec(noinline) bool CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b)
{
    return Hero != 0 && a.Index < b.Index;
}
#pragma auto_inline(on)

namespace std {
template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                                 // 0x0080A1B0
template <class _RandomAccessIter, class _Compare>
void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                             _Compare __comp);                                                       // 0x0080BCB0

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(CTargetPoint*, CTargetPoint*, CTargetPoint*, int,
                                          CThingCompare_BestForTargeting2);
}