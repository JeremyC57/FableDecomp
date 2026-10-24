// _STL::__linear_insert(pair<C3DVector, long>* first, pair<C3DVector, long>* last, pair<C3DVector, long> val, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport insertion-sort step of the first-person targeting position sort: if val scores better than *first shift [first, last) up one and put val first, else the unguarded linear insert from last
// Grade: DIFFER(229v229) -- scheduling only, in the else arm: retail software-pipelines the comparator and value copies into the argument area (two loads hoisted above `sub esp,2Ch`, each load one store ahead, the value's first two words loaded before `sub esp,10h`), ours copies load/store in order; tried the callee __unguarded_linear_insert body in the TU (the loop rows 0x00808120 / 0x00808CF0 do pipeline and match)
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808AA0, 229 bytes, /Gr `__fastcall` (first in ecx, last in edx), the
//   16-byte value and the 0x2c-byte comparator by value on the stack, `ret 3Ch`.
// Name: STLport `__linear_insert`; Ego_r.pdb's Dinkumware counterpart is `_Insertion_sort1<pair<C3DVector,long>*, ..>`
//   (donor 0x00801B70). The catalog's "std_insertion_sort_Generic" is descriptive.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660, STLport
//   __copy_backward(first, last, result, const random_access_iterator_tag&, int*) 0x00806310, __unguarded_linear_insert
//   (last, val, comp) 0x008074A0 (landed).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` (the landed 0x0080AA60 CTargetPoint twin): copy_backward inlined to
//   `__copy_backward(.., random_access_iterator_tag(), (int*)0)` whose empty tag is homed in the LIVE `val` argument
//   slot -- only with the real __copy_backward body out of line in the TU; `*first = val` is the implicit four-dword
//   copy; the else arm copies the comparator memberwise (the engine C3DVector's user copy constructor) and then the
//   value through STLport's explicit pair copy constructor. The comparator's two compare values are named float
//   locals: that is what gives retail's `fld [tmp] ; fcomp st(1) ; fstp st(0) ; test ah,41h ; jne` in this `if`.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
};

struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy constructor
};
}
typedef _STL::pair<C3DVector, long> CPositionEntry;

namespace {
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const CPositionEntry& a, const CPositionEntry& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// The real __copy_backward body (0x00806310), out of line in the TU: VC7.1 then homes the empty tag temporary in the
// live `val` argument slot as retail does.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _BI1, class _BI2> inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x008074A0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(CPositionEntry*, CPositionEntry*, CPositionEntry,
                                         CCompareTargetedPositionsForFirstPersonTargeting);
}
