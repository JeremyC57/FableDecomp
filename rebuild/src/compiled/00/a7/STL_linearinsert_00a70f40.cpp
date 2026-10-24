// _STL::__linear_insert(pair<CCharString,C3DHelperPoint>* first, pair<CCharString,C3DHelperPoint>* last, pair<CCharString,C3DHelperPoint> val, CCompareString<C3DHelperPoint> comp) -- insertion-sort step over the helper-point map entries: a value whose name sorts before *first shifts [first, last) up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70F40, 167 bytes (+9 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the 20-byte value and the 4-byte comparator by value on the stack, `ret 18h`).
// Callees: CCharString::FastCompare 0x009E6530 (the CCompareString "less"), _STL::__copy_backward 0x00A6E7C0 (landed
//   "CopyBackward_PairCharStringValue10": first/last in ecx/edx, result, the tag reference and a null distance
//   pointer), CCharString::operator= 0x0099EFB0, CCharString::CCharString(const CCharString&) 0x0099EC30,
//   _STL::__unguarded_linear_insert 0x00A6EFD0 (same chunk), CCharString::~CCharString 0x0099EAE0 (val).
// Identity: the manifest's "Vector_ConstructElement_Pair" is descriptive. 20-byte pair<CCharString, C3DHelperPoint>
//   entries of CPhysicsMesh::HelperPoints ordered by CCompareString.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward inlined down to the out-of-line SGI __copy_backward
//   with the random-access tag homed in the dead comparator slot (`lea eax,[esp+24h]`) and a null distance pointer;
//   `*first = val` is the pair's implicit assignment (string operator=, then four words); the else arm rebuilds val in
//   the argument area (pair copy constructor); the callee destroys its by-value val on both paths.
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

class __false_type { public: __false_type() { } };
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00A6E7C0

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A6EFD0

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

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template void __fastcall __linear_insert<THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint> >(THelperEntry*, THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint>);

}