// std::__uninitialized_fill_n(CCountedPointer<CChunkedFileChunk>* first, unsigned int n, CCountedPointer<CChunkedFileChunk> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCChunkedFileChunk@@@@IV1@@std@@YIPAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA8E80, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00aa94e0 source (stdvector_Minsertoverflow_00aa94e0.cpp) names CCountedPointer<CChunkedFileChunk>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCChunkedFileChunk@@@@IV1@@std@@YIPAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CChunkedFileChunk>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileChunk.h"
template CCountedPointer<CChunkedFileChunk>* std::__uninitialized_fill_n<CCountedPointer<CChunkedFileChunk>*,unsigned int,CCountedPointer<CChunkedFileChunk> >(CCountedPointer<CChunkedFileChunk>*, unsigned int, CCountedPointer<CChunkedFileChunk> const&, std::__false_type const&);
