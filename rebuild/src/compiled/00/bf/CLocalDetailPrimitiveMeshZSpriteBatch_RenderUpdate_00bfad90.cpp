// 00bfad90 CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate
// Leaf: ?RenderUpdate@CLocalDetailPrimitiveMeshZSpriteBatch@@UAEXABVCEngineCamera@@_NKPBVCLocalDetailObjectCollectionType@@M@Z
// Flags: /O2 /Oy
// Inferred: retail clip-distance clamps, mesh fade and replacement flags are reconstructed from both loops.
// The individual flag enumerator names for 0x211, 0x311 and 0x1000 remain unresolved.
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
// Retail-asm: 0x00BFAFEC `fistp dword ptr [esp+28h]` (after `fld dword ptr [esp+24h]`) -- the engine's inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, Retail-asm-helper).
#include "engine/C3DVector.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CRGBColour.h"
#include "engine/CLocalDetailPrimitiveMeshZSpriteBatch.h"
#include "engine/GFFloatToLongNear.h"
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; extern CEngineLocalDetailGenerator* GPLocalDetailGenerator; }
void CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(const CEngineCamera& camera, bool all_inside_zclip_region, unsigned long clip_flags,
                                                         const CLocalDetailObjectCollectionType* type, float interp)
{
    if (!ClipPrimitive(type, camera, all_inside_zclip_region, clip_flags)) {
        if (HasMeshPrimitiveHandles) {
            HasMeshPrimitiveHandles = false;
            for (long obj = 0; obj < ObjectCount; ++obj) {
                CObject& object = Objects[obj];
                float distance = (camera.PeekCameraPos() - object.BoundingSphere.Center).GetMagnitude();
                float zsprite_fade_end = type->PeekZSpriteFadeEnd();
                float min_zsprite_fade_end = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
                float max_zsprite_fade_end = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
                if (zsprite_fade_end < min_zsprite_fade_end) zsprite_fade_end = min_zsprite_fade_end;
                else if (zsprite_fade_end > max_zsprite_fade_end) zsprite_fade_end = max_zsprite_fade_end;
                if (distance > zsprite_fade_end)
                    object.PrimitiveHandle.Clear();
                else if (!object.PrimitiveHandle.IsNull())
                    HasMeshPrimitiveHandles = true;
            }
        }
        return;
    }
    for (long obj = 0; obj < ObjectCount; ++obj) {
        CObject& object = Objects[obj];
        C3DVector unrotated_bounding_sphere_center = type->PeekMesh()->GetBoundingSphere().Center;
        unrotated_bounding_sphere_center *= object.Scale;
        unrotated_bounding_sphere_center += object.Matrix.GetTranslation();
        float distance = (camera.PeekCameraPos() - unrotated_bounding_sphere_center).GetMagnitude();
        CEngineFadeDistance mesh_fade(type->PeekZSpriteFadeStart(), type->PeekZSpriteFadeEnd());
        float min_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
        float max_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
        float clamped_dist = mesh_fade.MaxDrawDistance < min_mesh_dist ? min_mesh_dist : (mesh_fade.MaxDrawDistance > max_mesh_dist ? max_mesh_dist : mesh_fade.MaxDrawDistance);
        mesh_fade.FadeBeginDistance -= mesh_fade.MaxDrawDistance - clamped_dist;
        mesh_fade.MaxDrawDistance = clamped_dist;
        long mesh_alpha = GFFloatToLongNear((distance - mesh_fade.MaxDrawDistance) * 255.0f / (mesh_fade.FadeBeginDistance - mesh_fade.MaxDrawDistance));
        mesh_alpha += NEngineInternal::GPLocalDetailGenerator->GetStippleAlphaMeshBias();
        if (mesh_alpha < 0)
            mesh_alpha = 0;
        else if (mesh_alpha > 255)
            mesh_alpha = 255;
        if (distance < mesh_fade.MaxDrawDistance) {
            if (object.PrimitiveHandle.IsNull() || NEngineInternal::GPPrimitiveRenderer->IsDrawDistanceInvalid() != 0.0f) {
                CEngineFadeDistance fade(type->PeekFadeStartBeforeVideoOptionModifier(), type->PeekFadeEndBeforeVideoOptionModifier());
                unsigned long flags = 0x11;
                flags |= 0x200;
                if (type->PeekAlphaIsBoolean())
                    flags |= 0x100;
                if ((float)type->PeekReceivesShadows() == 0.0f)
                    flags |= 0x1000;
                object.PrimitiveHandle = NEngineInternal::GPPrimitiveRenderer->AddStaticMesh(type->PeekMesh(), type->PeekShadowMesh(), object.Matrix, object.Scale, fade,
                                                                            mesh_alpha, CRGBColour(0xff, 0xff, 0xff, 0xff), type->PeekAlphaRef(),
                                                                            type->PeekAlphaMipBias(), true, flags);
                HasMeshPrimitiveHandles = true;
            } else
                NEngineInternal::GPPrimitiveRenderer->UpdateStaticMeshAlpha(object.PrimitiveHandle, mesh_alpha);
        } else
            object.PrimitiveHandle.Clear();
    }
}