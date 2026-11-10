#ifndef FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#define FABLE_ENGINE_CCOMPOSITEBLENDDESC_H
#include "engine/CRGBColour.h"
// Both original PDB records: class size16, public fields0/4/8/12.
enum ECompositeBlendType { COMPOSITE_BLEND_NULL=0,COMPOSITE_BLEND_ADDITIVE=1,
 COMPOSITE_BLEND_ALPHA=2,COMPOSITE_BLEND_SOLID=3,COMPOSITE_BLEND_MULTIPLY=4 };
class CCompositeBlendDesc {
public:
 unsigned long TextureIndex;
 ECompositeBlendType BlendType;
 CRGBColour DiffuseColour,SpecularColour;
 CCompositeBlendDesc();
 CCompositeBlendDesc(ECompositeBlendType,unsigned long,const CRGBColour&,const CRGBColour&);
 CCompositeBlendDesc(const CCompositeBlendDesc&);
};
#endif
