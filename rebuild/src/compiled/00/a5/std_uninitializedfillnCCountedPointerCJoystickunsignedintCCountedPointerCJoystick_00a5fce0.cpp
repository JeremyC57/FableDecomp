// std::__uninitialized_fill_n(CCountedPointer<CJoystick>* first, unsigned int n, CCountedPointer<CJoystick> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCJoystick@@@@IV1@@std@@YIPAV?$CCountedPointer@VCJoystick@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5FCE0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a5fef0 source (stdvector_Minsertoverflow_00a5fef0.cpp) names CCountedPointer<CJoystick>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCJoystick@@@@IV1@@std@@YIPAV?$CCountedPointer@VCJoystick@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CJoystick>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CJoystick.h"
template CCountedPointer<CJoystick>* std::__uninitialized_fill_n<CCountedPointer<CJoystick>*,unsigned int,CCountedPointer<CJoystick> >(CCountedPointer<CJoystick>*, unsigned int, CCountedPointer<CJoystick> const&, std::__false_type const&);
