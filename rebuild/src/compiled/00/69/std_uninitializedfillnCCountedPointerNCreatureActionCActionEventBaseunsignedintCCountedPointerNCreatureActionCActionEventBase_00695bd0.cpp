// std::__uninitialized_fill_n(CCountedPointer<NCreatureAction::CActionEventBase>* first, unsigned int n, CCountedPointer<NCreatureAction::CActionEventBase> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@IV1@@std@@YIPAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00695BD0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00696f00 source (stdvector_Minsertoverflow_00696f00.cpp) names CCountedPointer<NCreatureAction::CActionEventBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@IV1@@std@@YIPAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NCreatureAction::CActionEventBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction {
class CActionEventBase;
}
template CCountedPointer<NCreatureAction::CActionEventBase>* std::__uninitialized_fill_n<CCountedPointer<NCreatureAction::CActionEventBase>*,unsigned int,CCountedPointer<NCreatureAction::CActionEventBase> >(CCountedPointer<NCreatureAction::CActionEventBase>*, unsigned int, CCountedPointer<NCreatureAction::CActionEventBase> const&, std::__false_type const&);
