// std::__pop_heap(pair<long,CCompositeSurfaceInfo>* first, .. last, .. result, pair<long,CCompositeSurfaceInfo> value, CKeyPairCompareLess<long,CCompositeSurfaceInfo> comp, int*) -- STLport pop_heap core: *result = *first, then sift `value` down from the root
// Leaf: ??$__pop_heap@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@U12@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@H@std@@YIXPAU?$pair@JVCCompositeSurfaceInfo@@@0@00U10@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEE070, 206 bytes, `ret 1Ch` (first/last in ecx/edx: the STLport algorithm TUs are /Gr, `__fastcall` on
//   the template definition; result, the 0x10-byte value, the empty comparator and the distance pointer on the stack). Callees: the old
//   count block's destroy and operator delete 0x00BFE9BC (the pair's implicit assignment through CCountedPointer), __adjust_heap 0x00BEDD70.
// Shape: the twin of the landed 0x00BEEDA0 (same TU). The STLport 4.6 body is shared (stlport/fable_containers.h FABLE_RETAIL_STLP_HEAP), and
//   the row is the explicit instantiation over the shared CCompositeSurfaceInfo (engine/CCompositeSurfaceInfo.h, CBaseClass + 
//   CCountedPointer<CCompositeSurfaceCache> Cache, 12 bytes). That replaces the draft's local stand-ins, which swapped esi/edi.
// Flags: none
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
#include "engine/CCompositeSurfaceInfo.h"

template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<long, CCompositeSurfaceInfo>*, std::pair<long, CCompositeSurfaceInfo>*,
                                                  std::pair<long, CCompositeSurfaceInfo>*, std::pair<long, CCompositeSurfaceInfo>,
                                                  CKeyPairCompareLess<long, CCompositeSurfaceInfo>, int*);
