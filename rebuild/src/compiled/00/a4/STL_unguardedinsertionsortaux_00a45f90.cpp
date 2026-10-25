// std::__unguarded_insertion_sort_aux(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph*, bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&) comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A45F90, 83 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00A45242 (C3DSkeletalMorphTarget::Initialise 0x00A45150, landed: the bone morphs sorted by a __fastcall
//   unsigned-CRC predicate), 0x00A46257, 0x00A46916.
// Callees: __unguarded_linear_insert(last, val, comp) 0x00A45B00, CCharString copy constructor 0x0099EC30.
// Element: Ego_r.pdb C3DSkeletalBoneMorph (20 bytes): CCharString BoneName +0x0, unsigned long BoneNameCRC +0x4, C3DVector
//   Scaling +0x8 (memberwise user copy constructor); the comparator is a function pointer, so the __fastcall callee takes
//   it in edx (the by-value element does not fit a register), as the landed __unguarded_partition 0x00A461D0.
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

class C3DSkeletalBoneMorph {                    // Ego_r.pdb, 20 bytes
public:
    CCharString   BoneName;                     // +0x00
    unsigned long BoneNameCRC;                  // +0x04
    C3DVector     Scaling;                      // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A45B00

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, TBoneMorphCompare);
}