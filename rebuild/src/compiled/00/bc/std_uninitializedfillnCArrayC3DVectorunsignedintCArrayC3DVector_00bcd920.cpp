// std::uninitialized_fill_n(CArray<C3DVector>* first, unsigned int n, CArray<C3DVector> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CArray@VC3DVector@@@@IV1@@std@@YIPAV?$CArray@VC3DVector@@@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCD920, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00bcdca0 std::vector<CArray<C3DVector>, std::allocator<CArray<C3DVector>>>::_M_fill_insert; callee 006f0810 std::vector<C3DVector>::vector; binder ??$uninitialized_fill_n@PAV?$CArray@VC3DVector@@@@IV1@@std@@YIPAV?$CArray@VC3DVector@@@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CArray<C3DVector>.
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
template CArray<C3DVector>* std::uninitialized_fill_n<CArray<C3DVector>*,unsigned int,CArray<C3DVector> >(CArray<C3DVector>*, unsigned int, CArray<C3DVector> const&);
