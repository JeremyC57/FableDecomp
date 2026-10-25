// std::__unguarded_insertion_sort_aux(pair<CCharString, C3DHelperPoint>* first, pair<CCharString, C3DHelperPoint>* last, pair<CCharString, C3DHelperPoint>*, CCompareString<C3DHelperPoint> comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6F8B0, 84 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00A70007 and 0x00A72276 (the __final_insertion_sort of the sort 0x00A735A0 that CPhysicsMesh::
//   GetHelperDataFromFileData 0x00A6D0E0 runs).
// Callees: __unguarded_linear_insert(last, val, comp) 0x00A6EFD0, CCharString copy constructor 0x0099EC30.
// Element: STLport pair<CCharString, C3DHelperPoint> (20 bytes; Ego_r.pdb C3DHelperPoint = C3DVector Position + long BoneIndex)
//   -- the named helper points; Ego_r.pdb sorts exactly this pair with CCompareString<C3DHelperPoint> (an empty functor
//   comparing the names; its linear insert 0x00A6EFD0 calls the string compare).
// Label: catalog "vector<...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   by-value element is copy-constructed in its outgoing argument slot (the copy expanded inline); the comparator's
//   operator() stays declaration-only (never called here).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class C3DVector {                               // retail C3DVector: memberwise user copy constructor
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
    float Z;                                    // +0x8
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                          // Ego_r.pdb, 16 bytes
public:
    C3DVector Position;                         // +0x0
    long      BoneIndex;                        // +0xC
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

typedef std::pair<CCharString, C3DHelperPoint> CHelperPointEntry;          // 20 bytes
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const;
};
typedef CCompareString<C3DHelperPoint> CHelperPointNameLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A6EFD0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CHelperPointEntry*, CHelperPointEntry*, CHelperPointEntry*, CHelperPointNameLess);
}