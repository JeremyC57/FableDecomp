// std::__copy(CEngineOcclusionManager::COccluderFlareEntry* first, CEngineOcclusionManager::COccluderFlareEntry* last, CEngineOcclusionManager::COccluderFlareEntry* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@H@std@@YIPAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8E8B0, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00b8f780 std::vector<CEngineOcclusionManager::COccluderFlareEntry, std::allocator<CEngineOcclusionManager::COccluderFlareEntry>>::resize; binder ??$__copy@PAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@H@std@@YIPAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineOcclusionManager::COccluderFlareEntry.
#include "stlport/fable_containers.h"
class CEngineOcclusionManager {
public:
struct COccluderFlareEntry {
    unsigned char _body[0x1c];
};
};
template CEngineOcclusionManager::COccluderFlareEntry* std::__copy<CEngineOcclusionManager::COccluderFlareEntry*,CEngineOcclusionManager::COccluderFlareEntry*,int >(CEngineOcclusionManager::COccluderFlareEntry*, CEngineOcclusionManager::COccluderFlareEntry*, CEngineOcclusionManager::COccluderFlareEntry*, std::random_access_iterator_tag const&, int*);
