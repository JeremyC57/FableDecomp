// std::__uninitialized_copy(std::pair<CCharString,C3DVector>* first, std::pair<CCharString,C3DVector>* last, std::pair<CCharString,C3DVector>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VCCharString@@VC3DVector@@@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@VC3DVector@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077E280, 90 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0077ef30 source (stdvector_Minsertoverflow_0077ef30.cpp) names std::pair<CCharString,C3DVector>; binder ??$__uninitialized_copy@PAU?$pair@VCCharString@@VC3DVector@@@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@VC3DVector@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,C3DVector>.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};
template std::pair<CCharString,C3DVector>* std::__uninitialized_copy<std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>* >(std::pair<CCharString,C3DVector>*, std::pair<CCharString,C3DVector>*, std::pair<CCharString,C3DVector>*, std::__false_type const&);
