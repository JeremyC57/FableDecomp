#if defined(FABLE_RETAIL_MESH_PRELOAD_CALLS_PASS30) && !defined(FABLE_MESH_PRELOAD_INLINE_DEFINED)
#define FABLE_MESH_PRELOAD_INLINE_DEFINED
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }
extern unsigned char GTopBitInByteTable[256];
extern const float DELTA_NOTIONAL_ZERO;
inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return GTopBitInByteTable[value];
}

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

inline float FABLE_FASTCALL CEnginePrimitiveRenderUtil::GetLODErrorTolerance() { return NEngineInternal::GPPrimitiveRendererSharedData->LODErrorTolerance; }

inline float FABLE_FASTCALL CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted() { return NEngineInternal::GPPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }

inline float FABLE_FASTCALL CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier; }

#ifdef FABLE_RETAIL_ANIMATED_MESH_PRELOAD_PASS30
namespace NEngineInternal { extern CEngine* GPEngine; }
inline float FABLE_FASTCALL CEnginePrimitiveRenderUtil::GetCharacterLODErrorTolerance() {
    return NEngineInternal::GPPrimitiveRendererSharedData->CharacterLODErrorTolerance;
}
#endif

inline unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateLOD(
    const C3DMeshInfo* mesh_info, float distance, float radius, float tolerance)
{
    float min_distance = distance - radius > DELTA_NOTIONAL_ZERO ? distance - radius : DELTA_NOTIONAL_ZERO;
    float base = min_distance / (radius * CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted());
    float base_tolerance = base * tolerance;
    FABLE_RETAIL_ASSERT(mesh_info);
    unsigned long lod;
    for (lod = 0; mesh_info->HasLOD(lod + 1); ++lod) {
        if (mesh_info->GetLODError(lod + 1) > base_tolerance) return lod;
    }
    return lod;
}

inline unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(
    const C3DVector& cam_pos, const CMatrix3x4& object_transform, const C3DPrimitive2& prim,
    float object_scale, const C3DMeshMaterial* material, const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere bsphere = prim.GetBoundingSphere();
    object_transform.TransformPoint(bsphere.Center);
    float distance = (cam_pos - bsphere.Center).GetMagnitude();
    unsigned long mipmap_level = 0;
    float min_distance = distance - bsphere.GetRadius() * object_scale;
    float dtexdpos = prim.GetAverageTextureStretch();
    float divisor = min_distance * dtexdpos;
    if (divisor > DELTA_NOTIONAL_ZERO) {
        long diffuse_index = material->GetDiffuseMapBankIndex();
        if (diffuse_index > 0) {
            unsigned long texture_size = graphic_bank->PeekGraphic(diffuse_index)->GetWidth();
            float ideal_size = CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() * object_scale / divisor;
            long ratio = GFFloatToLongFloor((float)texture_size / ideal_size);
            if (ratio < 1) ratio = 1;
            else if (ratio > 255) ratio = 255;
            mipmap_level = GFGetTopBitInByte((unsigned char)ratio);
        }
    }
    return mipmap_level;
}

#endif
