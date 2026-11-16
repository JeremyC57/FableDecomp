// 00b4a700 CEnginePrimitiveRenderer2D::Render
// Flags: none
// Draw 2D primitives with temporary camera and lighting state.
// Leaf: ?Render@CEnginePrimitiveRenderer2D@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe0x00B4A700,926bytes,thiscall ret4.
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_RENDER_PASS23
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_GLOBAL_LIGHTING_PASS23
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_DELAYS_PASS23
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RENDERTYPE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_NEXT_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/EGraphicBankNo.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineSubPrimitiveChain.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;
extern CEngineCamera* GPCamera;
extern CEngineLightingManager* GPLightingManager;
extern CDisplayManager* GPDisplayManager;
extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
}
using namespace NEngineInternal;
void CEnginePrimitiveRenderer2D::Render(const CEngineComponentRenderParams& params)
{
 CRenderStateScopedCaptureBlock block;
 CScopedShadersEnable shaders_enabled(true,true);
 CGraphicDataBank& graphic_bank=*GPResourceManager->GetGraphicBank(MAIN_GRAPHIC_BANK_NO);
 unsigned long old_texture_resize_delay=graphic_bank.GetReduceSizeFrameDelay();
 unsigned long old_texture_unload_delay=graphic_bank.GetUnloadFrameDelay();
 graphic_bank.SetReduceSizeFrameDelay(1);
 graphic_bank.SetUnloadFrameDelay(1);
 CEngineCameraDesc old_camera_desc(GPCamera->PeekCameraState());
 CEngineInternalPrimitiveBase* current_prim;
 CEngineCameraDesc new_camera_desc;
 new_camera_desc.HorizontalFOV=FOV2D;
 new_camera_desc.StateValid=true;
 new_camera_desc.NearZPlane=.1f;
 new_camera_desc.FarZPlane=1;
 new_camera_desc.ZBufferStart=0;
 new_camera_desc.ZBufferEnd=.1f;
 new_camera_desc.Use2DFOV=false;
 new_camera_desc.VerticalFOV=0;
 new_camera_desc.WorldPosition.X=0;
 new_camera_desc.WorldPosition.Y=0;
 new_camera_desc.WorldPosition.Z=0;
 new_camera_desc.RotationMatrix.SetIdentity();
 new_camera_desc.RotationMatrix.E22=-1;
 C2DExtentsI render_target_extents=GPDisplayManager->GetRenderTargetDimensions();
 new_camera_desc.Window=C2DBoxF(0,0,(float)render_target_extents.X,(float)render_target_extents.Y);
 GPCamera->SetCameraState(new_camera_desc,false,true);
 C3DVector old_light_vector=GPLightingManager->PeekDiffuseVector();
 CRGBFloatColour old_ambient,old_backlight,old_diffuse;
 old_ambient=GPLightingManager->PeekGlobalAmbientColour();
 old_diffuse=GPLightingManager->PeekGlobalDiffuseColour();
 old_backlight=GPLightingManager->PeekGlobalBacklightColour();
 GPLightingManager->SetGlobalLightingParameters(LightVector,AmbientLightColour,DiffuseLightColour,BackLightColour);
 if(params.RenderLayer==RENDER_LAYER_EFFECTS){
  current_prim=First2DDisplacementPrim;
  while(current_prim){
   CEnginePrimitiveManager* prim_manager=GPPrimitiveTypeInfo->GetPrimitiveManager(current_prim->GetType());
   CEngineSubPrimitiveChain subprim_chain;
   prim_manager->Render2DPrimitive(*current_prim,subprim_chain,SUBPRIM_TYPE_2D);
   current_prim=current_prim->GetNextPrimitive();
  }
 }else{
  current_prim=First2DPrim;
  while(current_prim){
   CEnginePrimitiveManager* prim_manager=GPPrimitiveTypeInfo->GetPrimitiveManager(current_prim->GetType());
   CEngineSubPrimitiveChain subprim_chain;
   prim_manager->Render2DPrimitive(*current_prim,subprim_chain,SUBPRIM_TYPE_2D);
   current_prim=current_prim->GetNextPrimitive();
  }
  GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_2D,true);
 }
 GPLightingManager->SetGlobalLightingParameters(old_light_vector,old_ambient,old_diffuse,old_backlight);
 GPCamera->SetCameraState(old_camera_desc,false,true);
 graphic_bank.SetReduceSizeFrameDelay(old_texture_resize_delay);
 graphic_bank.SetUnloadFrameDelay(old_texture_unload_delay);
}