#ifndef FABLE_ENGINE_CENGINETEXTUREDECAL_H
#define FABLE_ENGINE_CENGINETEXTUREDECAL_H
#include "stlport/fable_containers.h"
#include "engine/CCompositeBlendDesc.h"
// Original PDB class fields; native vector is12 bytes, giving size20.
class CEngineTextureDecal {
public:
#ifdef FABLE_RETAIL_CENGINETEXTUREDECAL_ORIGINAL_CTORS_PASS26
 // Both original complete ctor overload records: public default and(long,long), generated copy left implicit.
 CEngineTextureDecal();
 CEngineTextureDecal(long original_texture,long decal_id);
#endif
#ifdef FABLE_RETAIL_CENGINETEXTUREDECAL_INLINE_GENERATED_COPY_PASS26
 // Inline-budget: 00BECDE0 expands both decal copies and calls the inner vector constructor only at placement.
 FABLE_FORCEINLINE CEngineTextureDecal(const CEngineTextureDecal& other);
#endif
 long OriginalTextureIndex,DecalID;
 std::vector<CCompositeBlendDesc> OverlaidBlends;
};
FABLE_STATIC_ASSERT_AT(11, sizeof(CEngineTextureDecal)==20);
#endif
