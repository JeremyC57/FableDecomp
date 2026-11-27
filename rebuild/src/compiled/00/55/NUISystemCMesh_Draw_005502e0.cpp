// 005502e0 NUISystem::CMesh::Draw
// Leaf: ?Draw@CMesh@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: /Os
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_GENERATED_SPECIALS_PASS23
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GENERATED_COPY_PASS23
#define FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_GENERATED_DTOR_PASS23
#define FABLE_RETAIL_DRAW_TYPED_ANIMATION
// Retail-asm: 0x00550464 `fld dword ptr [esp+20h]` + 0x00550468 `fistp dword ptr [esp+14h]` (and 0x00550478 / 0x00550488 / 0x00550498 / 0x0055053C..0x0055056C) -- the engine's inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, Retail-asm-helper).
#include "engine/EEnginePrimitiveType.h"
#define FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS
#define FABLE_RETAIL_C3DANIMATIONSTATE_ORIGINAL_CTORS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DANIMATIONSTATE
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CManager.h"

#include "engine/CIEngine.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CEnginePrimitive2DStaticMesh.h"
#include "engine/CEnginePrimitive2DAnimatingMesh.h"
#include "engine/GFFloatToLongNear.h"
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<>
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CCountedPointer<CMeshDataBank>::ReleaseData()
{
    if (Info && --Info->RefCount == 0) {
        Info->DeleteFunc(Info->Data);
        delete Info;
    }
    Data = 0;
    Info = 0;
}
template<>
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CCountedPointer<CMeshDataBank>::~CCountedPointer()
{
    ReleaseData();
}
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
C3DAnimationInterpolator::C3DAnimationInterpolator(const C3DAnimationBlendState& start_state,
                                                   const C3DAnimationBlendState& end_state)
    : StartState(start_state), EndState(end_state)
{
}

CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh(unsigned __int64 uid, unsigned long mesh_index, const C2DBoxF& clip_box,
    const C3DOrientationInterpolator& orient, EEngineInterpolationSet interp_set, unsigned char alpha, CRGBColour tint_colour)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_STATIC_MESH, uid, interp_set), MeshIndex(mesh_index), ClipBox(clip_box),
      Alpha(alpha), Orientation(orient), BoundingSphere(C3DVector(0, 0, 0), -1), TintColour(tint_colour)
{
}

CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(unsigned __int64 uid, unsigned long mesh_index, const C3DAnimationBlendState& anim,
    const C2DBoxF& clip_box, const C3DBoundingSphere& bounding_sphere, const C3DOrientationInterpolator& orient,
    EEngineInterpolationSet interp_set, unsigned char alpha, CRGBColour tint_colour)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_ANIMATING_MESH, uid, interp_set), ClipBox(clip_box), MeshIndex(mesh_index),
      Alpha(alpha), Orientation(orient), Anim(anim, anim), BoundingSphere(bounding_sphere), TintColour(tint_colour)
{
}

namespace NUISystem {
void CMesh::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || (RenderZoom.X <= 0.0f && RenderZoom.Y <= 0.0f)) {
        if (NoRenderNextFrame == 1) {
            if (p_handle)
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            else
                Handle.Clear();
            return;
        }
        NoRenderNextFrame = 1;
    } else {
        NoRenderNextFrame = 0;
    }

    C2DVector position(RelativeRenderPosition);
    C2DVector real_position(RelativeRenderPosition);
    C2DVector render_zoom(RelativeRenderZoom);
    real_position.X -= Width * render_zoom.X * 0.5f;
    real_position.Y -= Height * render_zoom.Y * 0.5f;
    CRGBColour colour(RenderColour);
    C2DExtentsF extend(Width * render_zoom.X, Height * render_zoom.Y);
    C2DBoxF clip_box(real_position, extend);
    clip_box.BRX = (float)GFFloatToLongNear(clip_box.BRX);
    clip_box.BRY = (float)GFFloatToLongNear(clip_box.BRY);
    clip_box.TLX = (float)GFFloatToLongNear(clip_box.TLX);
    clip_box.TLY = (float)GFFloatToLongNear(clip_box.TLY);
    if (clip_box.TLX >= CUIScreenUnitConverter::GetBackBufferWidth() || clip_box.TLY >= CUIScreenUnitConverter::GetBackBufferWidth()
        || clip_box.BRX <= 0.0f || clip_box.BRY <= 0.0f) {
        Handle.Clear();
        return;
    }

    C2DBoxF clip_box_rel(real_position, extend);
    clip_box_rel.BRX = (float)GFFloatToLongNear(clip_box_rel.BRX);
    clip_box_rel.BRY = (float)GFFloatToLongNear(clip_box_rel.BRY);
    clip_box_rel.TLX = (float)GFFloatToLongNear(clip_box_rel.TLX);
    clip_box_rel.TLY = (float)GFFloatToLongNear(clip_box_rel.TLY);
    if (CManager::GetInstance()->GetMeshBank().Get()->EntryExists(MeshIndex)) {
        if (CManager::GetInstance()->GetMeshBank().Get()->PeekMesh(MeshIndex)->GetDataType() == 5) {
            C3DAnimationBlendState anim_blend;
            anim_blend.SetBaseAnimation(C3DAnimationState(AnimationIndex, 0.0f));
            C3DBoundingSphere bounding_sphere(C3DVector(0.0f, 0.0f, 0.0f), 1.0f);
            CEnginePrimitive2DAnimatingMesh primitive(0, MeshIndex, anim_blend, clip_box_rel, bounding_sphere,
                                                      C3DOrientationInterpolator(Orientation, Orientation), (EEngineInterpolationSet)0,
                                                      colour.A, CRGBColour(255, 255, 255, 255));
            engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        } else {
            CEnginePrimitive2DStaticMesh primitive(0, MeshIndex, clip_box_rel, C3DOrientationInterpolator(Orientation, Orientation),
                                                   (EEngineInterpolationSet)0, colour.A, CRGBColour(255, 255, 255, 255));
            if (p_handle && pprimitive_index)
                engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
            else
                engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        }
    }
}
}