// std::__uninitialized_fill_n(CCountedPointer<NShape::CShape>* first, unsigned int n, CCountedPointer<NShape::CShape> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShape@NShape@@@@IV1@@std@@YIPAV?$CCountedPointer@VCShape@NShape@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00763EB0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007656d0 source (stdvector_Minsertoverflow_007656d0.cpp) names CCountedPointer<NShape::CShape>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShape@NShape@@@@IV1@@std@@YIPAV?$CCountedPointer@VCShape@NShape@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NShape::CShape>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NShape {
class CShape;
}
template CCountedPointer<NShape::CShape>* std::__uninitialized_fill_n<CCountedPointer<NShape::CShape>*,unsigned int,CCountedPointer<NShape::CShape> >(CCountedPointer<NShape::CShape>*, unsigned int, CCountedPointer<NShape::CShape> const&, std::__false_type const&);
