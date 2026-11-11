// std::__uninitialized_fill_n(std::pair<C3DVector,float>* first, unsigned int n, std::pair<C3DVector,float> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VC3DVector@@M@std@@IU12@@std@@YIPAU?$pair@VC3DVector@@M@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084EF90, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0084f5e0 std::vector<std::pair<C3DVector, float>, std::allocator<std::pair<C3DVector, float>>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAU?$pair@VC3DVector@@M@std@@IU12@@std@@YIPAU?$pair@VC3DVector@@M@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DVector,float>.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template std::pair<C3DVector,float>* std::__uninitialized_fill_n<std::pair<C3DVector,float>*,unsigned int,std::pair<C3DVector,float> >(std::pair<C3DVector,float>*, unsigned int, std::pair<C3DVector,float> const&, std::__false_type const&);
