#ifdef FABLE_RETAIL_TEXTURE_CACHE
#ifndef FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#define FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#include "engine/CRGBColour.h"
enum ECompositeBlendType {COMPOSITE_BLEND_NULL=0,COMPOSITE_BLEND_ADDITIVE=1,COMPOSITE_BLEND_ALPHA=2,COMPOSITE_BLEND_SOLID=3,COMPOSITE_BLEND_MULTIPLY=4};
// Original TPI classV; four fields at0,4,8,c.
class CCompositeBlendDesc {public:unsigned long TextureIndex;ECompositeBlendType BlendType;CRGBColour DiffuseColour,SpecularColour;};
FABLE_STATIC_ASSERT(sizeof(CCompositeBlendDesc)==16);
#endif

#else
#ifndef FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#define FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#include "engine/CRGBColour.h"
enum ECompositeBlendType {};
// Original TPI classV; four fields at0,4,8,c.
class CCompositeBlendDesc {public:unsigned long TextureIndex;ECompositeBlendType BlendType;CRGBColour DiffuseColour,SpecularColour;};
FABLE_STATIC_ASSERT(sizeof(CCompositeBlendDesc)==16);
#endif

#endif
