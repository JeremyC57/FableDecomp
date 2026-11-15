#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
#include "fable_compiler.h"
static void checkAt(bool b,int line){if(!b){printf("00bc6db0_TEST FAIL line%d\n",line);exit(1);}}
#define check(b) checkAt((b),__LINE__)
static int writes,draws,prepares,constant1,constant0,uv_calls,vs_applies,ps_applies,capture_depth,layout_changes;
static bool fixed,has_mask,keep_filter,oldvs,oldps;
static float constant_values[8];
struct C2DVector{float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
struct C3DVector{float X,Y,Z;};
struct C4DVector{float X,Y,Z,W;};
struct C2DExtentsI{long X,Y;};
struct C2DBoxF{float TLX,TLY,BRX,BRY;};
struct CRGBColour{unsigned int Value;};
struct CWideString{unsigned short* Text;};
struct CTexture{unsigned int Tag;};
struct CVertexShader{unsigned int Tag;};
struct CPixelShader{unsigned int Tag;};
enum ERenderPassType{FixturePass0=0,FixturePass1=1,FixturePass2=2};
enum TEXTURE_UV_MODE{TEXTURE_UV_WRAP=0,TEXTURE_UV_CLAMP=2};
enum TEXTURE_FILTER_MODE{TEXTURE_FILTER_POINT=0,TEXTURE_FILTER_LINEAR=1};
enum TEXTURE_MIPMAP_MODE{TEXTURE_MIPMAP_DISABLE=0};
enum ALPHA_BLEND_MODE{FixtureBlend0=0};
struct RenderStates{long U[2],V[2],Min[2],Mag[2],Mip[2],AlphaTest,AlphaBlend,Source,Dest;};
class CRenderStateManager{public:RenderStates States,Before;
 void StartCaptureBlock(void*){check(capture_depth==0);Before=States;++capture_depth;}
 void RestoreCaptureBlock(){check(capture_depth==1);States=Before;--capture_depth;}
 void SetTextureUVModeU(long s,TEXTURE_UV_MODE v){check(s==0||s==1);States.U[s]=v;++writes;}
 void SetTextureUVModeV(long s,TEXTURE_UV_MODE v){check(s==0||s==1);States.V[s]=v;++writes;}
 void SetTextureFilterModeMin(long s,TEXTURE_FILTER_MODE v){check(s==0||s==1);States.Min[s]=v;++writes;}
 void SetTextureFilterModeMag(long s,TEXTURE_FILTER_MODE v){check(s==0||s==1);States.Mag[s]=v;++writes;}
 void SetTextureMipMapMode(long s,TEXTURE_MIPMAP_MODE v){check(s==0||s==1);States.Mip[s]=v;++writes;}
 void SetAlphaTestEnable(bool v){States.AlphaTest=v;++writes;}
 void SetAlphaBlendEnable(bool v){States.AlphaBlend=v;++writes;}
 void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE v){States.Source=v;++writes;}
 void SetDestAlphaBlendMode(ALPHA_BLEND_MODE v){States.Dest=v;++writes;}
};
class CRenderManager:public CRenderStateManager{public:C2DBoxF Window;const C2DBoxF& GetCurrentAWindow()const{return Window;}};
class CShaderRenderManager{public:bool VS,PS;long Layout;static CShaderRenderManager* Instance;
 static CShaderRenderManager& __fastcall Get(){return *Instance;}
 void SetVertexShadersEnable(bool v,bool*old){if(old)*old=VS;VS=v;}
 void SetPixelShadersEnable(bool v,bool*old){if(old)*old=PS;PS=v;}
 long PeekVertexShaderConstantLayout()const{return Layout;}
 void SetVertexShaderConstantLayout(long x){Layout=x;++layout_changes;}
 void SetUserVertexShaderConstant(unsigned long n,float x,float y,float z,float w){check(n==1);++constant1;constant_values[0]=x;constant_values[1]=y;constant_values[2]=z;constant_values[3]=w;}
 void SetUserVertexShaderConstant(unsigned long n,const C4DVector& x){check(n==0);++constant0;constant_values[4]=x.X;constant_values[5]=x.Y;constant_values[6]=x.Z;constant_values[7]=x.W;}
 void ApplyVertexShader(const CVertexShader&s,bool b){check(!b);check(s.Tag==(has_mask?2U:1U));++vs_applies;}
 void ApplyPixelShader(const CPixelShader&s){check(s.Tag==(has_mask?4U:3U));++ps_applies;}
 void PrepareForRender(unsigned long n){check(n==0);check(draws==0);++prepares;}
};
CShaderRenderManager* CShaderRenderManager::Instance=0;
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
#define FABLE_ENGINE_CSHADERRENDERMANAGER_H
#define FABLE_ENGINE_CRENDERSTATEMANAGER_H
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CRenderStateCaptureBlock.h"
CRenderStateManager* CRenderStateCaptureBlock::StateManager=0;
void __fastcall CRenderStateCaptureBlock::SetStateManager(CRenderStateManager*p){StateManager=p;}
class CDisplayManager{public:C2DExtentsI Dimensions;const C2DExtentsI GetRenderTargetDimensions(){check(!fixed);return Dimensions;}};
class CStreamingFontBank{public:static bool __fastcall GetUseFixedFunction(){return fixed;}static void __fastcall SetAlphaMaskUVs(const C2DVector&a,const C2DVector&b);};
class C2DAlphaMaskInfo{public:C4DVector AlphaMaskPositionFactors;bool MaskObjectsOutsideAlphaMask,FilterMask;};
class CEnginePrimitiveManager{public:virtual ~CEnginePrimitiveManager(){}};
class CEnginePrimitive2DViewportManager:public CEnginePrimitiveManager{public:C2DAlphaMaskInfo AlphaMaskInfo;const C2DAlphaMaskInfo*GetCurrentAlphaMask(){return &AlphaMaskInfo;}};
class CEnginePrimitiveTypeInfo{public:CEnginePrimitiveManager* Manager;CEnginePrimitiveManager*GetPrimitiveManager(long n)const{check(n==33);return Manager;}};
class CEnginePrimitiveRenderUtil{public:const C2DAlphaMaskInfo*GetCurrent2DAlphaMaskInfo();void DisableTextureFiltering(unsigned long);void Set2DAlphaMaskTextureMode(unsigned long,bool,bool);};
class CEngineSubPrimitiveBase{public:virtual ~CEngineSubPrimitiveBase(){}};
class CFontBank{public:virtual ~CFontBank(){}virtual void PrepareToRender(bool)const=0;virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const=0;};
class CEngineSubPrimitiveText:public CEngineSubPrimitiveBase{public:C3DVector Pos;float Scale;CRGBColour Colour;CWideString Text;const CFontBank*Font;const CTexture*Mask;};
class CEnginePrimitiveManagerText:public CEnginePrimitiveRenderUtil{public:CVertexShader VShader,VShaderMask;CPixelShader PShader,PShaderMask;void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);};
namespace NEngineInternal{CRenderManager*GPRenderManager;CShaderRenderManager*GPShaderRenderManager;CDisplayManager*GPDisplayManager;CEnginePrimitiveTypeInfo*GPPrimitiveTypeInfo;}
#include "engine/GFDebugPDynamicCast.h"

