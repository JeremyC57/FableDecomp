// CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- queue one sub-primitive for a sprite-trail particle group whose bounding sphere is within its draw distance and inside the view frustum, with its distance-faded alpha
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerParticleSpriteTrailGroup@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBDDD0, 699 bytes, three `ret 0Ch` (last 0x00BBE088).
// Name: Ego_r.pdb / FableWin.pdb (the Leaf symbol); vtable slot 7 of CEnginePrimitiveManager.
// Inline helpers expanded by retail: GetEnable, CalcDistanceFromCamera / GetCurrentCamera (bodies as the landed 0x00B9A4A0 defines them),
//   CEngineCamera::ClipSphere (FableWin; planes 2, 3, 0, 1), CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade (text of the landed
//   0x00BA7B30; holds the retail-asm GFFastFloatToLong), CEnginePrimitiveManager::AddSubPrimitive (as 0x00B9A4A0).
// Retail-asm: 0x00BBE01C `fld dword ptr [esp+14h]` + 0x00BBE020 `mov edx,[esp+0Ch]` + 0x00BBE024 `fistp dword ptr [edx]` (GFFastFloatToLong, shared header).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITETRAILGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_CTOR_OUTOFLINE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CMATRIX3X4
#include "fable_compiler.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"

extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;       // 0x01436E74
extern CEngineCamera*               g_pEngineCamera;                     // 0x01436EA0

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *g_pEngineCamera; }
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& position)
{
    return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* sub_primitive, float distance, unsigned char alpha, unsigned long flags)
{
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(sub_primitive, distance, alpha, flags, this);
}

ERendered CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain)
{
    if (!GetEnable())
        return RENDERED_NOTHING;
    C3DBoundingSphere sphere;
    if (prim.GetBoundingSphereWorldSpace(sphere)) {
        float distance = CalcDistanceFromCamera(sphere.GetCenter());
        if (distance - sphere.GetRadius() > prim.FadeDistance.MaxDrawDistance)
            return RENDERED_NOTHING;
        if (GetCurrentCamera().ClipSphere(sphere.GetCenter(), sphere.GetRadius())) {
            unsigned char alpha = prim.GetAlphaIncludingDistanceFade(distance);
            AddSubPrimitive(new CEngineSubPrimitiveBase(&prim), distance, alpha, 1);
            return RENDERED_OBJECT;
        }
    }
    return RENDERED_NOTHING;
}
