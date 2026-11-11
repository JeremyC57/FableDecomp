// std::__uninitialized_copy(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DSkeletalBoneMorph@@PAV1@@std@@YIPAVC3DSkeletalBoneMorph@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088ABC0, 96 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0088b4f0 source (stdvector_Minsertoverflow_0088b4f0.cpp) names C3DSkeletalBoneMorph; binder ??$__uninitialized_copy@PAVC3DSkeletalBoneMorph@@PAV1@@std@@YIPAVC3DSkeletalBoneMorph@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DSkeletalBoneMorph.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

struct SValue0C {                                // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};
class C3DSkeletalBoneMorph {
public:                          // 20 bytes
    CCharString m_name;                          // +0x00
    long        m_value;                         // +0x04
    SValue0C    m_record;                        // +0x08
};
template C3DSkeletalBoneMorph* std::__uninitialized_copy<C3DSkeletalBoneMorph*,C3DSkeletalBoneMorph* >(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, std::__false_type const&);
