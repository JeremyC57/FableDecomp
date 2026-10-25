// std::__unguarded_linear_insert(pair<CCharString,C3DHelperPoint>* last, pair<CCharString,C3DHelperPoint> val, CCompareString<C3DHelperPoint> comp) -- insertion-sort inner step over the helper-point map entries: shift entries whose name sorts after val's up one slot, then drop val in
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6EFD0, 146 bytes (+14 int3), /Gr `__fastcall` (last in ecx; the
//   20-byte value and the 4-byte comparator by value on the stack, `ret 18h`).
// Callees: CCharString::FastCompare(const CCharString&) const 0x009E6530 (the CCompareString "less"),
//   CCharString::operator=(const CCharString&) 0x0099EFB0, CCharString::~CCharString 0x0099EAE0 (the by-value val).
// Identity: the manifest's "std_fill_Pair_WithComparison" is descriptive. 20-byte pair<CCharString, C3DHelperPoint>
//   entries of CPhysicsMesh::HelperPoints ordered by CCompareString (FastCompare), as the swap 0x00A6E5E0 and the
//   partition 0x00A6F610 of this TU.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- the comparator inlined to `val.first.FastCompare(next->first)`; each assignment is the pair's implicit
//   operator= (string operator=, then the trivially assigned C3DHelperPoint as four words through a strength-reduced
//   src+4 pointer in ebx); the callee destroys its by-value val (string destructor) before `ret 18h`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template void __fastcall __unguarded_linear_insert<THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint> >(THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint>);

}