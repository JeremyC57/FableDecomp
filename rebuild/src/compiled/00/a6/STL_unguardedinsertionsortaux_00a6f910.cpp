// std::__unguarded_insertion_sort_aux(std::pair<CCharString, C3DDummyObject>* first, std::pair<CCharString, C3DDummyObject>* last, std::pair<CCharString, C3DDummyObject>*, CCompareString<C3DDummyObject> comp) -- the unguarded tail of an insertion sort of named dummy objects by name
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@VCCharString@@VC3DDummyObject@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F910, 68 bytes, `__fastcall` (first ecx, last edx; the unused
//   T* tag and the 4-byte comparator on the stack), `ret 8`.
// Identity: the loop passes each element by value (0x38 bytes: CCharString copy 0x0099EC30 + 13 dwords) with the
//   comparator to 0x00A6F070, which is STLport __unguarded_linear_insert(last, T val, comp) (`next = last - 1;
//   while (comp(val, *next)) { *last = *next; last = next--; } *last = val;`, CCharString::operator= 0x0099EFB0 +
//   13-dword copies, the by-value temporary destroyed by ~CCharString 0x0099EAE0); its comparator calls
//   CCharString::FastCompare 0x009E6530 on the two keys, the landed CCompareString shape (STL_partialsort_00cbd0f0).
//   The 56-byte element is pair<CCharString, C3DDummyObject> (Ego_r.pdb C3DDummyObject {CMatrix3x4 Transform;
//   long BoneIndex} = 52 bytes), as the manifest's type names say; the manifest's "fill" is propagated.
//   Two callers, 0x00A70017 and 0x00A722DA.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, __unguarded_linear_insert @ 0x00A6F070.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` -- the comparator is pushed
//   first (right-to-left), then the element copy is built in the outgoing argument area.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;    // 0x009E6530
};
class CMatrix3x4 { public: float M[12]; };               // 48 bytes
class C3DDummyObject {                                   // 52 bytes
public:
    CMatrix3x4 Transform;                                // +0x00
    long       BoneIndex;                                // +0x30
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class V> class CCompareString {                // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const
    {
        return a.first.FastCompare(b.first);
    }
};

typedef std::pair<CCharString, C3DDummyObject> CDummyPair;   // 56 bytes
typedef CCompareString<C3DDummyObject> CDummyLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A6F070

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template void __fastcall std::__unguarded_insertion_sort_aux<CDummyPair*, CDummyPair, CDummyLess>(CDummyPair*, CDummyPair*, CDummyPair*, CDummyLess);