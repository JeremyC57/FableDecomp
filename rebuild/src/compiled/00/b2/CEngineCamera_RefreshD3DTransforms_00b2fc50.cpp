// CEngineCamera::RefreshD3DTransforms() -- publish viewport, view, perspective, fog and identity world transforms
// Retail 0x00B2FC50, 259 bytes. Actual CEngineCamera header agrees through ProjectionTransform.
// CameraDesc donor CEngineCameraDesc: Window+0, WorldPosition+64, Near/Far/ZStart/ZEnd+88..100.
// Calls: GFABoxToVBox 009E2020; RenderManager 00A0B560/00A0B0A0/00A0B320/00A0B140;
// ShaderRenderManager 00988350/00988540/00988320/009881F0. Globals 01436E18 and 01436E14.
// Leaf: RefreshD3DTransforms
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0); its calls are the
//   shared CRenderManagerCore members SetVWindow 0x00A0B560, SetViewTransform 0x00A0B0A0 and SetPerspectiveTransform(MMMM) 0x00A0B320
//   (overlay opt-ins for the last two) and SetWorldTransform 0x00A0B140 (was CRenderManagerCore_Methods / g_RenderManagerCore, with
//   SetVWindow spelt SetViewportFloat).
#define FABLE_RETAIL_CRENDERMANAGERCORE_SETVIEWTRANSFORM
#define FABLE_RETAIL_CRENDERMANAGERCORE_SETPERSPECTIVETRANSFORM
#include "engine/CRenderManager.h"
#include "engine/CEngineCamera.h"
class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };
class CMatrix3x4 { public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 void Identity() {
  E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f;
  E11=E22=E33=1.0f;
 }
};
class CMatrix4x4 { public: float E[16]; };
class CRGBFloatColour { public: float R,G,B; };
struct CEngineCameraDesc {
 C2DBoxF Window; CMatrix3x4 RotationMatrix;
 float WorldPosition[3]; float HorizontalFOV,VerticalFOV; bool Use2DFOV;
 unsigned char alignment[3]; float NearZPlane,FarZPlane,ZBufferStart,ZBufferEnd; bool StateValid;
};
void __fastcall GFABoxToVBox(const C2DBoxF&, C2DBoxF&); // 009E2020
class CShaderRenderManager { public:
 void SetViewTransform(const CMatrix3x4&); // 00988350
 void SetProjectionTransform(const CMatrix4x4&); // 00988540
 void SetFogColour(const CRGBFloatColour&); // 00988320
 void SetWorldTransform(const CMatrix3x4&); // 009881F0
};
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; } // 01436E14
struct CEngineCamera_Methods : CEngineCamera { CEngineCameraDesc& Desc() { return *reinterpret_cast<CEngineCameraDesc*>(CameraDesc); } void RefreshD3DTransforms(); };
void CEngineCamera_Methods::RefreshD3DTransforms() {
 { C2DBoxF viewport; GFABoxToVBox(Desc().Window,viewport); NEngineInternal::GPRenderManager->SetVWindow(viewport); }
 NEngineInternal::GPRenderManager->SetViewTransform(*reinterpret_cast<CMatrix3x4*>(ViewTransform));
 NEngineInternal::GPRenderManager->SetPerspectiveTransform(Desc().NearZPlane,Desc().FarZPlane,Desc().ZBufferStart,Desc().ZBufferEnd);
 NEngineInternal::GPShaderRenderManager->SetViewTransform(*reinterpret_cast<CMatrix3x4*>(ViewTransform));
 NEngineInternal::GPShaderRenderManager->SetProjectionTransform(*reinterpret_cast<CMatrix4x4*>(ProjectionTransform));
 NEngineInternal::GPShaderRenderManager->SetFogColour(*reinterpret_cast<CRGBFloatColour*>(&Desc().WorldPosition));
 CMatrix3x4 identity; identity.Identity();
 NEngineInternal::GPRenderManager->SetWorldTransform(identity); NEngineInternal::GPShaderRenderManager->SetWorldTransform(identity);
}