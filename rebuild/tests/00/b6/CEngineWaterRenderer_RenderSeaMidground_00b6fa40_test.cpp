// Full production target and two original helper definitions pasted below.
// External renderer/resource interfaces are controlled fixtures; native ABI is audited separately.
#include "fable_assert.h"   // FABLE_RETAIL_ASSERT: release form ((void)0) -- the dead arm is a row-only IL device
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_FORCEINLINE inline
static unsigned scenario;static int eventCount,events[64];
static void Check(bool b){if(!b){fprintf(stderr,"00b6fa40 case%u events%d\n",scenario,eventCount);exit(1);}}
static void Event(int x){Check(eventCount<64);events[eventCount++]=x;}
class CRGBFloatColour{public:float R,G,B,A;CRGBFloatColour(){}CRGBFloatColour(float r,float g,float b):R(r),G(g),B(b),A(1){}CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a){}CRGBFloatColour operator*(const CRGBFloatColour& c)const{return CRGBFloatColour(R*c.R,G*c.G,B*c.B,A*c.A);}};
class C4DVector{public:float X,Y,Z,W;C4DVector(){}C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}C4DVector(const CRGBFloatColour& c):X(c.R),Y(c.G),Z(c.B),W(c.A){}};
static void Equal(const C4DVector&v,float x,float y,float z,float w){Check(v.X==x&&v.Y==y&&v.Z==z&&v.W==w);}
class CTexture{public:void* PD3DTexture;};class CVertexShader{};class CPixelShader{};
enum TEXTURE_UV_MODE{TEXTURE_UV_WRAP=0,TEXTURE_UV_CLAMP=2};
class CRenderStateManager{public:
 struct CStateInfo{unsigned int CurrentState,DesiredState,BookmarkMask,RenderStateParam;bool DirtyListFlag;};
 struct CStateStackElement{CStateInfo* State;unsigned int Value;};
 CStateInfo states[8],BookmarkDummyState;CStateStackElement StateStack[32];CStateInfo* StateUpdateList[32];unsigned StateStackSize,StateUpdateListSize,CurrentBookmarkMask;
 void StartCaptureBlock(void*){StateStack[StateStackSize++].State=&BookmarkDummyState;CurrentBookmarkMask<<=1;Event(1);}
 void RestoreCaptureBlock(){Event(70);Check(StateStackSize>1);while(StateStack[StateStackSize-1].State!=&BookmarkDummyState){CStateStackElement e=StateStack[--StateStackSize];e.State->DesiredState=e.Value;e.State->BookmarkMask&=~CurrentBookmarkMask;}--StateStackSize;CurrentBookmarkMask>>=1;}
 inline void SetRenderState(unsigned long value,CStateInfo& state_info)
    {
        if(state_info.DesiredState==value) return;
        if((state_info.BookmarkMask & CurrentBookmarkMask)==0) {
            state_info.BookmarkMask|=CurrentBookmarkMask;
            StateStack[StateStackSize].State=&state_info;
            StateStack[StateStackSize].Value=state_info.DesiredState;
            ++StateStackSize;
        }
        state_info.DesiredState=value;
        AddStateToUpdateList(state_info);
    }

inline void AddStateToUpdateList(CStateInfo& state_info)
    {
        if(!state_info.DirtyListFlag) {
            state_info.DirtyListFlag=true;
            StateUpdateList[StateUpdateListSize]=&state_info;
            ++StateUpdateListSize;
        }
    }
 void SetTextureUVModeU(long i,TEXTURE_UV_MODE mode){Check(i>=0&&i<3);SetRenderState(mode==TEXTURE_UV_WRAP?1u:3u,states[2*i]);}
 void SetTextureUVModeV(long i,TEXTURE_UV_MODE mode){Check(i>=0&&i<3);SetRenderState(mode==TEXTURE_UV_WRAP?1u:3u,states[2*i+1]);}
 void SetAlphaBlendEnable(bool v){Check(!v);Event(62);SetRenderState(v?1:0,states[6]);}
 void SetAlphaTestEnable(bool v){Check(!v);Event(63);SetRenderState(v?1:0,states[7]);}
};
class CRenderStateCaptureBlock{public:static CRenderStateManager* StateManager;bool Open;CRenderStateCaptureBlock():Open(false){}~CRenderStateCaptureBlock(){if(Open)Restore();}void Capture(){StateManager->StartCaptureBlock(this);Open=true;}void Restore(){StateManager->RestoreCaptureBlock();Open=false;}};
CRenderStateManager* CRenderStateCaptureBlock::StateManager;
struct CRenderStateScopedCaptureBlock{CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->StartCaptureBlock(this);}~CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->RestoreCaptureBlock();}};
class CRenderManager:public CRenderStateManager{public:void* textures[3];long stages;void AttachTextureToStage(long stage,const CTexture*t){Check(stage>=0&&stage<3);void*p=t?t->PD3DTexture:0;if(textures[stage]!=p){textures[stage]=p;Event(10+(int)stage);}if(stages<stage+1)stages=stage+1;}};
static CVertexShader* wantedVS;static CPixelShader* wantedPS;static unsigned userCalls,pixelCalls,envCalls,lightCalls,disabledCalls,renderCalls;static float wantedInterpolate;static unsigned firstAlpha[2];
class CShaderRenderManager{public:C4DVector user[4],pixel[8];void ApplyVertexShader(const CVertexShader&s,bool b){Check(&s==wantedVS&&!b);Event(20);}void ApplyPixelShader(const CPixelShader&s){Check(&s==wantedPS);Event(21);}void SetUserVertexShaderConstant(unsigned long i,float x,float y,float z,float w){Check(i==0||i==3);user[i]=C4DVector(x,y,z,w);++userCalls;Event(30+(int)i);}void SetPixelShaderConstant(unsigned long i,const C4DVector&v){Check(i==5||i==6||i==7);pixel[i]=v;++pixelCalls;Event(40+(int)i);}void PrepareForRender(unsigned long f){Check(f==0x204);Event(50);}};
class CIDrawEnvironmentParams{public:struct CThemeChannel{float Scalers[4];}ThemeChannels[1];};
class CIDrawEnvironment{public:CIDrawEnvironmentParams params;const CIDrawEnvironmentParams&PeekInterpolatedParams()const{++envCalls;return params;}};
class CIDrawWorld{public:CIDrawEnvironment env;const CIDrawEnvironment*DrawGetEnvironment()const{++envCalls;return &env;}};
class CEngineWorldMap{public:CIDrawWorld world;const CIDrawWorld*PeekGameWorld()const{return &world;}};
class CEngineLightingManager{public:CRGBFloatColour diffuse;const CRGBFloatColour&PeekGlobalDiffuseColour()const{++lightCalls;return diffuse;}};
class CEngineLandscapeRenderer{public:struct CSettings{C4DVector BackgroundFadeTransform;}settings;const CSettings&GetSettings()const{return settings;}};
class CGraphicFrame{public:CTexture*GetTexture()const{++disabledCalls;return 0;}};
class CGraphicInfoFrame{public:bool GetFrame(CGraphicFrame&,unsigned long,unsigned long)const{++disabledCalls;return false;}};
class CEngineResourceManager{public:const CGraphicInfoFrame*GetTerrainTexture(long){++disabledCalls;return 0;}};
class CEnvironmentDef{public:long SeaBumpMapPC;};
class CEngineWaterSettings{public:float SeaBumpmapSpan;CRGBFloatColour Colour;float ColourToReflectionFactor,BumpMatFactor;unsigned long SeaBumpMapPCFrameIndex;};
class CEngineWaterBackgroundSubPatch{};template<class T>class CArray{};
class CEngineWaterRenderer{public:bool HighendWaterEnabled;CTexture*SeaBumpMap;CTexture*EnvironmentMap;CEngineWaterSettings Settings;float WaterBumpFactor;CVertexShader SeaBackgroundVertexShader,SeaBackgroundLowEndVertexShader;CPixelShader SeaBackgroundPixelShader,WaterLowEndPixelShader;CArray<CEngineWaterBackgroundSubPatch*> SeaAlphaedMidgroundRenderList,SeaMidgroundRenderList;
 bool SetSeaMidgroundTexturesAndUVModes();void SetupSeaMidgroundShaderConstantsAndFog();void RenderSeaMidground(float);void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>&,float);
};
namespace NEngineInternal{CRenderManager*GPRenderManager;CShaderRenderManager*GPShaderRenderManager;CEngineWorldMap*GPEngineWorldMap;CEngineLightingManager*GPLightingManager;CEngineLandscapeRenderer*GPLandscapeRenderer;CEngineResourceManager*GPResourceManager;const CEnvironmentDef*GPEnvironmentDef;}
FABLE_FORCEINLINE bool CEngineWaterRenderer::SetSeaMidgroundTexturesAndUVModes()
{
    bool animate_bump_map=true;
    if(animate_bump_map) {
        FABLE_RETAIL_ASSERT(SeaBumpMap);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, SeaBumpMap);
    }
    else {
        CGraphicFrame sea_bump_graphic;
        if(!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->SeaBumpMapPC)->GetFrame(sea_bump_graphic,Settings.SeaBumpMapPCFrameIndex,0))
            return false;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,sea_bump_graphic.GetTexture());
    }
    FABLE_RETAIL_ASSERT(EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    return true;
}
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupSeaMidgroundShaderConstantsAndFog()
{
    const float fade_mult=NEngineInternal::GPLandscapeRenderer->GetSettings().BackgroundFadeTransform.X;
    const float fade_offset=NEngineInternal::GPLandscapeRenderer->GetSettings().BackgroundFadeTransform.W;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SeaBumpmapSpan,fade_mult,fade_offset,1.0f);
    if(HighendWaterEnabled) {
        float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,-bump_factor/2.0f,1.0f);
        C4DVector colour(Settings.Colour);
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        colour.W=params.ThemeChannels[0].Scalers[2]*Settings.ColourToReflectionFactor;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7,colour);
    } else {
        static C4DVector light_dot(0.8f,0.8f,0.0f,0.0f);
        static CRGBFloatColour colour(0.0f,0.4f,0.8f);
        CRGBFloatColour col=colour*NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,light_dot);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6,C4DVector(col));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x204);
}
void CEngineWaterRenderer::RenderSeaMidground(const float interpolate)
{
    CRenderStateScopedCaptureBlock block;
    if(SetSeaMidgroundTexturesAndUVModes()) {
    if (HighendWaterEnabled) {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SeaBackgroundPixelShader);
    } else {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundLowEndVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(WaterLowEndPixelShader);
    }
        SetupSeaMidgroundShaderConstantsAndFog();
        RenderBackgroundList(SeaAlphaedMidgroundRenderList,interpolate);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
        RenderBackgroundList(SeaMidgroundRenderList,interpolate);
    }
}

