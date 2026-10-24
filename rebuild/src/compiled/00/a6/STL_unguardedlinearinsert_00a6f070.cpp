// _STL::__unguarded_linear_insert(pair<CCharString,C3DDummyObject>* last, pair<CCharString,C3DDummyObject> val, CCompareString<C3DDummyObject> comp) -- insertion-sort inner step over the dummy-object map entries: shift entries whose name sorts after val's up one slot, then drop val in
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F070, 116 bytes (+12 int3), /Gr `__fastcall` (last in ecx; the
//   56-byte value and the 4-byte comparator by value on the stack, `ret 3Ch`).
// Callees: CCharString::FastCompare(const CCharString&) const 0x009E6530 (the CCompareString "less"),
//   CCharString::operator=(const CCharString&) 0x0099EFB0, CCharString::~CCharString 0x0099EAE0 (the by-value val).
// Identity: the manifest's "std_fill_StringDummyObjectPair_Radial" is descriptive. 56-byte pair<CCharString,
//   C3DDummyObject> entries of CPhysicsMesh::DummyObjects ordered by CCompareString (FastCompare), as the
//   __copy_backward 0x00A6E840 / __uninitialized_copy 0x00A6EE30 of this TU.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- the comparator inlined to `val.first.FastCompare(next->first)`; each assignment is the pair's implicit
//   operator= (string operator=, then C3DDummyObject's 13-dword `rep movsd` from a strength-reduced src+4 pointer in
//   ebp); the callee destroys its by-value val before `ret 3Ch`.
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

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const _STL::pair<CCharString, V>& a, const _STL::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace _STL {

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

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template void __fastcall __unguarded_linear_insert<TDummyEntry*, TDummyEntry, CCompareString<C3DDummyObject> >(TDummyEntry*, TDummyEntry, CCompareString<C3DDummyObject>);

}