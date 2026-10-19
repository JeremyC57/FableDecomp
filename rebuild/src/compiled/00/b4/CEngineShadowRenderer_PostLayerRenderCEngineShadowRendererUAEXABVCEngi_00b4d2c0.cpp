// CEngineShadowRenderer::PostLayerRender -- finish shadow pass on layer2, optionally draw its colour texture.
// Retail00b4d2c0,162bytes; shared shadow layout334technique,24ccapture,318texture.
// Calls9bf220SetRenderTargetAsBackScreen,b2fc50RefreshD3DTransforms,a05840RestoreCaptureBlock,9dc6e0Draw2DTexture.
#ifdef _MSC_VER
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CRenderStateManager.h"
#include "engine/CTexture.h"
#else
struct CEngineShadowRenderer {unsigned char ShadowBufferRenderStateCaptureBlock[4];unsigned char ShadowBufferColourTexture[8];long RenderingTechnique[1];};
struct CEngineCamera {};struct CRenderStateManager {};struct CTexture {};
#endif
#ifdef _MSC_VER
#include "fable_display_viewport.h"
#else
struct C2DBoxF {float left,top,right,bottom;};
#endif
struct BoxMethods:C2DBoxF {BoxMethods(float l,float t,float r,float b){left=l;top=t;right=r;bottom=b;}};
struct CRGBColour {unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){B=b;G=g;R=r;A=a;}};
struct DisplayMethods {bool SetRenderTargetAsBackScreen();};
struct CameraMethods:CEngineCamera {void RefreshD3DTransforms();};
struct StateMethods:CRenderStateManager {void RestoreCaptureBlock();};
struct Render2DMethods {bool Draw2DTexture(const C2DBoxF&,const CRGBColour&,const CTexture&,unsigned long);};
extern DisplayMethods* gDisplay; //01436e1c
extern CameraMethods* gCamera; //01436ea0
extern StateMethods* gState; //013cb508
extern Render2DMethods* g2D; //01436e18
extern bool gShowShadowBuffer; //01437800
struct CEngineComponentRenderParams {long Unknown;long Layer;};
struct ShadowMethods:CEngineShadowRenderer {void PostLayerRender(const CEngineComponentRenderParams&);};
void ShadowMethods::PostLayerRender(const CEngineComponentRenderParams& params) {
 if(*reinterpret_cast<long*>(RenderingTechnique) && params.Layer==2){
  gDisplay->SetRenderTargetAsBackScreen();gCamera->RefreshD3DTransforms();ShadowBufferRenderStateCaptureBlock[0]=0;gState->RestoreCaptureBlock();
 }
 if(gShowShadowBuffer){
  g2D->Draw2DTexture(BoxMethods(1000,1000,20000,20000),CRGBColour(255,255,255,255),*reinterpret_cast<CTexture*>(ShadowBufferColourTexture),0);
 }
}