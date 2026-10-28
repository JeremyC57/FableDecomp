// std::__make_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&), C3DSkeletalBoneMorph*, int*) -- SGI heapify: sift every parent from (len - 2) / 2 down to 0 with __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46350, length 124 bytes, `ret 0Ch`.
// Callers: 0x00A465E9 (make_heap) and 0x00A46951 (__partial_sort 0x00A46940, this chunk).
// Callees: 0x00A46040 __adjust_heap<C3DSkeletalBoneMorph*> (landed), CCharString copy ctor 0x0099EC30 (the by-value
//   element argument is built in the argument area).
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Label correction: catalogued as `MakeHeap_Generic`.
// Shape: SGI/STLport `__make_heap` verbatim in a /Gr TU (`__fastcall`: first in ecx, saved in the `push ecx` word, last in
//   edx; the comparator and the two null type-tag pointers on the stack); the length is `(last - first) / 20` by the
//   0x66666667 reciprocal multiply, computed once for the `< 2` test and `__len`; `_Tp(*(first + parent))` is built in the
//   argument area by the implicit copy ctor (string copy ctor, then the CRC and C3DVector's memberwise user copy through
//   the strength-reduced element+0x10 pointer); `while (true) { ...; if (parent == 0) return; parent--; }`.
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector {                                    // retail C3DVector: memberwise user copy ctor, implicit operator=
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A46040

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    while (true) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, TBoneMorphCompare, C3DSkeletalBoneMorph*, int*);
}