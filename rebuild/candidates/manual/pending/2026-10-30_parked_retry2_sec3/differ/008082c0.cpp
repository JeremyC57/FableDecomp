// std::__adjust_heap(pair<C3DVector, long>* first, int holeIndex, int len, pair<C3DVector, long> value, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport heap sift-down of the first-person targeting positions (best score at the root): move the better child up until the hole reaches a leaf, then push `value` back up
// Grade: DIFFER(317v317) -- scheduling only, in the by-value comparator copy for __push_heap: retail preloads the first two comparator words (edx, ecx) before `sub esp,2Ch` and keeps the copy one load ahead; ours loads one word before the sub and copies load/store in lockstep (the 16-byte value copy that follows IS pipelined in both). Tried: C3DVector without its user copy ctor (worse: ebx/edi/ebp reshuffle). The landed __make_heap 0x00808CF0 of this family gets the pipelined copy from the same model.; this sweep: expression comparator (313, the if-compare shape breaks)
// Leaf: ??$__adjust_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008082C0, 317 bytes, /Gr __fastcall (first in ecx, holeIndex in edx; len, the
//   16-byte value and the 0x2C-byte comparator on the stack), `ret 40h`.
// Identity (proven): the landed __make_heap 0x00808CF0 of this family calls it as __adjust_heap(first, parent, len, value, comp)
//   (its binding is this template); the body is STLport 4.6 __adjust_heap with the comparator inlined and a closing
//   __push_heap(first, hole, top, value, comp) 0x00807620; Ego_r.pdb's Dinkumware twin is
//   ??$_Adjust_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x58ed6966@@@std@@
//   (donor 0x00801CA0). The anonymous-namespace hash is per build (retail ?A0x58ed6966), so the Leaf stops at `?A0x`.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (x2, the comparator's
//   `aValue > bValue`), std::__push_heap<pair<C3DVector, long>*, int, pair<C3DVector, long>, CCompareTargeted..> 0x00807620.
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (class, 0x2C: one CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes; the landed family model 0x00808CF0 (C3DVector user copy ctor, STLport explicit pair copy).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
};

class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy constructor
};
}

namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {
public:
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);              // 0x00807620

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

template void __fastcall __adjust_heap(pair<C3DVector, long>*, int, int, pair<C3DVector, long>,
                                       CCompareTargetedPositionsForFirstPersonTargeting);
}
