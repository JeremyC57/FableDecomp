// std::__uninitialized_copy(CKeyCamera* first, CKeyCamera* last, CKeyCamera* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCKeyCamera@@PAV1@@std@@YIPAVCKeyCamera@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075F3E0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0075f860 std::vector<CTCCameraPointScriptedSpline::CKeyCamera>::_M_insert_overflow; callee 0075f1f0 CTCCameraPointScriptedSpline::CKeyCamera::CKeyCamera; binder ??$__uninitialized_copy@PAVCKeyCamera@@PAV1@@std@@YIPAVCKeyCamera@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CKeyCamera.
#include "stlport/fable_containers.h"
class CKeyCamera {
public:
    unsigned char _body[0x38];
    CKeyCamera(const CKeyCamera& other);           // 0x0075F1F0
};
template CKeyCamera* std::__uninitialized_copy<CKeyCamera*,CKeyCamera* >(CKeyCamera*, CKeyCamera*, CKeyCamera*, std::__false_type const&);
