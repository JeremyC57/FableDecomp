// std::__partial_sort(pair<C3DVector,long>* first, pair<C3DVector,long>* middle, pair<C3DVector,long>* last, pair<C3DVector,long>*, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport partial sort of the first-person targeting positions: heapify [first, middle), pop every later position the comparator prefers into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080AC10, 374 bytes, /Gr `__fastcall` (first ecx, middle edx; last, the
//   value-type pointer and the 0x2c-byte comparator by value on the stack, `ret 34h`).
// Identity: the landed partial_sort front end 0x0080B1A0 names this address as its __partial_sort; the catalog's
//   "std_sort_ContextItems" is wrong (no context items: pair<C3DVector, long> targeted positions).
// Callees: make_heap(first, last, comp) 0x008097D0, CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660
//   (the inline comparator; its body and GetCompareValue 0x007F9480's are in the TU), __pop_heap(first, last, result, val, comp, int*) 0x00808BD0, sort_heap(first, last, comp)
//   0x0080A3F0 (all __fastcall, out of line).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- the comparator inline
//   (`aValue > bValue` over two GetCompareValueForPoint calls, the first result spilled to the `push ecx` word), every
//   by-value comparator copied memberwise (CompareInXYPlane cached in bl), the pair temporary built in its slot.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Y * Y; s += X * X; s += Z * Z; return s; }
    void operator*=(float f) { X *= f; Y *= f; Z *= f; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};

struct CVectorCompare_BestForTargeting {                   // Ego_r.pdb, 0x2c bytes
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValue(float sqrDist, float angleDot) const;     // 0x007F9480
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

// The comparator's two out-of-line scorers in the TU, noinline: GetCompareValueForPoint with its real (landed) body,
// GetCompareValue as a read-only STAND-IN (not the retail body 0x007F9480: that one's inline exp code never touches
// ecx, and VC7.1 would then keep the comparator's address in ecx across the second call; the variable shift makes the
// stand-in clobber ecx like any out-of-line call). Both only read, so VC7.1 knows the inline comparator leaves the
// by-value comparator alone and caches its CompareInXYPlane byte in bl across the loop as retail (LTCG) does;
// declared-only, the lane caches `last` instead and re-copies the byte through cl.
#pragma auto_inline(off)
__declspec(noinline) float CVectorCompare_BestForTargeting::GetCompareValue(float sqrDist, float angleDot) const
{
    long scale = 1L << (long)CompareInXYPlane;
    return (sqrDist * SqrDistCompare + (1.0f - angleDot) * AngleCompare) * (float)scale;
}
__declspec(noinline) float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) const
{
    C3DVector d = point - AnchorPos;
    if (CompareInXYPlane)
        d.Z = 0.0f;
    float sqrDist = d.GetLengthSquared();
    if (sqrDist > 1e-4f)
        d *= 1.0f / (float)sqrt(sqrDist);
    return GetCompareValue(sqrDist, d.Dot(PreferredLine));
}
#pragma auto_inline(on)

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy constructor
};
}

namespace {
struct CCompareTargetedPositionsForFirstPersonTargeting {  // Ego_r.pdb, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}
typedef std::pair<C3DVector, long> TargetedPosition;

namespace std {
template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x008097D0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);                                  // 0x00808BD0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0080A3F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(TargetedPosition*, TargetedPosition*, TargetedPosition*, TargetedPosition*,
                                        CCompareTargetedPositionsForFirstPersonTargeting);
}