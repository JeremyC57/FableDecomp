// std::__destroy_aux(CCountedPointer<NShape::CShape>* first, CCountedPointer<NShape::CShape>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCShape@NShape@@@@@std@@YIXPAV?$CCountedPointer@VCShape@NShape@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00764C40, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00762110 source (CTCShapeManager_CTCShapeManager_00762110.cpp) names CCountedPointer<NShape::CShape>; caller 007656d0 source (stdvector_Minsertoverflow_007656d0.cpp) names CCountedPointer<NShape::CShape>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCShape@NShape@@@@@std@@YIXPAV?$CCountedPointer@VCShape@NShape@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NShape::CShape>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NShape {
class CShape;
}
template void std::__destroy_aux<CCountedPointer<NShape::CShape>* >(CCountedPointer<NShape::CShape>*, CCountedPointer<NShape::CShape>*, std::__false_type const&);
