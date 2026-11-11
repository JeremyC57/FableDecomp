// std::__uninitialized_fill_n(CCountedPointer<CGuiVarTransferBase>* first, unsigned int n, CCountedPointer<CGuiVarTransferBase> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCGuiVarTransferBase@@@@IV1@@std@@YIPAV?$CCountedPointer@VCGuiVarTransferBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF0AA0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00af0fa0 source (stdvector_Minsertoverflow_00af0fa0.cpp) names CCountedPointer<CGuiVarTransferBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCGuiVarTransferBase@@@@IV1@@std@@YIPAV?$CCountedPointer@VCGuiVarTransferBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CGuiVarTransferBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CGuiVarTransferBase;
template CCountedPointer<CGuiVarTransferBase>* std::__uninitialized_fill_n<CCountedPointer<CGuiVarTransferBase>*,unsigned int,CCountedPointer<CGuiVarTransferBase> >(CCountedPointer<CGuiVarTransferBase>*, unsigned int, CCountedPointer<CGuiVarTransferBase> const&, std::__false_type const&);
