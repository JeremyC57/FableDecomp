// std::__uninitialized_fill_n(CCountedPointer<NShape::CSubdivisionTriangle>* first, unsigned int n, CCountedPointer<NShape::CSubdivisionTriangle> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@IV1@@std@@YIPAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00764590, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007659f0 source (stdvector_Minsertoverflow_007659f0.cpp) names CCountedPointer<NShape::CSubdivisionTriangle>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@IV1@@std@@YIPAV?$CCountedPointer@VCSubdivisionTriangle@NShape@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NShape::CSubdivisionTriangle>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NShape_CSubdivisionTriangle.h"
template CCountedPointer<NShape::CSubdivisionTriangle>* std::__uninitialized_fill_n<CCountedPointer<NShape::CSubdivisionTriangle>*,unsigned int,CCountedPointer<NShape::CSubdivisionTriangle> >(CCountedPointer<NShape::CSubdivisionTriangle>*, unsigned int, CCountedPointer<NShape::CSubdivisionTriangle> const&, std::__false_type const&);
