// std::__destroy_aux(CCountedPointer<NShape::CSubdivisionTriangle>* first, CCountedPointer<NShape::CSubdivisionTriangle>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@@std@@YIXPAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00764EE0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 007659f0 source (stdvector_Minsertoverflow_007659f0.cpp) names CCountedPointer<NShape::CSubdivisionTriangle>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@@std@@YIXPAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NShape::CSubdivisionTriangle>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NShape_CSubdivisionTriangle.h"
template void std::__destroy_aux<CCountedPointer<NShape::CSubdivisionTriangle>* >(CCountedPointer<NShape::CSubdivisionTriangle>*, CCountedPointer<NShape::CSubdivisionTriangle>*, std::__false_type const&);
