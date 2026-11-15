#include <assert.h>
#include <stdio.h>
#include <string.h>
#define FABLE_FORCEINLINE inline
static int Events[128],EventCount,GraphicQueries,FrameCalls,Releases,Scenario;
static void Event(int n){assert(EventCount<128);Events[EventCount++]=n;}
struct C4DVector{float X,Y,Z,W;C4DVector(){}C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}};
static bool EqualFloat(float a,float b){return a==b||(a!=a&&b!=b);}
static void EqualVector(const C4DVector&a,float x,float y,float z,float w){assert(EqualFloat(a.X,x)&&EqualFloat(a.Y,y)&&EqualFloat(a.Z,z)&&EqualFloat(a.W,w));}
struct CTexture{void*PD3DTexture;};struct CVertexShader{};struct CPixelShader{int ID;};struct CIndexBuffer{};struct CMatrix3x4{int ID;};
enum TEXTURE_UV_MODE{TEXTURE_UV_WRAP=0,TEXTURE_UV_MIRROR=1,TEXTURE_UV_CLAMP=2};enum EGraphicBankNo{FixtureTerrain=0};
struct CRenderStateManager{
 struct State{unsigned Value,Mask;bool Dirty;};struct Saved{State*P;unsigned Value;};State States[8];Saved Stack[16];unsigned StackSize,Mask,UpdateCount;
 void StartCaptureBlock(void*){assert(StackSize==1&&Mask==1);Stack[StackSize++].P=0;Mask<<=1;Event(1);}
 void RestoreCaptureBlock(){while(Stack[StackSize-1].P){Saved s=Stack[--StackSize];s.P->Value=s.Value;s.P->Mask&=~Mask;}--StackSize;Mask>>=1;Event(250);}
 void Set(unsigned v,State&s){if(s.Value!=v){if(!(s.Mask&Mask)){s.Mask|=Mask;Stack[StackSize].P=&s;Stack[StackSize++].Value=s.Value;}s.Value=v;if(!s.Dirty){s.Dirty=true;++UpdateCount;}}}
 void SetTextureUVModeU(long i,TEXTURE_UV_MODE v){assert(i>=0&&i<4);Event(40+(int)i*2);Set((unsigned)v+1,States[i*2]);}
 void SetTextureUVModeV(long i,TEXTURE_UV_MODE v){assert(i>=0&&i<4);Event(41+(int)i*2);Set((unsigned)v+1,States[i*2+1]);}
};
struct CRenderStateCaptureBlock{static CRenderStateManager*StateManager;bool Open;CRenderStateCaptureBlock():Open(false){}~CRenderStateCaptureBlock(){if(Open)Restore();}void Capture(){StateManager->StartCaptureBlock(this);Open=true;}void Restore(){StateManager->RestoreCaptureBlock();Open=false;}};
CRenderStateManager*CRenderStateCaptureBlock::StateManager;
struct CRenderStateScopedCaptureBlock{CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->StartCaptureBlock(this);}~CRenderStateScopedCaptureBlock(){CRenderStateCaptureBlock::StateManager->RestoreCaptureBlock();}};
struct CRenderManager:CRenderStateManager{void*Textures[4];unsigned Stages,Changes,AttachCalls,IndexCalls;const CIndexBuffer*ExpectedIndex;
 void AttachTextureToStage(long n,const CTexture*t){assert(n>=0&&n<4);++AttachCalls;Event(20+(int)n);void*p=t?t->PD3DTexture:0;if(Textures[n]!=p){Textures[n]=p;++Changes;}if(Stages<(unsigned)n+1)Stages=(unsigned)n+1;}
 void SelectIndexBuffer(CIndexBuffer*p,unsigned long base){assert(p==ExpectedIndex&&base==0);++IndexCalls;Event(170);}
};
struct CShaderRenderManager{C4DVector User[4],Pixel;unsigned UserCalls,PixelCalls,BoneCalls,CountCalls,PrepareCalls,VertexCalls,PixelShaderCalls;int ActivePixel;const CVertexShader*ExpectedVertex;const CMatrix3x4*ExpectedMatrices[4];
 void SetUserVertexShaderConstant(unsigned long i,float x,float y,float z,float w){assert(i==0||i==1||i==3);User[i]=C4DVector(x,y,z,w);++UserCalls;Event(100+(int)i);}
 void SetPixelShaderConstant(unsigned long i,const C4DVector&v){assert(i==5);Pixel=v;++PixelCalls;Event(135);}
 void SetBoneMatrixCount(long n){assert(n==4);++CountCalls;Event(110);}
 void SetBoneTransform(unsigned long i,const CMatrix3x4&m){assert(i<4&&&m==ExpectedMatrices[i]&&m.ID==10+(int)i);++BoneCalls;Event(120+(int)i);}
 void PrepareForRender(unsigned long f){assert(f==0x604);++PrepareCalls;Event(150);}
 void ApplyVertexShader(const CVertexShader&s,bool f){assert(&s==ExpectedVertex&&!f);++VertexCalls;Event(160);}
 void ApplyPixelShader(const CPixelShader&s){assert(s.ID==0||s.ID==1);ActivePixel=s.ID;++PixelShaderCalls;Event(180+s.ID);}
};
struct FrameData{CTexture*Texture;long Refs;};struct CGraphicFrame{FrameData*Data;CGraphicFrame():Data(0){}~CGraphicFrame(){if(Data){--Data->Refs;++Releases;Event(60);}}CTexture*GetTexture()const{assert(Data);return Data->Texture;}};
struct CGraphicInfoFrame{FrameData*Data;bool Loaded;bool GetFrame(CGraphicFrame&out,unsigned long mip,unsigned long flags)const{assert(mip==0&&flags==0);++FrameCalls;Event(10);if(!Loaded)return false;assert(!out.Data);out.Data=Data;++Data->Refs;return true;}};
struct CGraphicInfo{CGraphicInfoFrame*Frames;const CGraphicInfoFrame*PeekInfoFrame(unsigned long f)const{assert(f<2);return Frames+f;}};
struct CGraphicDataBank{CGraphicInfo Info;unsigned long Count;const CGraphicInfo*PeekGraphic(unsigned long i)const{++GraphicQueries;return i<Count?&Info:0;}const CGraphicInfoFrame*PeekGraphicInfoFrame(unsigned long,unsigned long)const;};
struct CEngineResourceManager{CGraphicDataBank*Bank;const CGraphicDataBank*PeekGraphicBank(EGraphicBankNo n)const{assert(n==FixtureTerrain);return Bank;}const CGraphicDataBank*PeekTerrainBank()const;const CGraphicInfoFrame*GetTerrainTexture(long);};
struct CEnvironmentDef{long WaterEdgeAlphaMap,WaterBumpMapPC;};
template<class T>struct CArray{typedef T*iterator;T Data[4];int Count;iterator begin(){return Data;}iterator end(){return Data+Count;}};
struct CWaterPatchMesh{int ID;void Render(long)const;};
struct CEngineWaterSettings{bool EnableSwell,EnableFresnel;float SwellFactor,ShoreSwellFactor,RefOffset,SoftEdgeSpanFactor,FresnelFactor,BumpMatFactor;unsigned long WaterBumpMapPCFrameIndex;CMatrix3x4 OtherShaderVars,ShoreLookupMaskMatrix,ShoreLookupMaskMatrixForBounds,ShoreLookupMaskMatrix2;};
struct CEngineWaterRenderer{CTexture*WaterBumpMap,*EnvironmentMap;CEngineWaterSettings Settings;float WaterBumpFactor;CVertexShader ForegroundVertexShader;CPixelShader RefractionForegroundPixelShader,ForegroundPixelShader;CIndexBuffer ForegroundIndexBuffers;CArray<CWaterPatchMesh*>ForegroundRenderList;long WaterForegroundPolyCount;bool SetForegroundTexturesAndUVModes();void SetupFresnelPixelShaderConstants();void SetupForegroundShaderConstantsAndFog();void RenderForeground(float);};
namespace NEngineInternal{CRenderManager*GPRenderManager;CShaderRenderManager*GPShaderRenderManager;CEngineResourceManager*GPResourceManager;const CEnvironmentDef*GPEnvironmentDef;}
struct RenderRecord{int ID,Pixel;float X,Y,Z,W;long Count;};static RenderRecord Records[8];static int RenderCount;
void CWaterPatchMesh::Render(long count)const{assert(RenderCount<8);RenderRecord&r=Records[RenderCount++];r.ID=ID;r.Pixel=NEngineInternal::GPShaderRenderManager->ActivePixel;const C4DVector&v=NEngineInternal::GPShaderRenderManager->User[3];r.X=v.X;r.Y=v.Y;r.Z=v.Z;r.W=v.W;r.Count=count;Event(200+ID);}
// BEGIN_AUTHORED_BODIES
// Inline-budget: native expands this genuine lookup; plain/depth255 retain a call,
// while budgets10000 expand the original native-called vector resize as well.
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long index,unsigned long frame) const { return PeekGraphic(index) ? PeekGraphic(index)->PeekInfoFrame(frame) : 0; }
inline const CGraphicDataBank* CEngineResourceManager::PeekTerrainBank() const { return PeekGraphicBank((EGraphicBankNo)0); }
// Inline-budget: same original getter-chain expansion requirement and budget/depth proof above.
FABLE_FORCEINLINE const CGraphicInfoFrame* CEngineResourceManager::GetTerrainTexture(long index) { return PeekTerrainBank()->PeekGraphicInfoFrame(index,0); }

