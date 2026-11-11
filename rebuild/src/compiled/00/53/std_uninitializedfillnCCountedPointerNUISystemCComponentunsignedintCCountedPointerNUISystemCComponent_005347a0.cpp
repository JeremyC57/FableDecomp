// std::__uninitialized_fill_n(CCountedPointer<NUISystem::CComponent>* first, unsigned int n, CCountedPointer<NUISystem::CComponent> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCComponent@NUISystem@@@@IV1@@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005347A0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005359d0 source (stdvector_Minsertoverflow_005359d0.cpp) names CCountedPointer<NUISystem::CComponent>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCComponent@NUISystem@@@@IV1@@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NUISystem::CComponent>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CComponent.h"
template CCountedPointer<NUISystem::CComponent>* std::__uninitialized_fill_n<CCountedPointer<NUISystem::CComponent>*,unsigned int,CCountedPointer<NUISystem::CComponent> >(CCountedPointer<NUISystem::CComponent>*, unsigned int, CCountedPointer<NUISystem::CComponent> const&, std::__false_type const&);
