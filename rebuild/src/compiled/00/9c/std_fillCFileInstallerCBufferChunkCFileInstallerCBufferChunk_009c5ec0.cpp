// std::fill(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, CFileInstaller::CBufferChunk const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCBufferChunk@CFileInstaller@@V12@@std@@YIXPAVCBufferChunk@CFileInstaller@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C5EC0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009c75a0 std::vector<CFileInstaller::CBufferChunk, std::allocator<CFileInstaller::CBufferChunk>>::_M_fill_insert; binder ??$fill@PAVCBufferChunk@CFileInstaller@@V12@@std@@YIXPAVCBufferChunk@CFileInstaller@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CFileInstaller::CBufferChunk.
#include "stlport/fable_containers.h"
#include "engine/CFileInstaller.h"
template void std::fill<CFileInstaller::CBufferChunk*,CFileInstaller::CBufferChunk >(CFileInstaller::CBufferChunk*, CFileInstaller::CBufferChunk*, CFileInstaller::CBufferChunk const&);