FABLE_FORCEINLINE bool CEngineWaterRenderer::SetForegroundTexturesAndUVModes()
{
    CGraphicFrame graphic_frame;
    if (!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->WaterEdgeAlphaMap)->GetFrame(graphic_frame,0,0))
        return false;
    bool animate_bump_map=true;
    if (animate_bump_map)
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,WaterBumpMap);
    else {
        CGraphicFrame lake_bump_graphic;
        if (!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->WaterBumpMapPC)->GetFrame(lake_bump_graphic,Settings.WaterBumpMapPCFrameIndex,0))
            return false;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,lake_bump_graphic.GetTexture());
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(1,EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2,EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(3,graphic_frame.GetTexture());
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1,TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2,TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3,TEXTURE_UV_WRAP);
    return true;
}
// Inline-budget: plain and depth255 keep the original Fresnel helper called; rowT200
// expands it but also incorrectly expands native called vector::resize.
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupFresnelPixelShaderConstants()
{
    // Native retail uses float-width quarter/zero/one operands; later debug donor uses double literals.
    float fresnel_factor=Settings.EnableFresnel ?
        ((Settings.FresnelFactor/4.0f)<0.0f ? 0.0f :
        ((Settings.FresnelFactor/4.0f)>1.0f ? 1.0f : (Settings.FresnelFactor/4.0f))) : 0.0f;
    C4DVector fresnel(fresnel_factor,fresnel_factor,fresnel_factor,fresnel_factor);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,fresnel);
}
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SwellFactor,Settings.ShoreSwellFactor,Settings.RefOffset,0.0f);
    else
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,0.0f,0.0f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1,Settings.SoftEdgeSpanFactor,0.0f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetBoneMatrixCount(4);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0,Settings.OtherShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1,Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2,Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3,Settings.ShoreLookupMaskMatrix2);
    SetupFresnelPixelShaderConstants();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}
