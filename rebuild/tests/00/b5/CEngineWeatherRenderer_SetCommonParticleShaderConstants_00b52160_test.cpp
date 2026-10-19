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

#include <cstdio>
#include <cstddef>
struct Upload { long Index; float Values[4]; };
static Upload uploads[16]; static int count;
CShaderRenderManager manager,otherManager; CShaderRenderManager* gShaderRenderManager=&manager;
static bool liveMode,liveOK=true;static long* activeIndex;static CWeatherMask* activeMasks;
void CShaderRenderManager::SetVertexShaderConstant(long i,float x,float y,float z,float w) {
 if(liveMode){liveOK=liveOK&&this==(count==0?&manager:&otherManager)&&*activeIndex==i+1;
 if(count==0){*activeIndex+=10;gShaderRenderManager=&otherManager;activeMasks[0].InvTransform[2][1]=-7.0f;}}
 uploads[count].Index=i; uploads[count].Values[0]=x;uploads[count].Values[1]=y;uploads[count].Values[2]=z;uploads[count].Values[3]=w;++count;
}
void CShaderRenderManager::SetVertexShaderConstant(long i,const C3DVector& v,float w) {SetVertexShaderConstant(i,v.X,v.Y,v.Z,w);}
int main(){
 WeatherRendererMethods r; C3DVector& span=*reinterpret_cast<C3DVector*>(r.Settings+0xc0);span.X=11;span.Y=-22;span.Z=33;
 CWeatherMask* masks=reinterpret_cast<CWeatherMask*>(r.WeatherMaskList);
 for(int m=0;m<3;++m){
  masks[m].MinCoord.X=100+m;masks[m].MinCoord.Y=200+m;masks[m].MinCoord.Z=300+m;
  masks[m].MaxCoord.X=400+m;masks[m].MaxCoord.Y=500+m;masks[m].MaxCoord.Z=600+m;
  for(int row=0;row<4;++row)for(int col=0;col<3;++col)masks[m].InvTransform[row][col]=float(1000*m+10*row+col);
 }
 long index=7;r.SetCommonParticleShaderConstants(&index);
 bool ok=count==16&&index==23;
 for(int k=0;k<16;++k)ok=ok&&uploads[k].Index==7+k;
 ok=ok&&uploads[0].Values[0]==11&&uploads[0].Values[1]==-22&&uploads[0].Values[2]==33&&uploads[0].Values[3]==8388608.0f;
 for(int m=0;m<3;++m){for(int col=0;col<3;++col)for(int row=0;row<4;++row)ok=ok&&uploads[1+5*m+col].Values[row]==float(1000*m+10*row+col);
 for(int j=0;j<3;++j){ok=ok&&uploads[4+5*m].Values[j]==float(100*(j+1)+m);ok=ok&&uploads[5+5*m].Values[j]==float(100*(j+4)+m);}ok=ok&&uploads[4+5*m].Values[3]==0&&uploads[5+5*m].Values[3]==0;}
#ifdef _MSC_VER
 ok=ok&&offsetof(CEngineWeatherRenderer,Settings)==0x114&&offsetof(CEngineWeatherRenderer,WeatherMaskList)==0x66c;
#endif
 ok=ok&&sizeof(CWeatherMask)==92&&offsetof(CWeatherMask,MinCoord)==0x10&&offsetof(CWeatherMask,MaxCoord)==0x1c&&offsetof(CWeatherMask,InvTransform)==0x28;
 count=0;index=7;activeIndex=&index;activeMasks=masks;liveMode=true;r.SetCommonParticleShaderConstants(&index);
 ok=ok&&liveOK&&count==16&&index==33&&uploads[0].Index==7&&uploads[2].Values[2]==-7.0f;
 for(int k=1;k<16;++k)ok=ok&&uploads[k].Index==17+k;
 std::printf("00b52160_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}