// Verbatim production target and original helper bodies; generated below.
inline const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo() {
    CEnginePrimitive2DViewportManager* manager=GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(33),(CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::DisableTextureFiltering(unsigned long stage) {
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage,TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage,TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureMipMapMode(stage,TEXTURE_MIPMAP_DISABLE);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::Set2DAlphaMaskTextureMode(unsigned long stage,bool mask_outside,bool filter) {
    if(!filter)DisableTextureFiltering(stage);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(stage,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(stage,TEXTURE_UV_CLAMP);
}

void CEnginePrimitiveManagerText::RenderSubPrimitive(CEngineSubPrimitiveBase& subprim_base, ERenderPassType render_pass)
{
    CEngineSubPrimitiveText& subprim = static_cast<CEngineSubPrimitiveText&>(subprim_base);
    bool vs_enable = !CStreamingFontBank::GetUseFixedFunction();
    CScopedVertexShaderConstantLayout layout(1);
    CRenderStateScopedCaptureBlock state;
    CScopedShadersEnable shaders_enable(vs_enable, true);
    if (vs_enable) {
        C2DExtentsI dim = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
        const float one_on_half_screen_width = 1.0f / ((float)dim.X * 0.5f);
        const float one_on_half_screen_height = 1.0f / ((float)dim.Y * 0.5f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1,one_on_half_screen_width,one_on_half_screen_height,-1.0f,1.0f);
    }
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    TEXTURE_FILTER_MODE filter_mode=TEXTURE_FILTER_LINEAR;
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, filter_mode);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, filter_mode);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode((ALPHA_BLEND_MODE)4);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode((ALPHA_BLEND_MODE)5);
    if (subprim.Mask) {
        const C2DAlphaMaskInfo& alpha_mask_info = *GetCurrent2DAlphaMaskInfo();
        Set2DAlphaMaskTextureMode(1,alpha_mask_info.MaskObjectsOutsideAlphaMask,alpha_mask_info.FilterMask);
        if (vs_enable) {
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, alpha_mask_info.AlphaMaskPositionFactors);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShaderMask, false);
        } else {
            C4DVector alpha_mask_uvs = alpha_mask_info.AlphaMaskPositionFactors;
            CStreamingFontBank::SetAlphaMaskUVs(C2DVector(alpha_mask_uvs.X, alpha_mask_uvs.Y), C2DVector(alpha_mask_uvs.Z, alpha_mask_uvs.W));
        }
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShaderMask);
    } else {
        if (vs_enable)
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShader);
    }
    C3DVector pos;
    const C2DBoxF& window = NEngineInternal::GPRenderManager->GetCurrentAWindow();
    pos.X = window.TLX + subprim.Pos.X;
    pos.Y = window.TLY + subprim.Pos.Y;
    pos.Z = subprim.Pos.Z;
    bool fixed_function=false;
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    subprim.Font->PrepareToRender(fixed_function);
    subprim.Font->Render(subprim.Text, pos, subprim.Scale, subprim.Colour, 0, subprim.Mask);
}


