// _STL::__unguarded_partition(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph pivot, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) -- SGI Hoare partition around a by-value pivot; returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A461D0, length 114 bytes, `ret 18h`.
// Caller: 0x00A46ED6 (the introsort loop). Callees: 0x00A45A70 swap<C3DSkeletalBoneMorph> (this chunk, reached through
//   the inlined iter_swap), ~CCharString 0x0099EAE0 (the by-value pivot is destroyed by the callee); the comparator
//   through ebx.
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Label correction: catalogued as `Introsort_PartitionStep`.
// Shape: SGI/STLport `__unguarded_partition` verbatim in a /Gr TU (`__fastcall`: first in ecx -> edi, last in edx -> esi,
//   the 20-byte pivot and the comparator on the stack, comparator in ebx); `iter_swap(first, last)` inlines to the
//   out-of-line `swap(*first, *last)`; the two inner `while` loops are rotated (one test before, one at the bottom).
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector {                                    // retail C3DVector: memberwise user copy ctor, implicit operator=
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

namespace _STL {
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);            // 0x00A45A70

template <class _ForwardIter1, class _ForwardIter2>
inline void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for ( ; ; ) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template C3DSkeletalBoneMorph* __fastcall __unguarded_partition(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph, TBoneMorphCompare);
}