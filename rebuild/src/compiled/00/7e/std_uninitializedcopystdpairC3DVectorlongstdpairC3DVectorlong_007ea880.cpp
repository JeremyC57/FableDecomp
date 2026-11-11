// std::__uninitialized_copy(std::pair<C3DVector,long>* first, std::pair<C3DVector,long>* last, std::pair<C3DVector,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIPAU?$pair@VC3DVector@@J@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA880, 56 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007eaa40 std::vector<std::pair<C3DVector, long>, std::allocator<std::pair<C3DVector, long>>>::_M_insert_overflow; caller 007eaa40 std::vector<std::pair<C3DVector, long>, std::allocator<std::pair<C3DVector, long>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIPAU?$pair@VC3DVector@@J@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DVector,long>.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template std::pair<C3DVector,long>* std::__uninitialized_copy<std::pair<C3DVector,long>*,std::pair<C3DVector,long>* >(std::pair<C3DVector,long>*, std::pair<C3DVector,long>*, std::pair<C3DVector,long>*, std::__false_type const&);
