// std::__unguarded_linear_insert(C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph val, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) -- SGI insertion step without a lower bound: shift elements up while comp(val, *prev), then store val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45B00, length 145 bytes, `ret 14h`.
// Callers: 0x00A45FCD (__unguarded_insertion_sort) and 0x00A465C5 (__linear_insert 0x00A46530, this chunk).
//   Callees: CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0 (the by-value val is destroyed by the callee);
//   the comparator through ebp.
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Label correction: catalogued as `Vector_Insert_ConditionalShift`; the body is SGI/STLport `__unguarded_linear_insert`.
// Shape: /Gr TU `__fastcall` template: last in ecx -> esi, the comparator (the first 4-byte argument after the 20-byte
//   val) in edx -> ebp, val on the stack; `next` is `last - 1` (edi) with the strength-reduced next+8 pointer in ebx for
//   the Scaling block copy; the element's implicit operator= (string operator=, CRC word, Scaling sub-object).
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector {
public:
    float X, Y, Z;
};

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

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

template void __fastcall __unguarded_linear_insert(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph, TBoneMorphCompare);
}