// std::__uninitialized_fill_n(CCountedPointer<NUISystem::CAction>* first, unsigned int n, CCountedPointer<NUISystem::CAction> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCAction@NUISystem@@@@IV1@@std@@YIPAV?$CCountedPointer@VCAction@NUISystem@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054A180, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0054a2a0 source (stdvector_Minsertoverflow_0054a2a0.cpp) names CCountedPointer<NUISystem::CAction>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCAction@NUISystem@@@@IV1@@std@@YIPAV?$CCountedPointer@VCAction@NUISystem@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NUISystem::CAction>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CAction.h"
template CCountedPointer<NUISystem::CAction>* std::__uninitialized_fill_n<CCountedPointer<NUISystem::CAction>*,unsigned int,CCountedPointer<NUISystem::CAction> >(CCountedPointer<NUISystem::CAction>*, unsigned int, CCountedPointer<NUISystem::CAction> const&, std::__false_type const&);
