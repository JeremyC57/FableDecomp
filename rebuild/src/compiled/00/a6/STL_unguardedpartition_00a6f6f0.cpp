// std::__unguarded_partition(pair<CCharString,C3DDummyObject>* first, pair<CCharString,C3DDummyObject>* last, pair<CCharString,C3DDummyObject> pivot, CCompareString<C3DDummyObject> comp) -- quicksort partition of the dummy-object map entries around a by-value pivot (name order); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F6F0, 198 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   56-byte pivot and the 4-byte comparator by value on the stack, `ret 3Ch`).
// Callees: CCharString::FastCompare(const CCharString&) const 0x009E6530 (the CCompareString "less"),
//   CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::operator= 0x0099EFB0 (x2),
//   CCharString::~CCharString 0x0099EAE0 (the swap temporary and the by-value pivot).
// Identity: the manifest's "QuickSort_Partition_CCharString" is descriptive. 56-byte pair<CCharString,
//   C3DDummyObject> entries of CPhysicsMesh::DummyObjects ordered by CCompareString.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- here (unlike the 20-byte twin 0x00A6F610,
//   which calls swap out of line) the swap is inlined: a 0x38-byte frame temporary built by STLport's explicit pair
//   copy constructor (string copy + 13-dword `rep movsd`), two implicit pair assignments, the temporary's destructor.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
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
inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

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

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template TDummyEntry* __fastcall __unguarded_partition<TDummyEntry*, TDummyEntry, CCompareString<C3DDummyObject> >(TDummyEntry*, TDummyEntry*, TDummyEntry, CCompareString<C3DDummyObject>);

}