// std::__uninitialized_copy(CGPUScratchBufferTextureManager::CResourceEntry* first, CGPUScratchBufferTextureManager::CResourceEntry* last, CGPUScratchBufferTextureManager::CResourceEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@@std@@YIPAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F37CC0, 98 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00f37f60 std::vector<CGPUScratchBufferTextureManager::CResourceEntry, std::allocator<CGPUScratchBufferTextureManager::CResourceEntry>>::_M_insert_overflow; caller 00f37f60 std::vector<CGPUScratchBufferTextureManager::CResourceEntry, std::allocator<CGPUScratchBufferTextureManager::CResourceEntry>>::_M_insert_overflow; caller 00f381f0 std::vector<CGPUScratchBufferTextureManager::CResourceEntry, std::allocator<CGPUScratchBufferTextureManager::CResourceEntry>>::_M_fill_insert; binder ??$__uninitialized_copy@PAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@@std@@YIPAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CGPUScratchBufferTextureManager::CResourceEntry.
#include "stlport/fable_containers.h"
class CGPUScratchBufferTextureManager {
public:
struct CResourceEntry {                               // 44 bytes (0x2c), dword members
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    long m_1c;
    long m_20;
    long m_24;
    long m_28;
    CResourceEntry(const CResourceEntry& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18), m_1c(o.m_1c), m_20(o.m_20), m_24(o.m_24), m_28(o.m_28) { }
};
};
template CGPUScratchBufferTextureManager::CResourceEntry* std::__uninitialized_copy<CGPUScratchBufferTextureManager::CResourceEntry*,CGPUScratchBufferTextureManager::CResourceEntry* >(CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry*, std::__false_type const&);
