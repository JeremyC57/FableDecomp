// std::__destroy_aux(CCountedPointer<CChunkedFileChunk>* first, CCountedPointer<CChunkedFileChunk>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCChunkedFileChunk@@@@@std@@YIXPAV?$CCountedPointer@VCChunkedFileChunk@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA9140, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00aa9330 std::vector<CCountedPointer<CChunkedFileChunk>, std::allocator<CCountedPointer<CChunkedFileChunk>>>::operator=; caller 00aa9330 std::vector<CCountedPointer<CChunkedFileChunk>, std::allocator<CCountedPointer<CChunkedFileChunk>>>::operator=; binder ??$__destroy_aux@PAV?$CCountedPointer@VCChunkedFileChunk@@@@@std@@YIXPAV?$CCountedPointer@VCChunkedFileChunk@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CChunkedFileChunk>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileChunk.h"
template void std::__destroy_aux<CCountedPointer<CChunkedFileChunk>* >(CCountedPointer<CChunkedFileChunk>*, CCountedPointer<CChunkedFileChunk>*, std::__false_type const&);
