// std::__uninitialized_fill_n(CCountedPointer<CCombatSequenceBase>* first, unsigned int n, CCountedPointer<CCombatSequenceBase> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCCombatSequenceBase@@@@IV1@@std@@YIPAV?$CCountedPointer@VCCombatSequenceBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EFE50, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006f42e0 source (stdvector_Minsertoverflow_006f42e0.cpp) names CCountedPointer<CCombatSequenceBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCCombatSequenceBase@@@@IV1@@std@@YIPAV?$CCountedPointer@VCCombatSequenceBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CCombatSequenceBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CCombatSequenceBase;
template CCountedPointer<CCombatSequenceBase>* std::__uninitialized_fill_n<CCountedPointer<CCombatSequenceBase>*,unsigned int,CCountedPointer<CCombatSequenceBase> >(CCountedPointer<CCombatSequenceBase>*, unsigned int, CCountedPointer<CCombatSequenceBase> const&, std::__false_type const&);
