// std::__uninitialized_fill_n(CDefPointer<CHeroAbilityDef const>* first, unsigned int n, CDefPointer<CHeroAbilityDef const> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CDefPointer@$$CBVCHeroAbilityDef@@@@IV1@@std@@YIPAV?$CDefPointer@$$CBVCHeroAbilityDef@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F7A1D, 36 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005fa10a std::vector<CDefPointer<const CHeroAbilityDef> >::_M_insert_overflow (landed source); binder names CDefPointer<const CHeroAbilityDef> (non-template spelling)
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDefPointer<CHeroAbilityDef const>.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroAbilityDef.h"
template CDefPointer<CHeroAbilityDef const>* std::__uninitialized_fill_n<CDefPointer<CHeroAbilityDef const>*,unsigned int,CDefPointer<CHeroAbilityDef const> >(CDefPointer<CHeroAbilityDef const>*, unsigned int, CDefPointer<CHeroAbilityDef const> const&, std::__false_type const&);
