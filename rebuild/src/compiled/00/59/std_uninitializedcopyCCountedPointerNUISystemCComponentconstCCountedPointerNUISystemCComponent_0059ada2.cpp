// std::__uninitialized_copy(CCountedPointer<NUISystem::CComponent> const* first, CCountedPointer<NUISystem::CComponent> const* last, CCountedPointer<NUISystem::CComponent>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059ADA2, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0059ae54 std::vector<CCountedPointer<NUISystem::CComponent>, std::allocator<CCountedPointer<NUISystem::CComponent>>>::vector; callee 00428928 CCountedPointer<NUISystem::CComponent>::CCountedPointer; binder ??$__uninitialized_copy@PBV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PBV1@0PAV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NUISystem::CComponent>.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CComponent.h"
template CCountedPointer<NUISystem::CComponent>* std::__uninitialized_copy<CCountedPointer<NUISystem::CComponent> const*,CCountedPointer<NUISystem::CComponent>* >(CCountedPointer<NUISystem::CComponent> const*, CCountedPointer<NUISystem::CComponent> const*, CCountedPointer<NUISystem::CComponent>*, std::__false_type const&);
