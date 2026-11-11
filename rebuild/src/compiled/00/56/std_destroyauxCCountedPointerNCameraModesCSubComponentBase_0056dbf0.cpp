// std::__destroy_aux(CCountedPointer<NCameraModes::CSubComponentBase>* first, CCountedPointer<NCameraModes::CSubComponentBase>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@@std@@YIXPAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056DBF0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0084f740 std::vector<CCountedPointer<NCameraModes::CSubComponentBase>, std::allocator<CCountedPointer<NCameraModes::CSubComponentBase>>>::operator=; caller 0084f740 std::vector<CCountedPointer<NCameraModes::CSubComponentBase>, std::allocator<CCountedPointer<NCameraModes::CSubComponentBase>>>::operator=; binder ??$__destroy_aux@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@@std@@YIXPAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NCameraModes::CSubComponentBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NCameraModes {
class CSubComponentBase;
}
template void std::__destroy_aux<CCountedPointer<NCameraModes::CSubComponentBase>* >(CCountedPointer<NCameraModes::CSubComponentBase>*, CCountedPointer<NCameraModes::CSubComponentBase>*, std::__false_type const&);
