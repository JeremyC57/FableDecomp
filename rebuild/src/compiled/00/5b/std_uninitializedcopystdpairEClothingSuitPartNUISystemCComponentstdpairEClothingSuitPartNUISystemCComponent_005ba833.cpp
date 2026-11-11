// std::__uninitialized_copy(std::pair<EClothingSuitPart,NUISystem::CComponent*>* first, std::pair<EClothingSuitPart,NUISystem::CComponent*>* last, std::pair<EClothingSuitPart,NUISystem::CComponent*>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@PAU12@@std@@YIPAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA833, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005bb409 std::vector<std::pair<EClothingSuitPart, NUISystem::CComponent *>, std::allocator<std::pair<EClothingSuitPart, NUISystem::CComponent *>>>::_M_insert_overflow; caller 005bb409 std::vector<std::pair<EClothingSuitPart, NUISystem::CComponent *>, std::allocator<std::pair<EClothingSuitPart, NUISystem::CComponent *>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@PAU12@@std@@YIPAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<EClothingSuitPart,NUISystem::CComponent*>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
enum EClothingSuitPart { };
namespace NUISystem {
class CComponent;
}
template std::pair<EClothingSuitPart,NUISystem::CComponent*>* std::__uninitialized_copy<std::pair<EClothingSuitPart,NUISystem::CComponent*>*,std::pair<EClothingSuitPart,NUISystem::CComponent*>* >(std::pair<EClothingSuitPart,NUISystem::CComponent*>*, std::pair<EClothingSuitPart,NUISystem::CComponent*>*, std::pair<EClothingSuitPart,NUISystem::CComponent*>*, std::__false_type const&);
