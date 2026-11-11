// std::__uninitialized_copy(std::vector<C3DVector,std::allocator<C3DVector> >* first, std::vector<C3DVector,std::allocator<C3DVector> >* last, std::vector<C3DVector,std::allocator<C3DVector> >* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@PAV12@@std@@YIPAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@0@PAV10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00764970, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: callee 006f0810 std::vector<C3DVector>::vector; binder ??$__uninitialized_copy@PAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@PAV12@@std@@YIPAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@0@PAV10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::vector<C3DVector,std::allocator<C3DVector>>.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template std::vector<C3DVector,std::allocator<C3DVector> >* std::__uninitialized_copy<std::vector<C3DVector,std::allocator<C3DVector> >*,std::vector<C3DVector,std::allocator<C3DVector> >* >(std::vector<C3DVector,std::allocator<C3DVector> >*, std::vector<C3DVector,std::allocator<C3DVector> >*, std::vector<C3DVector,std::allocator<C3DVector> >*, std::__false_type const&);
