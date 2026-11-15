#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FABLE_FORCEINLINE inline
static unsigned sample;static int nEvents,events[128];
static void Check(bool ok){if(!ok){std::printf("00b70230_TEST FAIL case%u event%d\n",sample,nEvents);std::exit(1);}}
static void Event(int e){Check(nEvents<128);events[nEvents++]=e;}
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CRGBFloatColour{public:float R,G,B,A;CRGBFloatColour(){}CRGBFloatColour(float r,float g,float b):R(r),G(g),B(b),A(1){}CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a){}CRGBFloatColour operator*(const CRGBFloatColour& c)const{return CRGBFloatColour(R*c.R,G*c.G,B*c.B,A*c.A);}};
class C4DVector{public:float X,Y,Z,W;C4DVector(){}C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}C4DVector(const CRGBFloatColour& c):X(c.R),Y(c.G),Z(c.B),W(c.A){}};
enum EMatrix34Init{MATRIX34_INIT_ZERO,MATRIX34_INIT_IDENTITY};
class CMatrix3x4{public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;CMatrix3x4(){}CMatrix3x4(EMatrix34Init init){E12=E13=E21=E23=E31=E32=E41=E42=E43=0;E11=E22=E33=init==MATRIX34_INIT_ZERO?0.0f:1.0f;}void SetTranslateOnly(const C3DVector& v){E41=v.X;E42=v.Y;E43=v.Z;}};
static void Equal(const C4DVector& a,float x,float y,float z,float w){Check(a.X==x&&a.Y==y&&a.Z==z&&a.W==w);}
class CTexture{public:void* PD3DTexture;};class CVertexShader{public:int id;};class CPixelShader{public:int id;};
class CVertexBufferWin32{public:void* PVertexBuffer;unsigned long Stride;};class CIndexBuffer{public:void* PD3DIndexBuffer;};
class CRenderStateManager{public:
 struct CStateInfo{unsigned int CurrentState,DesiredState,BookmarkMask,RenderStateParam;bool DirtyListFlag;};
 struct CStateStackElement{CStateInfo* State;unsigned int Value;};
 struct CDeviceState{CStateInfo U[3],V[3],Mip;}DeviceState;
 CStateInfo BookmarkDummyState;CStateStackElement StateStack[32];CStateInfo* StateUpdateList[32];unsigned StateStackSize,StateUpdateListSize,CurrentBookmarkMask;
 void StartCaptureBlock(void*){StateStack[StateStackSize++].State=&BookmarkDummyState;CurrentBookmarkMask<<=1;Event(1);}
 void RestoreCaptureBlock(){Event(30);Check(StateStackSize>0);while(StateStack[StateStackSize-1].State!=&BookmarkDummyState){CStateStackElement e=StateStack[--StateStackSize];e.State->DesiredState=e.Value;e.State->BookmarkMask&=~CurrentBookmarkMask;}--StateStackSize;CurrentBookmarkMask>>=1;}
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
 void SetTextureUVModeU(long i,int mode){SetRenderState(mode==0?1:3,DeviceState.U[i]);}
 void SetTextureUVModeV(long i,int mode){SetRenderState(mode==0?1:3,DeviceState.V[i]);}
 void SetTextureMipmapLODBias(long i,float bias){Check(i==0);unsigned int word;std::memcpy(&word,&bias,4);SetRenderState(word,DeviceState.Mip);}
};
enum TEXTURE_UV_MODE{TEXTURE_UV_WRAP=0,TEXTURE_UV_CLAMP=2};
class CRenderStateCaptureBlock{public:static CRenderStateManager* StateManager;};CRenderStateManager* CRenderStateCaptureBlock::StateManager;
class CRenderStateScopedCaptureBlock{public:CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->StartCaptureBlock(this);}~CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->RestoreCaptureBlock();}};
static void* wantedTextures[2];static unsigned expectedTextureCalls;static unsigned textureCalls,streamCalls,indexCalls,drawCalls,vsCalls,psCalls,worldCalls,userCalls,envCalls,lightCalls,cameraCalls,disabledCalls;
static CVertexBufferWin32* expectedVB;static CIndexBuffer* expectedIB;static CVertexShader* expectedVS;static CPixelShader* expectedPS;static unsigned long expectedVertices,expectedPolys;static bool high;
static C4DVector expectedUser[4];static C4DVector expectedColour;
class CRenderManagerCore:public CRenderStateManager{public:
 void* CurrentlySelectedTextures[3];long TextureStagesInUse;CIndexBuffer* PSelectedIndexBuffer;
 void AttachTextureToStage(long stage,const CTexture* texture){void* tex=texture?texture->PD3DTexture:0;if(CurrentlySelectedTextures[stage]!=tex){CurrentlySelectedTextures[stage]=tex;++textureCalls;Event(2+(int)stage);}if(TextureStagesInUse<stage+1)TextureStagesInUse=stage+1;}
 void SelectVertexBuffer(unsigned long stream,CVertexBufferWin32* buffer){Check(stream==0&&buffer==expectedVB);++streamCalls;Event(6);}
 void SelectIndexBuffer(CIndexBuffer* buffer,unsigned long base){Check(buffer==expectedIB&&base==0);PSelectedIndexBuffer=buffer;++indexCalls;Event(7);}
 void DrawCurrentBuffersIndexed(unsigned long,unsigned long,unsigned long,unsigned long);
};class CRenderManager:public CRenderManagerCore{};
class CVertexShaderConstant{public:float X,Y,Z,W;};template<class T>class CArray{public:T* first;T* last;T* begin(){return first;}const T* GetAsCArray()const{return first!=last?first:(const T*)this;}unsigned long size()const{return(unsigned long)(last-first);}};
class CVertexShaderConstantLayout{public:struct{long Offset;}Reserved;CArray<CVertexShaderConstant>ReservedPresets;};
class CShaderRenderManager{public:
 unsigned long UpdateFlags;bool WorldMatrixIsIdentity;CVertexShaderConstantLayout* CurrentVSConstantLayout;C4DVector constants[12];CMatrix3x4 world;
 void ApplyVertexShader(const CVertexShader& shader,bool force){Check(&shader==expectedVS&&!force);++vsCalls;Event(4);}
 void ApplyPixelShader(const CPixelShader& shader){Check(&shader==expectedPS);++psCalls;Event(5);}
 void SetPixelShaderConstant(unsigned long i,const C4DVector& v){constants[i]=v;UpdateFlags|=2;Event(10+(int)i);}
 void SetUserVertexShaderConstant(unsigned long i,float x,float y,float z,float w){Check((i==0&&userCalls==0)||(i==3&&high&&userCalls==1));Equal(C4DVector(x,y,z,w),expectedUser[i].X,expectedUser[i].Y,expectedUser[i].Z,expectedUser[i].W);++userCalls;Event(8+(int)i);}
 void SetWorldTransform(const CMatrix3x4& m){world=m;++worldCalls;Event(19);}
 void PrepareForRender(unsigned long flags){
  if(UpdateFlags&4)SetVSConstants(CurrentVSConstantLayout->Reserved.Offset,CurrentVSConstantLayout->ReservedPresets.GetAsCArray(),CurrentVSConstantLayout->ReservedPresets.size());
  if(UpdateFlags&1)UpdatePixelShader();if(UpdateFlags&2)UpdatePixelShaderConstants();
  if((flags&0x400)&&(UpdateFlags&0x20))UpdateBoneTransform();
  if(flags&0x40){if(UpdateFlags&0x1000)UpdateWorldSpaceTextureTransform();}else if(flags&0x80){if(UpdateFlags&0x800)UpdateObjectSpaceTextureTransform();}
  if(flags&0x200){if(UpdateFlags&0x2000)UpdateWorldSpaceFogTransform();}else if(flags&0x100){if(UpdateFlags&0x4000)UpdateObjectSpaceFogTransform();}
  if(UpdateFlags&4)UpdateAmbient();if(UpdateFlags&0x20000)UpdateFogColour();if(UpdateFlags&0x400)UpdateLightGlobals();
  if(flags&8){if(UpdateFlags&0x100)UpdateObjectSpaceLighting(flags);}else if(flags&0x10){if(UpdateFlags&0x200)UpdateWorldSpaceLighting();}
  if((flags&0x20)&&(UpdateFlags&0x8000))UpdateLocalisedSphereMapping();if(!(flags&1)&&!WorldMatrixIsIdentity)ResetWorldTransform();if(UpdateFlags&0x80)UpdateCombinedProjectionTransform();if((flags&4)&&(UpdateFlags&8))UpdateCameraPos();
 }
 void SetVSConstants(long start,const CVertexShaderConstant* p,unsigned long n){Check(start==7&&n==CurrentVSConstantLayout->ReservedPresets.size()&&p==CurrentVSConstantLayout->ReservedPresets.GetAsCArray());Event(20);}
 void UpdatePixelShader(){UpdateFlags&=~1ul;Event(21);}void UpdatePixelShaderConstants(){UpdateFlags&=~2ul;Event(22);}void UpdateWorldSpaceFogTransform(){UpdateFlags&=~0x2000ul;Event(23);}void UpdateAmbient(){UpdateFlags&=~4ul;Event(24);}void UpdateFogColour(){UpdateFlags&=~0x20000ul;Event(25);}void UpdateLightGlobals(){UpdateFlags&=~0x400ul;Event(26);}void UpdateCombinedProjectionTransform(){UpdateFlags&=~0x80ul;Event(27);}void UpdateCameraPos(){UpdateFlags&=~8ul;Event(28);}
 void UpdateBoneTransform(){Check(false);}void UpdateWorldSpaceTextureTransform(){Check(false);}void UpdateObjectSpaceTextureTransform(){Check(false);}void UpdateObjectSpaceFogTransform(){Check(false);}void UpdateObjectSpaceLighting(unsigned long){Check(false);}void UpdateWorldSpaceLighting(){Check(false);}void UpdateLocalisedSphereMapping(){Check(false);}void ResetWorldTransform(){Check(false);}
};
class CIDrawEnvironmentParams{public:struct CThemeChannel{float Scalers[4];}ThemeChannels[1];};
class CIDrawEnvironment{public:CIDrawEnvironmentParams params;const CIDrawEnvironmentParams& PeekInterpolatedParams(){++envCalls;return params;}};
class CIDrawWorld{public:CIDrawEnvironment env;CIDrawEnvironment* DrawGetEnvironment()const{++envCalls;return const_cast<CIDrawEnvironment*>(&env);}};
class CEngineWorldMap{public:CIDrawWorld world;const CIDrawWorld* PeekGameWorld()const{return &world;}};
// The interface method is const in the original headers.
class CEngineLightingManager{public:CRGBFloatColour diffuse;const CRGBFloatColour& PeekGlobalDiffuseColour()const{++lightCalls;return diffuse;}};
class CEngineCamera{public:C3DVector pos;const C3DVector& PeekCameraPos()const{++cameraCalls;return pos;}};
class CGraphicFrame{public:CTexture* GetTexture()const{++disabledCalls;return 0;}};
class CGraphicInfoFrame{public:bool GetFrame(CGraphicFrame&,unsigned long,unsigned long)const{++disabledCalls;return false;}};
class CEngineResourceManager{public:const CGraphicInfoFrame* GetTerrainTexture(long){++disabledCalls;return 0;}};
class CEnvironmentDef{public:long SeaBumpMapPC;};
class CEngineWaterSettings{public:float SeaBumpmapSpan;CRGBFloatColour Colour;float ColourToReflectionFactor,BumpMatFactor;unsigned long SeaBumpMapPCFrameIndex;};
class CEngineWaterRenderer{public:
 bool HighendWaterEnabled;CTexture* SeaBumpMap;CTexture* EnvironmentMap;CEngineWaterSettings Settings;float WaterBumpFactor;CVertexShader SeaBackgroundVertexShader,SeaBackgroundLowEndVertexShader;CPixelShader SeaBackgroundPixelShader,WaterLowEndPixelShader;CVertexBufferWin32 SeaBackgroundVertexBuffer;CIndexBuffer SeaBackgroundIndexBuffer;long SeaBackgroundVertexCount,SeaBackgroundPolyCount;
 bool SetSeaBackgroundTexturesAndUVModes();void SetupSeaBackgroundShaderConstantsAndFog();void RenderSeaBackground(const float);
};
namespace NEngineInternal{CRenderManager* GPRenderManager;CShaderRenderManager* GPShaderRenderManager;CEngineWorldMap* GPEngineWorldMap;CEngineLightingManager* GPLightingManager;CEngineCamera* GPCamera;CEngineResourceManager* GPResourceManager;const CEnvironmentDef* GPEnvironmentDef;}
static float mip=-4.0f;
FABLE_FORCEINLINE bool CEngineWaterRenderer::SetSeaBackgroundTexturesAndUVModes()
{
    bool animate_bump_map=true;
    if(animate_bump_map)
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,SeaBumpMap);
    else {
        CGraphicFrame sea_bump_graphic;
        if(!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->SeaBumpMapPC)->GetFrame(sea_bump_graphic,Settings.SeaBumpMapPCFrameIndex,0))
            return false;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,sea_bump_graphic.GetTexture());
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(1,EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1,EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2,TEXTURE_UV_CLAMP);
    return true;
}
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupSeaBackgroundShaderConstantsAndFog()
{
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SeaBumpmapSpan,0.0f,0.0f,0.0f);
    if(HighendWaterEnabled) {
        C4DVector colour(Settings.Colour);
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        colour.W=params.ThemeChannels[0].Scalers[2]*Settings.ColourToReflectionFactor;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7,colour);
        float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,-bump_factor/2.0f,1.0f);
        CMatrix3x4 world_matt(MATRIX34_INIT_IDENTITY);
        world_matt.SetTranslateOnly(NEngineInternal::GPCamera->PeekCameraPos());
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(world_matt);
    } else {
        static C4DVector light_dot(0.8f,0.8f,0.0f,0.0f);
        static CRGBFloatColour colour(0.0f,0.4f,0.8f);
        CRGBFloatColour col=colour*NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,light_dot);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6,C4DVector(col));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x205);
}
void CEngineWaterRenderer::RenderSeaBackground(const float interpolate)
{
    CRenderStateScopedCaptureBlock block;
    if(SetSeaBackgroundTexturesAndUVModes()) {
        NEngineInternal::GPRenderManager->SetTextureMipmapLODBias(0,mip);
        if(HighendWaterEnabled) {
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundVertexShader,false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SeaBackgroundPixelShader);
        } else {
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundLowEndVertexShader,false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(WaterLowEndPixelShader);
        }
        NEngineInternal::GPRenderManager->SelectVertexBuffer(0,&SeaBackgroundVertexBuffer);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&SeaBackgroundIndexBuffer,0);
        SetupSeaBackgroundShaderConstantsAndFog();
        NEngineInternal::GPRenderManager->DrawCurrentBuffersIndexed(0,SeaBackgroundVertexCount,0,SeaBackgroundPolyCount);
    }
}

void CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long a,unsigned long b,unsigned long c,unsigned long d){Check(a==0&&c==0&&b==expectedVertices&&d==expectedPolys);Check(vsCalls==1&&psCalls==1&&streamCalls==1&&indexCalls==1&&PSelectedIndexBuffer==expectedIB&&userCalls==(high?2u:1u));for(unsigned i=0;i<3;++i){Check(DeviceState.U[i].DesiredState==(i?3u:1u));Check(DeviceState.V[i].DesiredState==(i?3u:1u));}Check(DeviceState.Mip.DesiredState==0xc0800000u);Check(CurrentlySelectedTextures[0]==wantedTextures[0]&&CurrentlySelectedTextures[1]==wantedTextures[1]);++drawCalls;Event(29);}
int main(){
 CEngineWaterRenderer water;CRenderManager renderer;CShaderRenderManager shader;CEngineWorldMap map;CEngineLightingManager light;CEngineCamera camera;CVertexShaderConstantLayout layout;CVertexShaderConstant presets[3];CTexture sea,env;C4DVector sentinel(-8,-16,-32,-64);
 NEngineInternal::GPRenderManager=&renderer;NEngineInternal::GPShaderRenderManager=&shader;NEngineInternal::GPEngineWorldMap=&map;NEngineInternal::GPLightingManager=&light;NEngineInternal::GPCamera=&camera;CRenderStateCaptureBlock::StateManager=&renderer;
 shader.CurrentVSConstantLayout=&layout;layout.Reserved.Offset=7;layout.ReservedPresets.first=presets;
 const unsigned long flagbits[9]={1,4,8,0x80,0x400,0x2000,0x20000,0x100,0x8000};
 for(sample=0;sample<16384;++sample){
  unsigned pattern=sample&511u,cache=(sample>>9)&3u,variant=(sample>>11)&3u;high=(sample&8192u)!=0;water.HighendWaterEnabled=high;
  unsigned long initialFlags=0;for(unsigned i=0;i<9;++i)if(pattern&(1u<<i))initialFlags|=flagbits[i];shader.UpdateFlags=initialFlags;shader.WorldMatrixIsIdentity=(variant&1)!=0;bool initialIdentity=shader.WorldMatrixIsIdentity;
  layout.ReservedPresets.last=presets+(pattern%4u);for(unsigned i=0;i<12;++i)shader.constants[i]=sentinel;
  sea.PD3DTexture=(void*)0x100;env.PD3DTexture=(void*)0x200;water.SeaBumpMap=(variant&1)?&sea:0;water.EnvironmentMap=(variant&2)?&env:0;wantedTextures[0]=water.SeaBumpMap?sea.PD3DTexture:0;wantedTextures[1]=water.EnvironmentMap?env.PD3DTexture:0;
  renderer.CurrentlySelectedTextures[0]=(cache&1)?wantedTextures[0]:(void*)0x300;renderer.CurrentlySelectedTextures[1]=(cache&2)?wantedTextures[1]:(void*)0x400;expectedTextureCalls=((cache&1)?0u:1u)+((cache&2)?0u:1u);renderer.TextureStagesInUse=(long)(pattern%4u);
  water.Settings.SeaBumpmapSpan=(float)((int)(pattern%9u)-4)*0.25f;water.Settings.Colour=CRGBFloatColour(0.25f,0.5f,0.75f,0.125f);water.Settings.ColourToReflectionFactor=0.5f;water.Settings.BumpMatFactor=(float)((int)(variant)-1)*0.5f;water.WaterBumpFactor=(float)(cache+1u)*0.25f;map.world.env.params.ThemeChannels[0].Scalers[2]=(float)(pattern%5u)*0.25f;light.diffuse=CRGBFloatColour(0.5f,0.25f,0.125f,0.75f);camera.pos=C3DVector((float)cache,-(float)variant,0.5f);
  expectedUser[0]=C4DVector(water.Settings.SeaBumpmapSpan,0,0,0);float bump=water.Settings.BumpMatFactor*water.WaterBumpFactor;expectedUser[3]=C4DVector(bump,0,-bump/2.0f,1);expectedColour=C4DVector(0.25f,0.5f,0.75f,map.world.env.params.ThemeChannels[0].Scalers[2]*0.5f);
  expectedVB=&water.SeaBackgroundVertexBuffer;expectedIB=&water.SeaBackgroundIndexBuffer;expectedVS=high?&water.SeaBackgroundVertexShader:&water.SeaBackgroundLowEndVertexShader;expectedPS=high?&water.SeaBackgroundPixelShader:&water.WaterLowEndPixelShader;water.SeaBackgroundVertexCount=(long)(pattern+1u);water.SeaBackgroundPolyCount=(long)(variant*13u);expectedVertices=(unsigned long)water.SeaBackgroundVertexCount;expectedPolys=(unsigned long)water.SeaBackgroundPolyCount;
  CRenderStateManager::CStateInfo* states[7]={&renderer.DeviceState.U[0],&renderer.DeviceState.V[0],&renderer.DeviceState.U[1],&renderer.DeviceState.V[1],&renderer.DeviceState.U[2],&renderer.DeviceState.V[2],&renderer.DeviceState.Mip};unsigned int before[7],oldmask[7];bool dirty[7];unsigned expectQueued=0;
  for(unsigned i=0;i<7;++i){unsigned desired=i==6?0xc0800000u:(i<2?1u:3u);before[i]=((pattern>>i)&1u)?desired:99u+i;states[i]->CurrentState=1000u+i;states[i]->DesiredState=before[i];oldmask[i]=(pattern>>(i+2))&1u;states[i]->BookmarkMask=oldmask[i];states[i]->RenderStateParam=2000u+i;dirty[i]=((pattern>>(i+1))&1u)!=0;states[i]->DirtyListFlag=dirty[i];if(before[i]!=desired&&!dirty[i])++expectQueued;}
  renderer.StateStackSize=1;renderer.StateStack[0].State=&renderer.BookmarkDummyState;renderer.StateStack[0].Value=123;renderer.StateUpdateListSize=0;renderer.CurrentBookmarkMask=1;
  nEvents=0;textureCalls=streamCalls=indexCalls=drawCalls=vsCalls=psCalls=worldCalls=userCalls=envCalls=lightCalls=cameraCalls=disabledCalls=0;
  water.RenderSeaBackground((float)variant*0.25f);
  Check(drawCalls==1&&textureCalls==expectedTextureCalls&&worldCalls==(high?1u:0u)&&envCalls==(high?2u:0u)&&lightCalls==(high?0u:1u)&&cameraCalls==(high?1u:0u)&&disabledCalls==0);Check(renderer.TextureStagesInUse>=2&&renderer.StateStackSize==1&&renderer.CurrentBookmarkMask==1&&renderer.StateUpdateListSize==expectQueued);Check(shader.WorldMatrixIsIdentity==initialIdentity);
  for(unsigned i=0;i<7;++i){unsigned desired=i==6?0xc0800000u:(i<2?1u:3u);Check(states[i]->DesiredState==before[i]&&states[i]->BookmarkMask==oldmask[i]&&states[i]->DirtyListFlag==(dirty[i]||before[i]!=desired));Check(states[i]->CurrentState==1000u+i&&states[i]->RenderStateParam==2000u+i);}
  unsigned qi=0;for(unsigned i=0;i<7;++i){unsigned desired=i==6?0xc0800000u:(i<2?1u:3u);if(before[i]!=desired&&!dirty[i])Check(renderer.StateUpdateList[qi++]==states[i]);}Check(qi==expectQueued&&renderer.StateStack[0].State==&renderer.BookmarkDummyState&&renderer.StateStack[0].Value==123);
  if(high){Equal(shader.constants[7],expectedColour.X,expectedColour.Y,expectedColour.Z,expectedColour.W);Equal(shader.constants[5],-8,-16,-32,-64);Equal(shader.constants[6],-8,-16,-32,-64);Check(shader.world.E11==1&&shader.world.E22==1&&shader.world.E33==1&&shader.world.E12==0&&shader.world.E13==0&&shader.world.E21==0&&shader.world.E23==0&&shader.world.E31==0&&shader.world.E32==0&&shader.world.E41==camera.pos.X&&shader.world.E42==camera.pos.Y&&shader.world.E43==camera.pos.Z);}else{Equal(shader.constants[5],0.8f,0.8f,0,0);Equal(shader.constants[6],0,0.4f*0.25f,0.8f*0.125f,0.75f);Equal(shader.constants[7],-8,-16,-32,-64);}
  for(unsigned i=0;i<12;++i)if(i<5||i>7)Equal(shader.constants[i],-8,-16,-32,-64);
  unsigned long cleared=1|2|4|8|0x80|0x400|0x2000|0x20000;Check(shader.UpdateFlags==(initialFlags&~cleared));
  int expected[64],n=0;expected[n++]=1;if(!(cache&1))expected[n++]=2;if(!(cache&2))expected[n++]=3;expected[n++]=4;expected[n++]=5;expected[n++]=6;expected[n++]=7;expected[n++]=8;if(high){expected[n++]=17;expected[n++]=11;expected[n++]=19;}else{expected[n++]=15;expected[n++]=16;}if(initialFlags&4)expected[n++]=20;if(initialFlags&1)expected[n++]=21;expected[n++]=22;if(initialFlags&0x2000)expected[n++]=23;if(initialFlags&4)expected[n++]=24;if(initialFlags&0x20000)expected[n++]=25;if(initialFlags&0x400)expected[n++]=26;if(initialFlags&0x80)expected[n++]=27;if(initialFlags&8)expected[n++]=28;expected[n++]=29;expected[n++]=30;Check(nEvents==n);for(int i=0;i<n;++i)Check(events[i]==expected[i]);
 }
 std::puts("00b70230_TEST PASS");return 0;
}
