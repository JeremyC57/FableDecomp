// std::__uninitialized_copy(std::pair<C3DVector,CCharString>* first, std::pair<C3DVector,CCharString>* last, std::pair<C3DVector,CCharString>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VC3DVector@@VCCharString@@@std@@PAU12@@std@@YIPAU?$pair@VC3DVector@@VCCharString@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006555CE, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00656d5e _global::std_vector_insert_pair_C3DVector_CCharString_range; caller 00656d5e _global::std_vector_insert_pair_C3DVector_CCharString_range; callee 00654dd4 std::pair<C3DVector, CCharString>::pair; binder ??$__uninitialized_copy@PAU?$pair@VC3DVector@@VCCharString@@@std@@PAU12@@std@@YIPAU?$pair@VC3DVector@@VCCharString@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DVector,CCharString>.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

template std::pair<C3DVector,CCharString>* std::__uninitialized_copy<std::pair<C3DVector,CCharString>*,std::pair<C3DVector,CCharString>* >(std::pair<C3DVector,CCharString>*, std::pair<C3DVector,CCharString>*, std::pair<C3DVector,CCharString>*, std::__false_type const&);
