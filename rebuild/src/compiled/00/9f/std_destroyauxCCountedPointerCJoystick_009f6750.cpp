// std::__destroy_aux(CCountedPointer<CJoystick>* first, CCountedPointer<CJoystick>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCJoystick@@@@@std@@YIXPAV?$CCountedPointer@VCJoystick@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F6750, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009f5c80 source (CInputManager_CInputManager_009f5c80.cpp) names CCountedPointer<CJoystick>; caller 00a5fef0 source (stdvector_Minsertoverflow_00a5fef0.cpp) names CCountedPointer<CJoystick>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCJoystick@@@@@std@@YIXPAV?$CCountedPointer@VCJoystick@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CJoystick>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CJoystick.h"
template void std::__destroy_aux<CCountedPointer<CJoystick>* >(CCountedPointer<CJoystick>*, CCountedPointer<CJoystick>*, std::__false_type const&);
