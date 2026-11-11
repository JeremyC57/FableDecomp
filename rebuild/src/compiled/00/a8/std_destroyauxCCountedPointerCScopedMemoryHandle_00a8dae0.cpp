// std::__destroy_aux(CCountedPointer<CScopedMemoryHandle>* first, CCountedPointer<CScopedMemoryHandle>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCScopedMemoryHandle@@@@@std@@YIXPAV?$CCountedPointer@VCScopedMemoryHandle@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8DAE0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a8dd10 std::vector<CCountedPointer<CScopedMemoryHandle>, std::allocator<CCountedPointer<CScopedMemoryHandle>>>::~vector; binder ??$__destroy_aux@PAV?$CCountedPointer@VCScopedMemoryHandle@@@@@std@@YIXPAV?$CCountedPointer@VCScopedMemoryHandle@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CScopedMemoryHandle>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CScopedMemoryHandle;
template void std::__destroy_aux<CCountedPointer<CScopedMemoryHandle>* >(CCountedPointer<CScopedMemoryHandle>*, CCountedPointer<CScopedMemoryHandle>*, std::__false_type const&);
