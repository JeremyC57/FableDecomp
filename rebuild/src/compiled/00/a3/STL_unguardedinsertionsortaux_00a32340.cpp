// std::__unguarded_insertion_sort_aux(pair<unsigned long, C3DHelperPoint>* first, pair<unsigned long, C3DHelperPoint>* last, pair<unsigned long, C3DHelperPoint>*, CKeyPairCompareLess<unsigned long, C3DHelperPoint> comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A32340, 74 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00A32A67 and 0x00A34756 (the C3DMeshStats helper-point sort; the TU's landed __unguarded_partition 0x00A329B0
//   is the same pair and comparator).
// Callees: __unguarded_linear_insert(last, val, comp) 0x00A31280.
// Element: STLport pair<unsigned long, C3DHelperPoint> (20 bytes; Ego_r.pdb C3DHelperPoint = C3DVector Position + long
//   BoneIndex, C3DVector with a memberwise user copy constructor) -- five dword moves.
// Label: catalog "vector<...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   by-value element is copy-constructed in its outgoing argument slot (the copy expanded inline); the comparator's
//   operator() stays declaration-only (never called here).
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

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<unsigned long, C3DHelperPoint> CHelperPointIDEntry;     // 20 bytes
typedef CKeyPairCompareLess<unsigned long, C3DHelperPoint> CHelperPointIDLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A31280

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CHelperPointIDEntry*, CHelperPointIDEntry*, CHelperPointIDEntry*, CHelperPointIDLess);
}