// std::__uninitialized_fill_n(std::pair<C3DVector,long>* first, unsigned int n, std::pair<C3DVector,long> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VC3DVector@@J@std@@IU12@@std@@YIPAU?$pair@VC3DVector@@J@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA8C0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007eaa40 std::vector<std::pair<C3DVector, long>, std::allocator<std::pair<C3DVector, long>>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAU?$pair@VC3DVector@@J@std@@IU12@@std@@YIPAU?$pair@VC3DVector@@J@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DVector,long>.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template std::pair<C3DVector,long>* std::__uninitialized_fill_n<std::pair<C3DVector,long>*,unsigned int,std::pair<C3DVector,long> >(std::pair<C3DVector,long>*, unsigned int, std::pair<C3DVector,long> const&, std::__false_type const&);
