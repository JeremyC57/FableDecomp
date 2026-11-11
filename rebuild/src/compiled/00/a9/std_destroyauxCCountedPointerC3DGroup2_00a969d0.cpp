// std::__destroy_aux(CCountedPointer<C3DGroup2>* first, CCountedPointer<C3DGroup2>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VC3DGroup2@@@@@std@@YIXPAV?$CCountedPointer@VC3DGroup2@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A969D0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a97690 std::vector<CCountedPointer<C3DGroup2>, std::allocator<CCountedPointer<C3DGroup2>>>::operator=; caller 00a97690 std::vector<CCountedPointer<C3DGroup2>, std::allocator<CCountedPointer<C3DGroup2>>>::operator=; caller 00a985a0 std::vector<CCountedPointer<C3DGroup2>, std::allocator<CCountedPointer<C3DGroup2>>>::resize; binder ??$__destroy_aux@PAV?$CCountedPointer@VC3DGroup2@@@@@std@@YIXPAV?$CCountedPointer@VC3DGroup2@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DGroup2>.
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DGROUP2
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DGroup2.h"
template void std::__destroy_aux<CCountedPointer<C3DGroup2>* >(CCountedPointer<C3DGroup2>*, CCountedPointer<C3DGroup2>*, std::__false_type const&);
