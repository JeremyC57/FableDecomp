// std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(unsigned int) -- resize with an empty navigation-layer update
// Retail: 0x0051D870,104 bytes,thiscall
// Flags: /Gr
// Leaf: ?resize@?$vector@VCNavigationLayerUpdate@CNavigationAreaUpdate@@V?$allocator@VCNavigationLayerUpdate@CNavigationAreaUpdate@@@std@@@std@@QAEXI@Z
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/CNavigationAreaUpdate.h"
template void std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::size_type);