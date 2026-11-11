// std::uninitialized_fill_n(CAttackHistoryCombo* first, unsigned int n, CAttackHistoryCombo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCAttackHistoryCombo@@IV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC670, 48 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006dccc0 source (stdvector_Mfillinsert_006dccc0.cpp) names CAttackHistoryCombo; binder ??$uninitialized_fill_n@PAVCAttackHistoryCombo@@IV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAttackHistoryCombo.
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#include "stlport/fable_containers.h"
#include "engine/CAttackHistoryCombo.h"
template CAttackHistoryCombo* std::uninitialized_fill_n<CAttackHistoryCombo*,unsigned int,CAttackHistoryCombo >(CAttackHistoryCombo*, unsigned int, CAttackHistoryCombo const&);
