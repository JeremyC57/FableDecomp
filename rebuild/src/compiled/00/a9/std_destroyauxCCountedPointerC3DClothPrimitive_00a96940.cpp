// std::__destroy_aux(CCountedPointer<C3DClothPrimitive>* first, CCountedPointer<C3DClothPrimitive>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VC3DClothPrimitive@@@@@std@@YIXPAV?$CCountedPointer@VC3DClothPrimitive@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A96940, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a975e0 std::vector<CCountedPointer<C3DClothPrimitive>, std::allocator<CCountedPointer<C3DClothPrimitive>>>::reserve; caller 00a98540 std::vector<CCountedPointer<C3DClothPrimitive>, std::allocator<CCountedPointer<C3DClothPrimitive>>>::resize; binder ??$__destroy_aux@PAV?$CCountedPointer@VC3DClothPrimitive@@@@@std@@YIXPAV?$CCountedPointer@VC3DClothPrimitive@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DClothPrimitive>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class C3DClothPrimitive;
template void std::__destroy_aux<CCountedPointer<C3DClothPrimitive>* >(CCountedPointer<C3DClothPrimitive>*, CCountedPointer<C3DClothPrimitive>*, std::__false_type const&);
