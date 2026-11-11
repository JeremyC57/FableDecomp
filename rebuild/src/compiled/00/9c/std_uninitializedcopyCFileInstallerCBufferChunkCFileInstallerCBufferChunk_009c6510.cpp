// std::__uninitialized_copy(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, CFileInstaller::CBufferChunk* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCBufferChunk@CFileInstaller@@PAV12@@std@@YIPAVCBufferChunk@CFileInstaller@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6510, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 009c75a0 std::vector<CFileInstaller::CBufferChunk, std::allocator<CFileInstaller::CBufferChunk>>::_M_fill_insert; caller 009c75a0 std::vector<CFileInstaller::CBufferChunk, std::allocator<CFileInstaller::CBufferChunk>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCBufferChunk@CFileInstaller@@PAV12@@std@@YIPAVCBufferChunk@CFileInstaller@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CFileInstaller::CBufferChunk.
#include "stlport/fable_containers.h"
#include "engine/CFileInstaller.h"
template CFileInstaller::CBufferChunk* std::__uninitialized_copy<CFileInstaller::CBufferChunk*,CFileInstaller::CBufferChunk* >(CFileInstaller::CBufferChunk*, CFileInstaller::CBufferChunk*, CFileInstaller::CBufferChunk*, std::__false_type const&);
