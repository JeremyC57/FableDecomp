// _STL::__linear_insert(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph val, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) -- SGI insertion step: if val sorts before *first shift [first, last) up one and store val at first, else the unguarded insert from last
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46530, length 169 bytes, `ret 18h`.
// Caller: 0x00A466CF __insertion_sort<C3DSkeletalBoneMorph*> (landed 0x00A46680 calls this address as __linear_insert).
// Callees: 0x00A45910 __copy_backward(first, last, result, const random_access_iterator_tag&, int*) (landed), 0x00A45B00
//   __unguarded_linear_insert (this chunk), CCharString copy ctor 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0.
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Label correction: catalogued as `Vector_InsertConditionalShift_Decal`.
// Shape: SGI/STLport `__linear_insert` in a /Gr TU (`__fastcall`: first in ecx -> esi, last in edx -> edi, the 20-byte
//   val and the comparator on the stack, comparator kept in ebx); `copy_backward(first, last, last + 1)` inlines down to
//   the out-of-line `__copy_backward` with a `random_access_iterator_tag()` temporary homed in the dead comparator slot
//   and a null Distance*; the by-value val is re-copied into the argument area for the unguarded insert (implicit copy:
//   string copy ctor, then four dwords) and destroyed by this function on both paths. C3DVector needs its memberwise user
//   copy ctor (`X(v.X), Y(v.Y), Z(v.Z)`): with the implicit one VC copies Scaling through an `add esi,8` pointer (171v169).
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*);   // 0x00A45910

template <class _BI1, class _BI2>
inline _BI2 __fastcall copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A45B00

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

template void __fastcall __linear_insert(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph, TBoneMorphCompare);
}