void CEngineWaterRenderer::RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list,float interpolate){
 Check(interpolate==wantedInterpolate);CRenderManager& r=*NEngineInternal::GPRenderManager;
 for(unsigned i=0;i<6;++i)Check(r.states[i].DesiredState==(i<2?1u:3u));
 Check(r.textures[0]==(SeaBumpMap?SeaBumpMap->PD3DTexture:0)&&r.textures[1]==(EnvironmentMap?EnvironmentMap->PD3DTexture:0)&&r.textures[2]==r.textures[1]);
 if(renderCalls==0){Check(&list==&SeaAlphaedMidgroundRenderList&&r.states[6].DesiredState==firstAlpha[0]&&r.states[7].DesiredState==firstAlpha[1]);Event(61);}
 else{Check(renderCalls==1&&&list==&SeaMidgroundRenderList&&r.states[6].DesiredState==0&&r.states[7].DesiredState==0);Event(64);}++renderCalls;
}
int main(){
 CEngineWaterRenderer water;CRenderManager renderer;CShaderRenderManager shader;CEngineWorldMap world;CEngineLightingManager light;CEngineLandscapeRenderer landscape;CTexture sea,env;int textureIDs[5]={0,1,2,3,4};
 NEngineInternal::GPRenderManager=&renderer;NEngineInternal::GPShaderRenderManager=&shader;NEngineInternal::GPEngineWorldMap=&world;NEngineInternal::GPLightingManager=&light;NEngineInternal::GPLandscapeRenderer=&landscape;CRenderStateCaptureBlock::StateManager=&renderer;
 for(scenario=0;scenario<16384;++scenario){
  unsigned pattern=scenario&255u,cache=(scenario>>8)&7u,variant=(scenario>>11)&3u;bool high=(scenario&8192u)!=0;water.HighendWaterEnabled=high;
  sea.PD3DTexture=&textureIDs[0];env.PD3DTexture=&textureIDs[1];water.SeaBumpMap=(variant&1)?&sea:0;water.EnvironmentMap=(variant&2)?&env:0;
  void*wanted[3]={water.SeaBumpMap?sea.PD3DTexture:0,water.EnvironmentMap?env.PD3DTexture:0,water.EnvironmentMap?env.PD3DTexture:0};
  for(unsigned i=0;i<3;++i)renderer.textures[i]=(cache&(1u<<i))?wanted[i]:&textureIDs[i+2];renderer.stages=(long)(pattern%4u);
  wantedVS=high?&water.SeaBackgroundVertexShader:&water.SeaBackgroundLowEndVertexShader;wantedPS=high?&water.SeaBackgroundPixelShader:&water.WaterLowEndPixelShader;
  water.Settings.SeaBumpmapSpan=(float)((int)(pattern%9)-4)*0.25f;water.Settings.Colour=CRGBFloatColour(0.25f,0.5f,0.75f,0.125f);water.Settings.ColourToReflectionFactor=0.5f;water.Settings.BumpMatFactor=(float)((int)variant-1)*0.5f;water.WaterBumpFactor=(float)(cache+1)*0.25f;
  world.world.env.params.ThemeChannels[0].Scalers[2]=(float)(pattern%5)*0.25f;light.diffuse=CRGBFloatColour(0.5f,0.25f,0.125f,0.75f);landscape.settings.BackgroundFadeTransform=C4DVector((float)(pattern%7)*0.25f,17,19,-(float)(pattern%3)*0.5f);
  for(unsigned i=0;i<4;++i)shader.user[i]=C4DVector(-8,-16,-32,-64);for(unsigned i=0;i<8;++i)shader.pixel[i]=C4DVector(-8,-16,-32,-64);
  unsigned before[8],oldmask[8];bool dirty[8];unsigned queued=0;
  for(unsigned i=0;i<8;++i){unsigned desired=i>=6?0:(i<2?1:3);before[i]=(pattern&(1u<<i))?desired:99+i;oldmask[i]=(pattern>>((i+2)%8))&1u;dirty[i]=((pattern>>((i+1)%8))&1)!=0;CRenderStateManager::CStateInfo& st=renderer.states[i];st.CurrentState=1000+i;st.DesiredState=before[i];st.BookmarkMask=oldmask[i];st.DirtyListFlag=dirty[i];st.RenderStateParam=2000+i;if(before[i]!=desired&&!dirty[i])++queued;}
  firstAlpha[0]=before[6];firstAlpha[1]=before[7];renderer.StateStackSize=1;renderer.StateStack[0].State=&renderer.BookmarkDummyState;renderer.StateStack[0].Value=123;renderer.CurrentBookmarkMask=1;renderer.StateUpdateListSize=0;
  eventCount=0;userCalls=pixelCalls=envCalls=lightCalls=disabledCalls=renderCalls=0;wantedInterpolate=(float)variant*0.25f;water.RenderSeaMidground(wantedInterpolate);
  Check(renderCalls==2&&userCalls==(high?2u:1u)&&pixelCalls==(high?1u:2u)&&envCalls==(high?2u:0u)&&lightCalls==(high?0u:1u)&&disabledCalls==0);
  Check(renderer.stages>=3&&renderer.StateStackSize==1&&renderer.CurrentBookmarkMask==1&&renderer.StateUpdateListSize==queued&&renderer.StateStack[0].State==&renderer.BookmarkDummyState&&renderer.StateStack[0].Value==123);
  unsigned qi=0;for(unsigned i=0;i<8;++i){unsigned desired=i>=6?0:(i<2?1:3);CRenderStateManager::CStateInfo&st=renderer.states[i];Check(st.DesiredState==before[i]&&st.BookmarkMask==oldmask[i]&&st.DirtyListFlag==(dirty[i]||before[i]!=desired)&&st.CurrentState==1000+i&&st.RenderStateParam==2000+i);if(before[i]!=desired&&!dirty[i])Check(renderer.StateUpdateList[qi++]==&st);}Check(qi==queued);
  Equal(shader.user[0],water.Settings.SeaBumpmapSpan,landscape.settings.BackgroundFadeTransform.X,landscape.settings.BackgroundFadeTransform.W,1);
  if(high){float bump=water.Settings.BumpMatFactor*water.WaterBumpFactor;Equal(shader.user[3],bump,0,-bump/2.0f,1);Equal(shader.pixel[7],0.25f,0.5f,0.75f,world.world.env.params.ThemeChannels[0].Scalers[2]*0.5f);Equal(shader.pixel[5],-8,-16,-32,-64);Equal(shader.pixel[6],-8,-16,-32,-64);}
  else{Equal(shader.user[3],-8,-16,-32,-64);Equal(shader.pixel[5],0.8f,0.8f,0,0);Equal(shader.pixel[6],0,0.4f*0.25f,0.8f*0.125f,0.75f);Equal(shader.pixel[7],-8,-16,-32,-64);}
  for(unsigned i=0;i<5;++i)Equal(shader.pixel[i],-8,-16,-32,-64);Equal(shader.user[1],-8,-16,-32,-64);Equal(shader.user[2],-8,-16,-32,-64);
  int expect[32],n=0;expect[n++]=1;for(unsigned i=0;i<3;++i)if(!(cache&(1u<<i)))expect[n++]=10+i;expect[n++]=20;expect[n++]=21;expect[n++]=30;if(high){expect[n++]=33;expect[n++]=47;}else{expect[n++]=45;expect[n++]=46;}expect[n++]=50;expect[n++]=61;expect[n++]=62;expect[n++]=63;expect[n++]=64;expect[n++]=70;Check(n==eventCount);for(int i=0;i<n;++i)Check(expect[i]==events[i]);
 }
 puts("00b6fa40_TEST PASS");return 0;
}