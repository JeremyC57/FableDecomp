// std::uninitialized_fill_n(CFileInstaller::CBufferChunk* first, unsigned int n, CFileInstaller::CBufferChunk const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCBufferChunk@CFileInstaller@@IV12@@std@@YIPAVCBufferChunk@CFileInstaller@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6570, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009c75a0 std::vector<CFileInstaller::CBufferChunk, std::allocator<CFileInstaller::CBufferChunk>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCBufferChunk@CFileInstaller@@IV12@@std@@YIPAVCBufferChunk@CFileInstaller@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CFileInstaller::CBufferChunk.
#include "stlport/fable_containers.h"
#include "engine/CFileInstaller.h"
template CFileInstaller::CBufferChunk* std::uninitialized_fill_n<CFileInstaller::CBufferChunk*,unsigned int,CFileInstaller::CBufferChunk >(CFileInstaller::CBufferChunk*, unsigned int, CFileInstaller::CBufferChunk const&);
