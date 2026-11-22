// CEngineInternalPrimitiveMorphInfo::InitialiseTextureDecals -- rebuild and sort keyed texture decals.
// Leaf: ?InitialiseTextureDecals@CEngineInternalPrimitiveMorphInfo@@QAEXABV?$vector@VCEngineTextureDecal@@V?$allocator@VCEngineTextureDecal@@@std@@@std@@@Z
// Flags: /Gr /d2inlT41
// Inline-budget: /d2inlT41 keeps the temporary blend copy expanded and the placement copy called. The lower thresholds shrink the native range helpers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_COPY_OWNER_PASS26
#define FABLE_RETAIL_CENGINETEXTUREDECAL_INLINE_GENERATED_COPY_PASS26
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
#define FABLE_RETAIL_STLP_BLEND_VECTOR_COPY_OWNERS_PASS26
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_RETAIL_STLP_DECAL_CLEAR_EXPANSION_PASS26
#define FABLE_RETAIL_STLP_DECAL_PAIR_INLINE_PASS26
#define FABLE_RETAIL_STLP_DECAL_CONSTRUCT_INLINE_PASS26
#define FABLE_RETAIL_STLP_DECAL_PUSHBACK_INLINE_PASS26
#define FABLE_RETAIL_CENGINETEXTUREDECAL_ORIGINAL_CTORS_PASS26
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_DECALS_PASS26
#include "fable_string.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
#include "engine/CEngineTextureDecal_STLHelpers.h"
void CEngineInternalPrimitiveMorphInfo::InitialiseTextureDecals(const std::vector<CEngineTextureDecal>& texture_decals)
{
    unsigned long size = texture_decals.size();
    TextureDecals.clear();
    for (long c1 = 0; c1 < size; ++c1) {
        const CEngineTextureDecal& decal = texture_decals[c1];
        std::pair<long, long> key(decal.OriginalTextureIndex, decal.DecalID);
        TextureDecals.Insert(key, decal);
    }
    TextureDecals.Update();
}