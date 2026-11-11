// std::__uninitialized_copy(QuadricOptimiserInternals::COptimisedFace* first, QuadricOptimiserInternals::COptimisedFace* last, QuadricOptimiserInternals::COptimisedFace* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCOptimisedFace@QuadricOptimiserInternals@@PAV12@@std@@YIPAVCOptimisedFace@QuadricOptimiserInternals@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD1990, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ad4100 std::vector<QuadricOptimiserInternals::COptimisedFace, std::allocator<QuadricOptimiserInternals::COptimisedFace>>::_M_insert_overflow; callee 00ad0100 COptimisedFace::COptimisedFace; binder ??$__uninitialized_copy@PAVCOptimisedFace@QuadricOptimiserInternals@@PAV12@@std@@YIPAVCOptimisedFace@QuadricOptimiserInternals@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over QuadricOptimiserInternals::COptimisedFace.
#include "stlport/fable_containers.h"
namespace QuadricOptimiserInternals {
class COptimisedFace {
public:
    unsigned char _body[0x48];
    COptimisedFace(const COptimisedFace& other);           // 0x00AD0100
};
}
template QuadricOptimiserInternals::COptimisedFace* std::__uninitialized_copy<QuadricOptimiserInternals::COptimisedFace*,QuadricOptimiserInternals::COptimisedFace* >(QuadricOptimiserInternals::COptimisedFace*, QuadricOptimiserInternals::COptimisedFace*, QuadricOptimiserInternals::COptimisedFace*, std::__false_type const&);
