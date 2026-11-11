// std::__uninitialized_copy(CArray<C3DVector>* first, CArray<C3DVector>* last, CArray<C3DVector>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CArray@VC3DVector@@@@PAV1@@std@@YIPAV?$CArray@VC3DVector@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCD8A0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bcdb70 std::vector<CArray<C3DVector>, std::allocator<CArray<C3DVector>>>::_M_insert_overflow; caller 00bcdca0 std::vector<CArray<C3DVector>, std::allocator<CArray<C3DVector>>>::_M_fill_insert; caller 00bcdca0 std::vector<CArray<C3DVector>, std::allocator<CArray<C3DVector>>>::_M_fill_insert; binder ??$__uninitialized_copy@PAV?$CArray@VC3DVector@@@@PAV1@@std@@YIPAV?$CArray@VC3DVector@@@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CArray<C3DVector>.
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
template CArray<C3DVector>* std::__uninitialized_copy<CArray<C3DVector>*,CArray<C3DVector>* >(CArray<C3DVector>*, CArray<C3DVector>*, CArray<C3DVector>*, std::__false_type const&);
