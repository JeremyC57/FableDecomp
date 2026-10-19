// CEngineCamera::RefreshD3DTransforms() -- publish viewport, view, perspective, fog and identity world transforms
// Retail 0x00B2FC50, 259 bytes. Actual CEngineCamera header agrees through ProjectionTransform.
// CameraDesc donor CEngineCameraDesc: Window+0, WorldPosition+64, Near/Far/ZStart/ZEnd+88..100.
// Calls: GFABoxToVBox 009E2020; RenderManager 00A0B560/00A0B0A0/00A0B320/00A0B140;
// ShaderRenderManager 00988350/00988540/00988320/009881F0. Globals 01436E18 and 01436E14.
// Leaf: RefreshD3DTransforms
#include "engine/CEngineCamera.h"
#include "engine/CRenderManagerCore.h"
struct C2DBoxF { float MinX, MinY, MaxX, MaxY; };
struct CMatrix3x4 {
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 void Identity() {
  E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f;
  E11=E22=E33=1.0f;
 }
};
struct CMatrix4x4 { float E[16]; };
struct CRGBFloatColour { float R,G,B; };
struct CEngineCameraDesc {
 C2DBoxF Window; CMatrix3x4 RotationMatrix;
 float WorldPosition[3]; float HorizontalFOV,VerticalFOV; bool Use2DFOV;
 unsigned char alignment[3]; float NearZPlane,FarZPlane,ZBufferStart,ZBufferEnd; bool StateValid;
};
void __fastcall GFABoxToVBox(const C2DBoxF&, C2DBoxF&); // 009E2020
struct CRenderManagerCore_Methods : CRenderManagerCore {
 void SetViewportFloat(const C2DBoxF&); // 00A0B560
 void SetViewTransform(const CMatrix3x4&); // 00A0B0A0
 void SetPerspectiveTransform(float,float,float,float); // 00A0B320
 void SetWorldTransform(const CMatrix3x4&); // 00A0B140
};
struct CShaderRenderManager {
 void SetViewTransform(const CMatrix3x4&); // 00988350
 void SetProjectionTransform(const CMatrix4x4&); // 00988540
 void SetFogColour(const CRGBFloatColour&); // 00988320
 void SetWorldTransform(const CMatrix3x4&); // 009881F0
};
extern CRenderManagerCore_Methods* g_RenderManagerCore; // 01436E18
extern CShaderRenderManager* g_ShaderRenderManager; // 01436E14
struct CEngineCamera_Methods : CEngineCamera { CEngineCameraDesc& Desc() { return *reinterpret_cast<CEngineCameraDesc*>(CameraDesc); } void RefreshD3DTransforms(); };
void CEngineCamera_Methods::RefreshD3DTransforms() {
 { C2DBoxF viewport; GFABoxToVBox(Desc().Window,viewport); g_RenderManagerCore->SetViewportFloat(viewport); }
 g_RenderManagerCore->SetViewTransform(*reinterpret_cast<CMatrix3x4*>(ViewTransform));
 g_RenderManagerCore->SetPerspectiveTransform(Desc().NearZPlane,Desc().FarZPlane,Desc().ZBufferStart,Desc().ZBufferEnd);
 g_ShaderRenderManager->SetViewTransform(*reinterpret_cast<CMatrix3x4*>(ViewTransform));
 g_ShaderRenderManager->SetProjectionTransform(*reinterpret_cast<CMatrix4x4*>(ProjectionTransform));
 g_ShaderRenderManager->SetFogColour(*reinterpret_cast<CRGBFloatColour*>(&Desc().WorldPosition));
 CMatrix3x4 identity; identity.Identity();
 g_RenderManagerCore->SetWorldTransform(identity); g_ShaderRenderManager->SetWorldTransform(identity);
}