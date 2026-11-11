// std::__destroy_aux(CCountedPointer<CGuiVarTransferBase>* first, CCountedPointer<CGuiVarTransferBase>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCGuiVarTransferBase@@@@@std@@YIXPAV?$CCountedPointer@VCGuiVarTransferBase@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF0D80, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00af0ef0 source (stdvectorCCountedPointerNParticleEngineCParticleSystem_reserve_00af0ef0.cpp) names CCountedPointer<CGuiVarTransferBase>; caller 00af0fa0 source (stdvector_Minsertoverflow_00af0fa0.cpp) names CCountedPointer<CGuiVarTransferBase>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCGuiVarTransferBase@@@@@std@@YIXPAV?$CCountedPointer@VCGuiVarTransferBase@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CGuiVarTransferBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CGuiVarTransferBase;
template void std::__destroy_aux<CCountedPointer<CGuiVarTransferBase>* >(CCountedPointer<CGuiVarTransferBase>*, CCountedPointer<CGuiVarTransferBase>*, std::__false_type const&);
