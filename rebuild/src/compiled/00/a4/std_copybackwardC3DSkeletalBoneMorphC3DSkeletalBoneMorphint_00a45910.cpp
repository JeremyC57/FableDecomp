// std::__copy_backward(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVC3DSkeletalBoneMorph@@PAV1@H@std@@YIPAVC3DSkeletalBoneMorph@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A45910, 122 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a46530 source (STL_linearinsert_00a46530.cpp) names C3DSkeletalBoneMorph; caller 00a46b50 source (stdvector_insert_00a46b50.cpp) names C3DSkeletalBoneMorph; binder ??$__copy_backward@PAVC3DSkeletalBoneMorph@@PAV1@H@std@@YIPAVC3DSkeletalBoneMorph@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DSkeletalBoneMorph.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

struct SValue0C {                                // 12-byte trivially copyable value
    long m_word0, m_word4, m_word8;
};
class C3DSkeletalBoneMorph {
public:                    // 20 bytes
    CCharString m_name;                          // +0x00
    long        m_index;                         // +0x04
    SValue0C    m_value;                         // +0x08
};
template C3DSkeletalBoneMorph* std::__copy_backward<C3DSkeletalBoneMorph*,C3DSkeletalBoneMorph*,int >(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, std::random_access_iterator_tag const&, int*);
