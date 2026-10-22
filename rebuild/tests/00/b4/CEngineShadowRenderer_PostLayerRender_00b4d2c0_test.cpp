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

#include <stdio.h>
DisplayMethods* gDisplay;CameraMethods* gCamera;StateMethods* gState;Render2DMethods* g2D;bool gShowShadowBuffer;
static int eventCount,events[8],bad,enableInRestore;static ShadowMethods* current;
bool DisplayMethods::SetRenderTargetAsBackScreen(){events[eventCount++]=1;return false;}
void CameraMethods::RefreshD3DTransforms(){events[eventCount++]=2;}
void StateMethods::RestoreCaptureBlock(){if(current->ShadowBufferRenderStateCaptureBlock[0])bad=1;events[eventCount++]=3;if(enableInRestore)gShowShadowBuffer=true;}
bool Render2DMethods::Draw2DTexture(const C2DBoxF& b,const CRGBColour& c,const CTexture& t,unsigned long flags){
 events[eventCount++]=4;if(b.left!=1000||b.top!=1000||b.right!=20000||b.bottom!=20000||c.R!=255||c.G!=255||c.B!=255||c.A!=255||&t!=reinterpret_cast<CTexture*>(current->ShadowBufferColourTexture)||flags)bad=1;return false;
}
int main(){DisplayMethods d;CameraMethods c;StateMethods st;Render2DMethods r;gDisplay=&d;gCamera=&c;gState=&st;g2D=&r;ShadowMethods sh;current=&sh;
 for(int technique=0;technique<2;++technique)for(int layer=1;layer<=3;++layer)for(int show=0;show<2;++show)for(int mutate=0;mutate<2;++mutate){
 *reinterpret_cast<long*>(sh.RenderingTechnique)=technique;sh.ShadowBufferRenderStateCaptureBlock[0]=17;gShowShadowBuffer=show!=0;enableInRestore=mutate;bad=0;eventCount=0;CEngineComponentRenderParams p;p.Layer=layer;
 sh.PostLayerRender(p);int finish=technique&&layer==2;int draw=show||(finish&&mutate);
 if(eventCount!=finish*3+draw||bad||sh.ShadowBufferRenderStateCaptureBlock[0]!=(finish?0:17))return 1;
 for(int i=0;i<finish*3;++i)if(events[i]!=i+1)return 1;if(draw&&events[eventCount-1]!=4)return 1;
 }
 printf("00b4d2c0_TEST PASS\n");return 0;}