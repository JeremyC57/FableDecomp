// CEngineWeatherRenderer::SetCommonParticleShaderConstants(long*) -- upload span and three clipping masks.
// Retail 00b52160, 235 bytes. Shared PDB layout agrees: Settings+0xc0=ParticleBoxSpan;
// WeatherMaskList+0x28=InvTransform. Calls 00989b00 four-float and 00989a60 vector shader setters.
#ifdef _MSC_VER
#include "engine/CEngineWeatherRenderer.h"
#else
struct CEngineWeatherRenderer { unsigned char Settings[0x558],WeatherMaskList[0x114]; };
#endif
struct C3DVector { float X,Y,Z; };
struct CWeatherMask { C3DVector Center; float Radius; C3DVector MinCoord,MaxCoord; float InvTransform[4][3]; float Scale; };
struct CShaderRenderManager {
 void SetVertexShaderConstant(long,float,float,float,float); // 0x00989b00
 void SetVertexShaderConstant(long,const C3DVector&,float); // 0x00989a60
};
extern CShaderRenderManager* gShaderRenderManager; // 0x01436e14
struct WeatherRendererMethods : CEngineWeatherRenderer { void SetCommonParticleShaderConstants(long*); };
void WeatherRendererMethods::SetCommonParticleShaderConstants(long* index) {
 C3DVector& span=*reinterpret_cast<C3DVector*>(Settings+0xc0);
 gShaderRenderManager->SetVertexShaderConstant((*index)++,span.X,span.Y,span.Z,8388608.0f);
 CWeatherMask* mask=reinterpret_cast<CWeatherMask*>(WeatherMaskList);
 for(int n=3;n;--n,++mask) {
  gShaderRenderManager->SetVertexShaderConstant((*index)++,mask->InvTransform[0][0],mask->InvTransform[1][0],mask->InvTransform[2][0],mask->InvTransform[3][0]);
  gShaderRenderManager->SetVertexShaderConstant((*index)++,mask->InvTransform[0][1],mask->InvTransform[1][1],mask->InvTransform[2][1],mask->InvTransform[3][1]);
  gShaderRenderManager->SetVertexShaderConstant((*index)++,mask->InvTransform[0][2],mask->InvTransform[1][2],mask->InvTransform[2][2],mask->InvTransform[3][2]);
  gShaderRenderManager->SetVertexShaderConstant((*index)++,mask->MinCoord,0.0f);
  gShaderRenderManager->SetVertexShaderConstant((*index)++,mask->MaxCoord,0.0f);
 }
}