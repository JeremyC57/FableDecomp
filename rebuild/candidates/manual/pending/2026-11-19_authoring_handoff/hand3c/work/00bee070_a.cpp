// std::__pop_heap(pair<long,CCompositeSurfaceInfo>* first, .. last, .. result, pair<long,CCompositeSurfaceInfo> value, CKeyPairCompareLess<long,CCompositeSurfaceInfo> comp, int*) -- STLport pop_heap core: *result = *first, then sift `value` down from the root
// Leaf: ??$__pop_heap@
// Flags: none
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
#include "engine/CCompositeSurfaceInfo.h"

template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<long, CCompositeSurfaceInfo>*, std::pair<long, CCompositeSurfaceInfo>*,
                                                  std::pair<long, CCompositeSurfaceInfo>*, std::pair<long, CCompositeSurfaceInfo>,
                                                  CKeyPairCompareLess<long, CCompositeSurfaceInfo>, int*);
