// CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&) -- queue an enabled nonempty sprite trail.
// Retail: 0x00bca6b0,188 bytes,thiscall.
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerSpriteTrails@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEnginePrimitiveManagerSpriteTrails.h"
#include "engine/CEngineSubPrimitiveSpriteTrail.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
namespace NEngineInternal {
 extern CEngineCamera* GPCamera;
 extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
}
inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera(){return *NEngineInternal::GPCamera;}
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& world_pos){return (world_pos-GetCurrentCamera().PeekCameraPos()).GetMagnitude();}
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* subprim,float distance,unsigned char primitive_alpha,unsigned long render_flags){NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(subprim,distance,primitive_alpha,render_flags,this);}
ERendered CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base,EEngineRenderLayer layer,CEngineSubPrimitiveChain& subprim_chain)
{
 if(!GetEnable())return RENDERED_NOTHING;
 CEngineInternalPrimitiveSpriteTrail& primitive=(CEngineInternalPrimitiveSpriteTrail&)primitive_base;
 if(!primitive.Trail.empty()) {
  C3DVector world_pos=primitive.Trail[0].Position;
  float distance=CalcDistanceFromCamera(world_pos);
  AddSubPrimitive(new CEngineSubPrimitiveSpriteTrail(&primitive),distance,0xff,1);
 }
 return RENDERED_OBJECT;
}