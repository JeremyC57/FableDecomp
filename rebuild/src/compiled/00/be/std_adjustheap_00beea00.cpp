// std::__adjust_heap -- restore the heap of texture-decal entries.
// Retail: 0x00beea00,266 bytes,fastcall template.
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@HU12@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@std@@YIXPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@HHU10@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@Z
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "engine/CEngineTextureDecal.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<std::pair<long,long>,CEngineTextureDecal>*,int,int,
 std::pair<std::pair<long,long>,CEngineTextureDecal>,CKeyPairCompareLess<std::pair<long,long>,CEngineTextureDecal>);