// std::swap(C3DSkeletalBoneMorph& a, C3DSkeletalBoneMorph& b) -- exchange two bone morphs through a temporary copy
// Leaf: ??$swap@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45A70, length 141 bytes (+3 int3 pad), `ret`.
// Callers: 0x00A461D0 __unguarded_partition (this chunk, through the inlined iter_swap) and the `jmp` at 0x00A45F80
//   (the out-of-line iter_swap). Callees: CCharString copy ctor 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0.
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Shape: STLport `swap` verbatim (`_Tp __tmp = __a; __a = __b; __b = __tmp;`) in a /Gr TU (`__fastcall`: a in ecx -> ebx,
//   b in edx -> edi); the copy runs the string copy ctor then C3DVector's memberwise user copy (four dwords into the
//   frame), each assignment the string operator=, the CRC word and the Scaling sub-object through a re-based pointer
//   (`lea esi,[ebx+8]`, `lea ebx,[edi+8]`); the temporary's string is destroyed last.
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

#include "engine/C3DVector.h"

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
};

namespace std {
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap(C3DSkeletalBoneMorph&, C3DSkeletalBoneMorph&);
}