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
#ifdef FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_CTOR_PASS26
 CCompositeBlendDesc(ECompositeBlendType blend,unsigned long texture_index,const CRGBColour& diffuse_colour,const CRGBColour& specular_colour)
 :TextureIndex(texture_index),BlendType(blend),DiffuseColour(diffuse_colour),SpecularColour(specular_colour){}
#else
 CCompositeBlendDesc(ECompositeBlendType,unsigned long,const CRGBColour&,const CRGBColour&);
#endif
#if defined(FABLE_RETAIL_CCOMPOSITEBLENDDESC_COPY_OWNER_PASS26) && defined(_MSC_VER)
 CCompositeBlendDesc(const CCompositeBlendDesc& other);
#elif !defined(FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_IMPLICIT_COPY_PASS26)
 CCompositeBlendDesc(const CCompositeBlendDesc&);
#endif
};
#if defined(FABLE_RETAIL_CCOMPOSITEBLENDDESC_COPY_OWNER_PASS26) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. Express the implicit copy so VC7.1 uses the
// native 005BA48A owner's size/frame settings. A row-head optimization follows the include.
#pragma optimize("s", on)
#pragma optimize("y", off)
inline CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex), BlendType(other.BlendType),
      DiffuseColour(other.DiffuseColour), SpecularColour(other.SpecularColour)
{
}
#pragma optimize("", on)
#endif // FABLE_RETAIL_CCOMPOSITEBLENDDESC_COPY_OWNER_PASS26
#endif
