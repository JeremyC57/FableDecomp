// std::__uninitialized_fill_n(std::pair<EClothingSuitPart,NUISystem::CComponent*>* first, unsigned int n, std::pair<EClothingSuitPart,NUISystem::CComponent*> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@IU12@@std@@YIPAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA858, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005bb409 std::vector<std::pair<EClothingSuitPart, NUISystem::CComponent *>, std::allocator<std::pair<EClothingSuitPart, NUISystem::CComponent *>>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@IU12@@std@@YIPAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<EClothingSuitPart,NUISystem::CComponent*>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
enum EClothingSuitPart { };
namespace NUISystem {
class CComponent;
}
template std::pair<EClothingSuitPart,NUISystem::CComponent*>* std::__uninitialized_fill_n<std::pair<EClothingSuitPart,NUISystem::CComponent*>*,unsigned int,std::pair<EClothingSuitPart,NUISystem::CComponent*> >(std::pair<EClothingSuitPart,NUISystem::CComponent*>*, unsigned int, std::pair<EClothingSuitPart,NUISystem::CComponent*> const&, std::__false_type const&);
