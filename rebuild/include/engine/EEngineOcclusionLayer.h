#ifndef FABLE_EENGINEOCCLUSIONLAYER_H
#define FABLE_EENGINEOCCLUSIONLAYER_H
#include "rebuild_abi.h"
enum EEngineOcclusionLayer{OCCLUSION_LAYER_NONE=-1,OCCLUSION_LAYER_PRIMARY_VIEW=0,OCCLUSION_LAYER_SHADOW_BUFFER=1,OCCLUSION_LAYER_MAX=2};
inline unsigned long FABLE_FASTCALL GFGetOcclusionLayerBitMask(EEngineOcclusionLayer layer){return 1UL<<(layer+1);}
#endif
