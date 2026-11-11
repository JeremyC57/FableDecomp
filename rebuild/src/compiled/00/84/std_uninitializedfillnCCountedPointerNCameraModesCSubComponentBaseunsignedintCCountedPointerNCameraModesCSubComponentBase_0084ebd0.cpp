// std::__uninitialized_fill_n(CCountedPointer<NCameraModes::CSubComponentBase>* first, unsigned int n, CCountedPointer<NCameraModes::CSubComponentBase> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@IV1@@std@@YIPAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084EBD0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0084fab0 source (stdvector_Minsertoverflow_0084fab0.cpp) names CCountedPointer<NCameraModes::CSubComponentBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@IV1@@std@@YIPAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NCameraModes::CSubComponentBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NCameraModes {
class CSubComponentBase;
}
template CCountedPointer<NCameraModes::CSubComponentBase>* std::__uninitialized_fill_n<CCountedPointer<NCameraModes::CSubComponentBase>*,unsigned int,CCountedPointer<NCameraModes::CSubComponentBase> >(CCountedPointer<NCameraModes::CSubComponentBase>*, unsigned int, CCountedPointer<NCameraModes::CSubComponentBase> const&, std::__false_type const&);
