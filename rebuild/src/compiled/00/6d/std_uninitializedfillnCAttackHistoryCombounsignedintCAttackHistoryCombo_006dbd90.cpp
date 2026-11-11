// std::__uninitialized_fill_n(CAttackHistoryCombo* first, unsigned int n, CAttackHistoryCombo const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCAttackHistoryCombo@@IV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DBD90, 48 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006dca50 std::vector<CAttackHistoryCombo>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCAttackHistoryCombo@@IV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAttackHistoryCombo.
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#include "stlport/fable_containers.h"
#include "engine/CAttackHistoryCombo.h"
template CAttackHistoryCombo* std::__uninitialized_fill_n<CAttackHistoryCombo*,unsigned int,CAttackHistoryCombo >(CAttackHistoryCombo*, unsigned int, CAttackHistoryCombo const&, std::__false_type const&);
