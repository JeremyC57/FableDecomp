// std::__unguarded_partition(pair<CCharString,C3DHelperPoint>* first, pair<CCharString,C3DHelperPoint>* last, pair<CCharString,C3DHelperPoint> pivot, CCompareString<C3DHelperPoint> comp) -- quicksort partition of the helper-point map entries around a by-value pivot (name order); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F610, 115 bytes (+13 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the 20-byte pivot and the 4-byte comparator by value on the stack, `ret 18h`).
// Callees: CCharString::FastCompare(const CCharString&) const 0x009E6530 (the CCompareString "less"),
//   std::swap<pair<CCharString,C3DHelperPoint> > 0x00A6E5E0 (same chunk, out of line), CCharString::~CCharString
//   0x0099EAE0 (the by-value pivot).
// Identity: the manifest's "QuickSort_Partition_Skeleton_Small" is descriptive. 20-byte pair<CCharString,
//   C3DHelperPoint> entries of CPhysicsMesh::HelperPoints ordered by CCompareString.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the comparator inlined to the
//   FastCompare call, iter_swap reaching the out-of-line swap(*a, *b); the callee destroys its by-value pivot.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class C3DVector {                                       // user memberwise copy ctor, implicit (block) assignment
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace std {

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);              // 0x00A6E5E0

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
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

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template THelperEntry* __fastcall __unguarded_partition<THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint> >(THelperEntry*, THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint>);

}