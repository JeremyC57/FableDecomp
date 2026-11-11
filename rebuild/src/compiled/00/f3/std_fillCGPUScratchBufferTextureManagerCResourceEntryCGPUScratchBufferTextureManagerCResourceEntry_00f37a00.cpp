// std::fill(CGPUScratchBufferTextureManager::CResourceEntry* first, CGPUScratchBufferTextureManager::CResourceEntry* last, CGPUScratchBufferTextureManager::CResourceEntry const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUCResourceEntry@CGPUScratchBufferTextureManager@@U12@@std@@YIXPAUCResourceEntry@CGPUScratchBufferTextureManager@@0ABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F37A00, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00f381f0 std::vector<CGPUScratchBufferTextureManager::CResourceEntry, std::allocator<CGPUScratchBufferTextureManager::CResourceEntry>>::_M_fill_insert; caller 00f381f0 std::vector<CGPUScratchBufferTextureManager::CResourceEntry, std::allocator<CGPUScratchBufferTextureManager::CResourceEntry>>::_M_fill_insert; binder ??$fill@PAUCResourceEntry@CGPUScratchBufferTextureManager@@U12@@std@@YIXPAUCResourceEntry@CGPUScratchBufferTextureManager@@0ABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CGPUScratchBufferTextureManager::CResourceEntry.
#include "stlport/fable_containers.h"
class CGPUScratchBufferTextureManager {
public:
struct CResourceEntry {
    unsigned char _body[0x2c];
};
};
template void std::fill<CGPUScratchBufferTextureManager::CResourceEntry*,CGPUScratchBufferTextureManager::CResourceEntry >(CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry const&);
