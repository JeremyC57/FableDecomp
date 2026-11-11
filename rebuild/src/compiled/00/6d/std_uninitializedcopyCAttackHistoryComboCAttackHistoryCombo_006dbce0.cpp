// std::__uninitialized_copy(CAttackHistoryCombo* first, CAttackHistoryCombo* last, CAttackHistoryCombo* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCAttackHistoryCombo@@PAV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DBCE0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006dca50 std::vector<CAttackHistoryCombo>::_M_insert_overflow; caller 006dca50 std::vector<CAttackHistoryCombo>::_M_insert_overflow; binder ??$__uninitialized_copy@PAVCAttackHistoryCombo@@PAV1@@std@@YIPAVCAttackHistoryCombo@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAttackHistoryCombo.
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#include "stlport/fable_containers.h"
#include "engine/CAttackHistoryCombo.h"
template CAttackHistoryCombo* std::__uninitialized_copy<CAttackHistoryCombo*,CAttackHistoryCombo* >(CAttackHistoryCombo*, CAttackHistoryCombo*, CAttackHistoryCombo*, std::__false_type const&);
