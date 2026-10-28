// std::__introsort_loop(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph*, int depth_limit, TBoneMorphCompare comp) -- STLport introsort recursion over the 20-byte bone morphs: partition around the median of three, recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@PAVC3DSkeletalBoneMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46E00, 303 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   value-type pointer, the depth limit and the comparator pointer on the stack, `ret 0Ch`).
// Identity: std::__introsort_loop<C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph, int, bool (__fastcall*)(const
//   C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)>. Callers: its own recursion 0x00A46EED, the sort called from
//   C3DSkeletalMorphTarget::Initialise 0x00A45219 and 0x00A46FFE; callees are this element's landed siblings
//   __unguarded_partition 0x00A461D0 and __partial_sort 0x00A46940 (both landed with this element and comparator). The
//   comparator is Ego_r.pdb `static bool __fastcall C3DSkeletalBoneMorph::CompareCRCs(const C3DSkeletalBoneMorph&,
//   const C3DSkeletalBoneMorph&)` (the PDB also lists _Median / _Unguarded_partition / _Make_heap over exactly this
//   element with `P6I_NABV1@0@Z`). The catalog's "Introsort_Partition_With_Predicate" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (landed twin 0x009327F0): __median inlined (five compares through the stack
//   comparator), the pivot copy-constructed straight into the by-value argument slot of __unguarded_partition.
class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {                                // Ego_r.pdb, 20 bytes
public:
    CCharString   BoneName;                                 // +0x00
    unsigned long BoneNameCRC;                              // +0x04
    C3DVector     Scaling;                                  // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

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
                                                   _Compare __comp);   // 0x00A461D0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A46940
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (C3DSkeletalBoneMorph*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
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

template void __fastcall __introsort_loop(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, int,
                                          TBoneMorphCompare);
}