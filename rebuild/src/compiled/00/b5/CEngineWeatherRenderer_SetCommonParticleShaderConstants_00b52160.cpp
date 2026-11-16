// Leaf: ?SetCommonParticleShaderConstants@CEngineWeatherRenderer@@AAEXAAJ@Z
// CEngineWeatherRenderer::SetCommonParticleShaderConstants(long& index) -- upload span and three clipping masks.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 00b52160, 235 bytes. Shared PDB layout agrees: Settings+0xc0=ParticleBoxSpan;
// WeatherMaskList+0x28=InvTransform. Calls 00989b00 four-float and 00989a60 vector shader setters.
// NF35 (2026-11-16): renamed from the stand-in WeatherRendererMethods::SetCommonParticleShaderConstants(long*) to Ego_r.pdb + FableWin.pdb
//   ?SetCommonParticleShaderConstants@CEngineWeatherRenderer@@AAEXAAJ@Z (private, long&) and moved onto engine/CEngineWeatherRenderer.h
//   (RENDERRAIN typed layout: Settings.ParticleBoxSpan +0x114+0xc0, WeatherMaskList CWeatherMask[3] +0x66c); the five-float constant
//   setter 0x00989B00 is ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z (Ego_r + FableWin).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
class CShaderRenderManager { public:
 void SetUserVertexShaderConstant(unsigned long,float,float,float,float); // 0x00989b00
 void SetVertexShaderConstant(long,const C3DVector&,float); // 0x00989a60
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; } // 0x01436e14
void CEngineWeatherRenderer::SetCommonParticleShaderConstants(long& index) {
 const C3DVector& span=Settings.ParticleBoxSpan;
 NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++,span.X,span.Y,span.Z,8388608.0f);
 CWeatherMask* mask=WeatherMaskList;
 for(int n=3;n;--n,++mask) {
  NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++,mask->InvTransform.E11,mask->InvTransform.E21,mask->InvTransform.E31,mask->InvTransform.E41);
  NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++,mask->InvTransform.E12,mask->InvTransform.E22,mask->InvTransform.E32,mask->InvTransform.E42);
  NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++,mask->InvTransform.E13,mask->InvTransform.E23,mask->InvTransform.E33,mask->InvTransform.E43);
  NEngineInternal::GPShaderRenderManager->SetVertexShaderConstant(index++,mask->BBox.PeekMinCoord(),0.0f);
  NEngineInternal::GPShaderRenderManager->SetVertexShaderConstant(index++,mask->BBox.PeekMaxCoord(),0.0f);
 }
}
