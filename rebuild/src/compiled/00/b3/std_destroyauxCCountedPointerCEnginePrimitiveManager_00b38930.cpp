// std::__destroy_aux(CCountedPointer<CEnginePrimitiveManager>* first, CCountedPointer<CEnginePrimitiveManager>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCEnginePrimitiveManager@@@@@std@@YIXPAV?$CCountedPointer@VCEnginePrimitiveManager@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38930, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b33520 source (CEnginePrimitiveRenderer_CEnginePrimitiveRenderer_00b33520.cpp) names CCountedPointer<CEnginePrimitiveManager>; caller 00b38bb0 source (stdvector_Minsertoverflow_00b38bb0.cpp) names CCountedPointer<CEnginePrimitiveManager>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCEnginePrimitiveManager@@@@@std@@YIXPAV?$CCountedPointer@VCEnginePrimitiveManager@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CEnginePrimitiveManager>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveManager.h"
template void std::__destroy_aux<CCountedPointer<CEnginePrimitiveManager>* >(CCountedPointer<CEnginePrimitiveManager>*, CCountedPointer<CEnginePrimitiveManager>*, std::__false_type const&);
