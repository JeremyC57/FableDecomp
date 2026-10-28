// std::__push_heap(C3DSkeletalBoneMorph* first, int holeIndex, int topIndex, C3DSkeletalBoneMorph val, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) -- SGI heap sift-up: move parents down while comp(parent, val), then store val in the hole
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45BC0, length 179 bytes (+13 int3 pad), `ret 1Ch`.
// Caller: 0x00A46125 in __adjust_heap<C3DSkeletalBoneMorph*> 0x00A46040 (landed). Callees: CCharString::operator=
//   0x0099EFB0, ~CCharString 0x0099EAE0 (the by-value val is destroyed by the callee); the comparator through the stack
//   slot (`call [esp+2Ch]`, `__fastcall` with both element addresses in ecx/edx).
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC unsigned long +4, Scaling
//   C3DVector +8.
// Shape: SGI/STLport `__push_heap` verbatim in a /Gr TU (`__fastcall`: first in ecx -> ebp, holeIndex in edx -> edi,
//   topIndex, the 20-byte val and the comparator on the stack); `(hole - 1) / 2` is `cdq ; sub ; sar`; the element's
//   implicit operator= runs CCharString's out-of-line operator=, the CRC word, then the Scaling sub-object copied through
//   re-based pointers (`add ebx,8 ; add edi,8`).
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap(C3DSkeletalBoneMorph*, int, int, C3DSkeletalBoneMorph, TBoneMorphCompare);
}