void CEngineWaterRenderer::RenderForeground(const float interpolate)
{
    CRenderStateScopedCaptureBlock block;
    if (SetForegroundTexturesAndUVModes()) {
        SetupForegroundShaderConstantsAndFog();
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ForegroundVertexShader,false);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&ForegroundIndexBuffers,0);
        for (CArray<CWaterPatchMesh*>::iterator patch_it=ForegroundRenderList.begin();patch_it!=ForegroundRenderList.end();++patch_it) {
            CWaterPatchMesh& water_patch=**patch_it;
            float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RefractionForegroundPixelShader);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,1.0f,0.0f);
            water_patch.Render(WaterForegroundPolyCount);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,0.0f,1.0f);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ForegroundPixelShader);
            water_patch.Render(WaterForegroundPolyCount);
        }
    }
}

// END_AUTHORED_BODIES
int main(){
 const unsigned requested[8]={1,1,2,3,2,3,3,1};
 for(int c=0;c<16384;++c){Scenario=c;CRenderManager manager;CShaderRenderManager shader;CEngineWaterRenderer renderer;CEngineResourceManager resource;CEnvironmentDef environment;CGraphicDataBank bank;CGraphicInfoFrame frames[2];FrameData data;CTexture water,env,edge;CWaterPatchMesh patches[4];
  water.PD3DTexture=(c%3)?(void*)&water:0;env.PD3DTexture=(c%5)?(void*)&env:0;edge.PD3DTexture=(c%7)?(void*)&edge:0;
  renderer.WaterBumpMap=(c%11)?&water:0;renderer.EnvironmentMap=(c%13)?&env:0;data.Texture=&edge;data.Refs=1;frames[0].Data=&data;frames[0].Loaded=(c&4096)!=0;frames[1]=frames[0];bank.Info.Frames=frames;bank.Count=1;resource.Bank=&bank;environment.WaterEdgeAlphaMap=environment.WaterBumpMapPC=0;
  NEngineInternal::GPRenderManager=&manager;NEngineInternal::GPShaderRenderManager=&shader;NEngineInternal::GPResourceManager=&resource;NEngineInternal::GPEnvironmentDef=&environment;CRenderStateCaptureBlock::StateManager=&manager;
  manager.StackSize=1;manager.Stack[0].P=0;manager.Mask=1;manager.UpdateCount=manager.Changes=manager.AttachCalls=manager.IndexCalls=0;manager.Stages=(unsigned)(c%5);const unsigned old_stages=manager.Stages;manager.ExpectedIndex=&renderer.ForegroundIndexBuffers;
  unsigned old_values[8],old_masks[8],expected_updates=0;bool old_dirty[8];
  for(int i=0;i<8;++i){old_values[i]=((c>>(i+2))&1)?requested[i]:99u;old_masks[i]=(c&(1<<i))?1u:0u;old_dirty[i]=(c&(1<<i))!=0;manager.States[i].Value=old_values[i];manager.States[i].Mask=old_masks[i];manager.States[i].Dirty=old_dirty[i];if(old_values[i]!=requested[i]&&!old_dirty[i])++expected_updates;}
  void*desired[4]={renderer.WaterBumpMap?water.PD3DTexture:0,renderer.EnvironmentMap?env.PD3DTexture:0,renderer.EnvironmentMap?env.PD3DTexture:0,edge.PD3DTexture};void*before[4];unsigned expected_changes=0;
  for(int i=0;i<4;++i){before[i]=((c>>(8+i))&1)?desired[i]:(void*)&patches[i];manager.Textures[i]=before[i];if(before[i]!=desired[i])++expected_changes;}
  shader.UserCalls=shader.PixelCalls=shader.BoneCalls=shader.CountCalls=shader.PrepareCalls=shader.VertexCalls=shader.PixelShaderCalls=0;shader.ExpectedVertex=&renderer.ForegroundVertexShader;
  renderer.Settings.EnableSwell=(c&8192)!=0;renderer.Settings.EnableFresnel=(c%3)!=0;renderer.Settings.SwellFactor=1.25f;renderer.Settings.ShoreSwellFactor=-0.5f;renderer.Settings.RefOffset=0.75f;renderer.Settings.SoftEdgeSpanFactor=(float)(c%17)*0.125f;renderer.Settings.BumpMatFactor=(float)(c%9-4)*0.25f;renderer.WaterBumpFactor=(float)(1+c%7)*0.5f;renderer.Settings.WaterBumpMapPCFrameIndex=0;
  const float factors[8]={-4.0f,0.0f,1.0f,3.5f,4.0f,8.0f,1048576.0f,0.0f};renderer.Settings.FresnelFactor=factors[c%8];if(c%8==7){unsigned bits=0x7fc00000;memcpy(&renderer.Settings.FresnelFactor,&bits,4);}
  renderer.Settings.OtherShaderVars.ID=10;renderer.Settings.ShoreLookupMaskMatrix.ID=11;renderer.Settings.ShoreLookupMaskMatrixForBounds.ID=12;renderer.Settings.ShoreLookupMaskMatrix2.ID=13;shader.ExpectedMatrices[0]=&renderer.Settings.OtherShaderVars;shader.ExpectedMatrices[1]=&renderer.Settings.ShoreLookupMaskMatrix;shader.ExpectedMatrices[2]=&renderer.Settings.ShoreLookupMaskMatrixForBounds;shader.ExpectedMatrices[3]=&renderer.Settings.ShoreLookupMaskMatrix2;
  renderer.RefractionForegroundPixelShader.ID=0;renderer.ForegroundPixelShader.ID=1;renderer.ForegroundRenderList.Count=c%5;renderer.WaterForegroundPolyCount=1+c%41;for(int i=0;i<4;++i){patches[i].ID=i;renderer.ForegroundRenderList.Data[i]=&patches[i];}
  EventCount=GraphicQueries=FrameCalls=Releases=RenderCount=0;renderer.RenderForeground((float)c);
  const bool loaded=frames[0].Loaded;const int n=renderer.ForegroundRenderList.Count;assert(FrameCalls==1&&GraphicQueries==2&&data.Refs==1&&Releases==(loaded?1:0));assert(manager.StackSize==1&&manager.Mask==1);
  for(int i=0;i<8;++i){assert(manager.States[i].Value==old_values[i]&&manager.States[i].Mask==old_masks[i]);assert(manager.States[i].Dirty==(old_dirty[i]||(loaded&&old_values[i]!=requested[i])));}
  assert(manager.UpdateCount==(loaded?expected_updates:0)&&manager.AttachCalls==(loaded?4u:0u)&&manager.Changes==(loaded?expected_changes:0));assert(manager.IndexCalls==(loaded?1u:0u)&&manager.Stages==(loaded?4u:old_stages));for(int i=0;i<4;++i)assert(manager.Textures[i]==(loaded?desired[i]:before[i]));
  assert(shader.UserCalls==(loaded?(unsigned)(2+2*n):0)&&shader.PixelCalls==(loaded?1u:0u)&&shader.BoneCalls==(loaded?4u:0u)&&shader.CountCalls==(loaded?1u:0u)&&shader.PrepareCalls==(loaded?1u:0u)&&shader.VertexCalls==(loaded?1u:0u)&&shader.PixelShaderCalls==(loaded?(unsigned)(2*n):0));assert(RenderCount==(loaded?2*n:0));
  if(loaded){if(renderer.Settings.EnableSwell)EqualVector(shader.User[0],1.25f,-0.5f,0.75f,0);else EqualVector(shader.User[0],0,0,0,0);EqualVector(shader.User[1],renderer.Settings.SoftEdgeSpanFactor,0,0,0);float f=0;if(renderer.Settings.EnableFresnel){f=renderer.Settings.FresnelFactor*0.25f;if(f<0)f=0;if(f>1)f=1;}EqualVector(shader.Pixel,f,f,f,f);float bump=renderer.Settings.BumpMatFactor*renderer.WaterBumpFactor;for(int i=0;i<2*n;++i){const RenderRecord&r=Records[i];assert(r.ID==i/2&&r.Pixel==i%2&&r.Count==renderer.WaterForegroundPolyCount&&r.X==bump&&r.Y==0&&r.Z==(i%2?0:1)&&r.W==(i%2?1:0));}}
  int expect[128],e=0;expect[e++]=1;expect[e++]=10;if(loaded){for(int i=0;i<4;++i)expect[e++]=20+i;for(int i=0;i<8;++i)expect[e++]=40+i;expect[e++]=60;expect[e++]=100;expect[e++]=101;expect[e++]=110;for(int i=0;i<4;++i)expect[e++]=120+i;expect[e++]=135;expect[e++]=150;expect[e++]=160;expect[e++]=170;for(int i=0;i<n;++i){expect[e++]=180;expect[e++]=103;expect[e++]=200+i;expect[e++]=103;expect[e++]=181;expect[e++]=200+i;}}expect[e++]=250;assert(e==EventCount);for(int i=0;i<e;++i)assert(expect[i]==Events[i]);
 }
 // Independent public getter boundary cases, including missing graphics.
 CGraphicDataBank bank;CGraphicInfoFrame frames[2];bank.Info.Frames=frames;bank.Count=1;CEngineResourceManager resource;resource.Bank=&bank;for(unsigned i=0;i<32;++i){for(unsigned f=0;f<2;++f){GraphicQueries=0;const CGraphicInfoFrame*p=bank.PeekGraphicInfoFrame(i,f);assert(p==(i==0?frames+f:0)&&GraphicQueries==(i==0?2:1));}GraphicQueries=0;const CGraphicInfoFrame*p=resource.GetTerrainTexture((long)i);assert(p==(i==0?frames:0)&&GraphicQueries==(i==0?2:1));}
 puts("00b761d0_TEST PASS");return 0;
}