static CEngineSubPrimitiveText* expected_sub;
static CEnginePrimitive2DViewportManager* expected_viewport;
void __fastcall CStreamingFontBank::SetAlphaMaskUVs(const C2DVector&a,const C2DVector&b){
 check(fixed&&has_mask);++uv_calls;const C4DVector&v=expected_viewport->AlphaMaskInfo.AlphaMaskPositionFactors;check(a.X==v.X&&a.Y==v.Y&&b.X==v.Z&&b.Y==v.W);
}
class FontFixture:public CFontBank{public:
 void PrepareToRender(bool b)const{check(!b);check(prepares==1);++draws;}
 void Render(const CWideString&t,const C3DVector&p,float scale,const CRGBColour&c,unsigned long f,const CTexture*m)const{
  check(draws==1&&prepares==1&&f==0&&m==expected_sub->Mask);++draws;
  check(&t==&expected_sub->Text&&&c==&expected_sub->Colour&&scale==expected_sub->Scale);
  CRenderManager&r=*NEngineInternal::GPRenderManager;CShaderRenderManager&s=*NEngineInternal::GPShaderRenderManager;
  check(p.X==expected_sub->Pos.X+r.Window.TLX&&p.Y==r.Window.TLY+expected_sub->Pos.Y&&p.Z==expected_sub->Pos.Z);
  check(s.VS==!fixed&&s.PS&&s.Layout==1&&capture_depth==1);
  check(r.States.U[0]==0&&r.States.V[0]==0&&r.States.Min[0]==1&&r.States.Mag[0]==1);
  check(!r.States.AlphaTest&&r.States.AlphaBlend&&r.States.Source==4&&r.States.Dest==5&&r.States.Mip[0]==r.Before.Mip[0]);
  if(has_mask){check(r.States.U[1]==2&&r.States.V[1]==2);if(!keep_filter)check(r.States.Min[1]==0&&r.States.Mag[1]==0&&r.States.Mip[1]==0);else check(r.States.Min[1]==r.Before.Min[1]&&r.States.Mag[1]==r.Before.Mag[1]&&r.States.Mip[1]==r.Before.Mip[1]);}
  else check(r.States.U[1]==r.Before.U[1]&&r.States.V[1]==r.Before.V[1]&&r.States.Min[1]==r.Before.Min[1]&&r.States.Mag[1]==r.Before.Mag[1]&&r.States.Mip[1]==r.Before.Mip[1]);
 }
};
int main(){
 CRenderManager render;CShaderRenderManager shader;CDisplayManager display;CEnginePrimitiveTypeInfo info;CEnginePrimitive2DViewportManager viewport;CEnginePrimitiveManagerText target;FontFixture font;CEngineSubPrimitiveText sub;CTexture mask;unsigned short text[]={65,66,0};
 NEngineInternal::GPRenderManager=&render;NEngineInternal::GPShaderRenderManager=&shader;NEngineInternal::GPDisplayManager=&display;NEngineInternal::GPPrimitiveTypeInfo=&info;info.Manager=&viewport;CShaderRenderManager::Instance=&shader;CRenderStateCaptureBlock::SetStateManager(&render);expected_sub=&sub;expected_viewport=&viewport;
 target.VShader.Tag=1;target.VShaderMask.Tag=2;target.PShader.Tag=3;target.PShaderMask.Tag=4;sub.Font=&font;sub.Text.Text=text;sub.Colour.Value=0x42a8c451U;
 unsigned cases=0;
 for(unsigned seed=0;seed<32;++seed)for(unsigned bits=0;bits<64;++bits){
  fixed=(bits&1)!=0;has_mask=(bits&2)!=0;keep_filter=(bits&4)!=0;oldvs=(bits&8)!=0;oldps=(bits&16)!=0;
  viewport.AlphaMaskInfo.FilterMask=keep_filter;viewport.AlphaMaskInfo.MaskObjectsOutsideAlphaMask=(bits&32)!=0;
  viewport.AlphaMaskInfo.AlphaMaskPositionFactors.X=(float)(seed+1)*0.125f;viewport.AlphaMaskInfo.AlphaMaskPositionFactors.Y=(float)(seed+3)*-0.25f;viewport.AlphaMaskInfo.AlphaMaskPositionFactors.Z=(float)(seed+2)*0.0625f;viewport.AlphaMaskInfo.AlphaMaskPositionFactors.W=(float)(seed+5)*0.5f;
  sub.Pos.X=(float)seed-10.0f;sub.Pos.Y=20.0f-(float)seed;sub.Pos.Z=(float)seed*-0.25f;sub.Scale=(float)seed*0.125f-1.0f;sub.Mask=has_mask?&mask:0;
  render.Window.TLX=(float)seed*0.75f;render.Window.TLY=(float)seed*-1.25f;display.Dimensions.X=37+seed*17;display.Dimensions.Y=29+seed*13;
  for(unsigned i=0;i<2;++i){render.States.U[i]=11+seed+i;render.States.V[i]=21+seed+i;render.States.Min[i]=31+seed+i;render.States.Mag[i]=41+seed+i;render.States.Mip[i]=51+seed+i;}render.States.AlphaTest=seed&1;render.States.AlphaBlend=(seed+1)&1;render.States.Source=61+seed;render.States.Dest=71+seed;
  RenderStates before=render.States;shader.VS=oldvs;shader.PS=oldps;shader.Layout=3+seed;long oldlayout=shader.Layout;
  writes=draws=prepares=constant1=constant0=uv_calls=vs_applies=ps_applies=capture_depth=layout_changes=0;memset(constant_values,0,sizeof(constant_values));
  target.RenderSubPrimitive(sub,(ERenderPassType)(seed%3));++cases;
  check(draws==2&&prepares==1&&capture_depth==0&&layout_changes==2&&shader.Layout==oldlayout&&shader.VS==oldvs&&shader.PS==oldps);
  check(memcmp(&before,&render.States,sizeof(before))==0);
  check(writes==8+(has_mask?(keep_filter?2:5):0));check(constant1==(!fixed?1:0)&&constant0==(!fixed&&has_mask?1:0));check(uv_calls==(fixed&&has_mask?1:0));check(vs_applies==(!fixed?1:0)&&ps_applies==1);
  if(!fixed){float expected_x=1.0f/((float)display.Dimensions.X*0.5f);float expected_y=1.0f/((float)display.Dimensions.Y*0.5f);check(constant_values[0]==expected_x&&constant_values[1]==expected_y&&constant_values[2]==-1.0f&&constant_values[3]==1.0f);if(has_mask){const C4DVector&v=viewport.AlphaMaskInfo.AlphaMaskPositionFactors;check(constant_values[4]==v.X&&constant_values[5]==v.Y&&constant_values[6]==v.Z&&constant_values[7]==v.W);}}
 }
 check(cases==2048);puts("00bc6db0_TEST PASS");return 0;
}