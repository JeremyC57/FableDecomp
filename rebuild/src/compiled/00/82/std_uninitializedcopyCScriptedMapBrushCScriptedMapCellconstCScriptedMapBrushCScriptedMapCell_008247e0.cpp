// std::__uninitialized_copy(CScriptedMapBrush::CScriptedMapCell const* first, CScriptedMapBrush::CScriptedMapCell const* last, CScriptedMapBrush::CScriptedMapCell* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCScriptedMapCell@CScriptedMapBrush@@PAV12@@std@@YIPAVCScriptedMapCell@CScriptedMapBrush@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008247E0, 68 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00824dd0 std::vector<CScriptedMapBrush::CScriptedMapCell, std::allocator<CScriptedMapBrush::CScriptedMapCell>>::vector; binder ??$__uninitialized_copy@PBVCScriptedMapCell@CScriptedMapBrush@@PAV12@@std@@YIPAVCScriptedMapCell@CScriptedMapBrush@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CScriptedMapBrush::CScriptedMapCell.
#include "stlport/fable_containers.h"
class CScriptedMapBrush {
public:
class CScriptedMapCell {
public:                              // 20-byte POD record, implicit copy constructor
    long m_0, m_4, m_8, m_c, m_10;
};
};
template CScriptedMapBrush::CScriptedMapCell* std::__uninitialized_copy<CScriptedMapBrush::CScriptedMapCell const*,CScriptedMapBrush::CScriptedMapCell* >(CScriptedMapBrush::CScriptedMapCell const*, CScriptedMapBrush::CScriptedMapCell const*, CScriptedMapBrush::CScriptedMapCell*, std::__false_type const&);
