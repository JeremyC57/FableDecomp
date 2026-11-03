// Exact selected Initialise body; tests verify changed and preserved state plus live resource enumeration.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
static int errors,scenarios,strings,resource_calls,name_calls,grow_resources,old_names;
static void check(bool b){if(!b){if(errors<8)fprintf(stderr,"scenario %d resource %d names %d\n",scenarios,resource_calls,name_calls);++errors;}}
class CCharString{public:const char* text;CCharString(const char* p,long n=-1):text(p){check(n==-1);++strings;}CCharString(const CCharString& s):text(s.text){++strings;}~CCharString(){--strings;}};
class C4DVector{public:float X,Y,Z,W;C4DVector(float x=0,float y=0,float z=0,float w=1):X(x),Y(y),Z(z),W(w){}};
class C3DVector{public:float X,Y,Z;};
class CRGBFloatColour{public:float R,G,B,A;void Initialise(float r,float g,float b,float a){R=r;G=g;B=b;A=a;}};
class CRGBColour{public:unsigned char B,G,R,A;CRGBColour(unsigned char r=0,unsigned char g=0,unsigned char b=0,unsigned char a=0){R=r;G=g;B=b;A=a;}};
class CMatrix4x4{public:float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44;
    void SetIdentity() {
        E11=1.0f; E22=1.0f; E33=1.0f; E44=1.0f;
        E12=0.0f; E13=0.0f; E14=0.0f; E21=0.0f; E23=0.0f; E24=0.0f; E31=0.0f; E32=0.0f; E34=0.0f; E41=0.0f; E42=0.0f; E43=0.0f;
    }
};
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer{public:T* Data;CCountedPointer(T* p=0):Data(p){}T* operator->()const{return Data;}};
class CVertexShaderConstantLayout{};class CShaderRenderManager;class CShaderResource{public:CShaderRenderManager* Manager;void Initialise(CShaderRenderManager&);};
class CRenderManager{};struct IDirect3DDevice9{};struct IDirect3DVertexShader9{};struct IDirect3DVertexDeclaration9{};
class CIndexedArray{public:std::vector<const char*> Names;void push_back(const CCharString&);};
class CShaderRenderManager{
public:
 const CVertexShaderConstantLayout* CurrentVSConstantLayout;long CurrentVSConstantLayoutIndex;
 CArray<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts;CVertexShaderConstantLayout NullConstantLayout;
 void* Lights;C4DVector* LightBuffer;unsigned long LightCount,AllocatedLights,AllocatedLightSize;unsigned char LightPool[28];
 CArray<C4DVector> LightGlobals,LightAttenuations;CRGBFloatColour AmbientColour,FogColour;CRGBColour TextureFactor;
 CRenderManager* RenderManager;IDirect3DDevice9* D3DDevice;IDirect3DVertexShader9* CurrentVertexShader;IDirect3DVertexDeclaration9* CurrentVertexShaderVertexDeclairation;
 unsigned char CurrentPixelShader[8];bool WorldMatrixIsIdentity;CMatrix4x4 WorldMatrix,ViewMatrix,ProjectionMatrix,ViewProjectionMatrix,CombinedProjectionMatrix,TextureTransforms[1];
 C4DVector FogTransform;C3DVector CameraPos;long TextureTransformCount;
 bool VertexShadersEnabled,PixelShadersEnabled,SWLightingEnable,SoftwareVertexShaderEnable,SoftwareFixedFunctionEnable,FixedFunctionVertexMode;
 float MultisampleViewPortScale;void* CycleTextures;unsigned long NextCycleTexture,CycleTextureStage;
 CArray<CCountedPointer<CShaderResource> > ResourceManagers;CIndexedArray PSConstNames;CArray<C4DVector> PSConstTable;unsigned long UpdateFlags;unsigned char BoneMatrices[12];
 void Initialise(CRenderManager*,IDirect3DDevice9*);
};
static CShaderRenderManager* owner;static CRenderManager renderer;static IDirect3DDevice9 device;static IDirect3DVertexShader9 vertex;static IDirect3DVertexDeclaration9 declaration;static CVertexShaderConstantLayout layout;
static CShaderResource resources[8];static C4DVector light(2,4,8,16);static int sentinel;
static bool same(const C4DVector& v,float x,float y,float z,float w){return v.X==x&&v.Y==y&&v.Z==z&&v.W==w;}
static bool matrix_is(const CMatrix4x4& m,float diagonal,float off){return m.E11==diagonal&&m.E22==diagonal&&m.E33==diagonal&&m.E44==diagonal&&m.E12==off&&m.E13==off&&m.E14==off&&m.E21==off&&m.E23==off&&m.E24==off&&m.E31==off&&m.E32==off&&m.E34==off&&m.E41==off&&m.E42==off&&m.E43==off;}
static void fill_matrix(CMatrix4x4& m){m.E11=m.E12=m.E13=m.E14=m.E21=m.E22=m.E23=m.E24=m.E31=m.E32=m.E33=m.E34=m.E41=m.E42=m.E43=m.E44=8.0f;}
static void flags_clear(){check(!owner->SWLightingEnable&&!owner->VertexShadersEnabled&&!owner->PixelShadersEnabled&&!owner->FixedFunctionVertexMode&&!owner->SoftwareVertexShaderEnable&&!owner->SoftwareFixedFunctionEnable);}
void CShaderResource::Initialise(CShaderRenderManager& manager){
 check(&manager==owner&&resource_calls<8);check(this==&resources[resource_calls]);check(name_calls==0&&matrix_is(manager.WorldMatrix,8,8));
 check(manager.D3DDevice==&device&&manager.RenderManager==&renderer&&manager.CurrentVertexShader==0&&manager.CurrentVertexShaderVertexDeclairation==0);
 check(manager.VertexShaderConstantLayouts.size()==1&&manager.VertexShaderConstantLayouts[0]==&manager.NullConstantLayout&&manager.CurrentVSConstantLayout==&manager.NullConstantLayout&&manager.CurrentVSConstantLayoutIndex==-1);flags_clear();
 Manager=&manager;++resource_calls;
 if(grow_resources&&resource_calls==1)manager.ResourceManagers.push_back(CCountedPointer<CShaderResource>(&resources[manager.ResourceManagers.size()]));
}
static const char* names[]={"PSCONST_INPUT_FACTOR_0","PSCONST_INPUT_FACTOR_1","PSCONST_OUTPUT_FACTOR","PSCONST_MAX_FOG_ALPHA","PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR","PSCONST_USER_0","PSCONST_USER_1","PSCONST_USER_2","PSCONST_USER_3"};
void CIndexedArray::push_back(const CCharString& name){
 check(name_calls<9);if(name_calls<9)check(!strcmp(name.text,names[name_calls]));check(resource_calls==(int)owner->ResourceManagers.size());
 check(matrix_is(owner->WorldMatrix,1,0)&&matrix_is(owner->ViewMatrix,1,0)&&matrix_is(owner->ProjectionMatrix,1,0)&&matrix_is(owner->TextureTransforms[0],1,0));
 check(owner->TextureFactor.R==255&&owner->TextureFactor.G==255&&owner->TextureFactor.B==255&&owner->TextureFactor.A==255);
 check(owner->AmbientColour.R==1&&owner->AmbientColour.G==1&&owner->AmbientColour.B==1&&owner->AmbientColour.A==1&&owner->FogColour.R==0&&owner->FogColour.G==0&&owner->FogColour.B==0&&owner->FogColour.A==0);
 Names.push_back(name.text);++name_calls;
}

void CShaderRenderManager::Initialise(CRenderManager* render_manager,IDirect3DDevice9* d3ddevice)
{
    D3DDevice=0;
    CurrentVertexShader=0;
    CurrentVertexShaderVertexDeclairation=0;
    SWLightingEnable=false;
    VertexShadersEnabled=false;
    PixelShadersEnabled=false;
    FixedFunctionVertexMode=false;
    SoftwareVertexShaderEnable=false;
    SoftwareFixedFunctionEnable=false;
    VertexShaderConstantLayouts.resize(1,(const CVertexShaderConstantLayout*)0);
    VertexShaderConstantLayouts[0]=&NullConstantLayout;
    CurrentVSConstantLayout=&NullConstantLayout;
    CurrentVSConstantLayoutIndex=-1;
    RenderManager=render_manager;
    D3DDevice=d3ddevice;
    for (unsigned long c_res=0;c_res<ResourceManagers.size();++c_res)
        ResourceManagers[c_res]->Initialise(*this);
    WorldMatrix.SetIdentity();
    WorldMatrixIsIdentity=true;
    ViewMatrix.SetIdentity();
    ProjectionMatrix.SetIdentity();
    MultisampleViewPortScale=1.0f;
    TextureTransformCount=0;
    for (unsigned long c_tt=0;c_tt<1;++c_tt)
        TextureTransforms[c_tt].SetIdentity();
    LightCount=0;
    AllocatedLights=0;
    AllocatedLightSize=0;
    AmbientColour.Initialise(1.0f,1.0f,1.0f,1.0f);
    FogColour.Initialise(0.0f,0.0f,0.0f,0.0f);
    TextureFactor=CRGBColour(255,255,255,255);
    PSConstNames.push_back("PSCONST_INPUT_FACTOR_0");
    PSConstNames.push_back("PSCONST_INPUT_FACTOR_1");
    PSConstNames.push_back("PSCONST_OUTPUT_FACTOR");
    PSConstNames.push_back("PSCONST_MAX_FOG_ALPHA");
    PSConstNames.push_back("PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR");
    PSConstNames.push_back("PSCONST_USER_0");
    PSConstNames.push_back("PSCONST_USER_1");
    PSConstNames.push_back("PSCONST_USER_2");
    PSConstNames.push_back("PSCONST_USER_3");
    PSConstTable.resize(9,C4DVector(0.0f,0.0f,0.0f,0.0f));
    PSConstTable[0]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    PSConstTable[1]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    PSConstTable[2]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    UpdateFlags=0;
}

int main(){
 const int table_sizes[]={0,1,3,8,9,10,14},layout_sizes[]={0,1,3,7};
 for(int lc=0;lc<4;++lc)for(int rc=0;rc<3;++rc)for(int tc=0;tc<7;++tc)for(int existing=0;existing<2;++existing)for(int growth=0;growth<2;++growth){
  ++scenarios;CShaderRenderManager row;owner=&row;resource_calls=name_calls=0;grow_resources=growth;old_names=existing;
  row.CurrentVSConstantLayout=&layout;row.CurrentVSConstantLayoutIndex=27;row.VertexShaderConstantLayouts.assign(layout_sizes[lc],&layout);
  row.Lights=&sentinel;row.LightBuffer=&light;row.LightCount=19;row.AllocatedLights=23;row.AllocatedLightSize=29;memset(row.LightPool,0xa5,sizeof(row.LightPool));
  row.LightGlobals.assign(2,C4DVector(2,4,8,16));row.LightAttenuations.assign(3,C4DVector(4,8,16,32));
  row.AmbientColour.Initialise(2,3,4,5);row.FogColour.Initialise(2,3,4,5);row.TextureFactor=CRGBColour(1,2,3,4);
  row.RenderManager=0;row.D3DDevice=&device;row.CurrentVertexShader=&vertex;row.CurrentVertexShaderVertexDeclairation=&declaration;memset(row.CurrentPixelShader,0xb6,sizeof(row.CurrentPixelShader));
  row.WorldMatrixIsIdentity=false;fill_matrix(row.WorldMatrix);fill_matrix(row.ViewMatrix);fill_matrix(row.ProjectionMatrix);fill_matrix(row.ViewProjectionMatrix);fill_matrix(row.CombinedProjectionMatrix);fill_matrix(row.TextureTransforms[0]);
  row.FogTransform=C4DVector(2,4,8,16);row.CameraPos.X=4;row.CameraPos.Y=8;row.CameraPos.Z=16;row.TextureTransformCount=5;
  row.VertexShadersEnabled=row.PixelShadersEnabled=row.SWLightingEnable=row.SoftwareVertexShaderEnable=row.SoftwareFixedFunctionEnable=row.FixedFunctionVertexMode=true;
  row.MultisampleViewPortScale=8;row.CycleTextures=&sentinel;row.NextCycleTexture=31;row.CycleTextureStage=37;
  int resource_count=rc==2?4:rc;for(int i=0;i<8;++i)resources[i].Manager=0;for(int i=0;i<resource_count;++i)row.ResourceManagers.push_back(CCountedPointer<CShaderResource>(&resources[i]));
  if(existing)row.PSConstNames.Names.push_back("existing");row.PSConstTable.assign(table_sizes[tc],C4DVector(2,4,8,16));row.UpdateFlags=0x1234;memset(row.BoneMatrices,0xc7,sizeof(row.BoneMatrices));
  row.Initialise(&renderer,&device);
  int actual_resources=resource_count+((growth&&resource_count)?1:0);check(resource_calls==actual_resources&&name_calls==9&&strings==0);for(int i=0;i<8;++i)check(resources[i].Manager==(i<actual_resources?&row:0));
  check(row.VertexShaderConstantLayouts.size()==1&&row.VertexShaderConstantLayouts[0]==&row.NullConstantLayout&&row.CurrentVSConstantLayout==&row.NullConstantLayout&&row.CurrentVSConstantLayoutIndex==-1);
  check(row.D3DDevice==&device&&row.RenderManager==&renderer&&row.CurrentVertexShader==0&&row.CurrentVertexShaderVertexDeclairation==0);flags_clear();
  check(row.WorldMatrixIsIdentity&&row.TextureTransformCount==0&&row.MultisampleViewPortScale==1&&row.LightCount==0&&row.AllocatedLights==0&&row.AllocatedLightSize==0&&row.UpdateFlags==0);
  check(matrix_is(row.WorldMatrix,1,0)&&matrix_is(row.ViewMatrix,1,0)&&matrix_is(row.ProjectionMatrix,1,0)&&matrix_is(row.TextureTransforms[0],1,0));
  check(matrix_is(row.ViewProjectionMatrix,8,8)&&matrix_is(row.CombinedProjectionMatrix,8,8));
  check(row.PSConstTable.size()==9);for(int i=0;i<9;++i){if(i<3)check(same(row.PSConstTable[i],1,1,1,1));else if(i<table_sizes[tc])check(same(row.PSConstTable[i],2,4,8,16));else check(same(row.PSConstTable[i],0,0,0,0));}
  check(row.PSConstNames.Names.size()==(unsigned)(9+existing));if(existing)check(!strcmp(row.PSConstNames.Names[0],"existing"));for(int i=0;i<9;++i)check(!strcmp(row.PSConstNames.Names[existing+i],names[i]));
  check(row.Lights==&sentinel&&row.LightBuffer==&light&&row.LightGlobals.size()==2&&row.LightAttenuations.size()==3);for(int i=0;i<2;++i)check(same(row.LightGlobals[i],2,4,8,16));for(int i=0;i<3;++i)check(same(row.LightAttenuations[i],4,8,16,32));
  for(unsigned i=0;i<sizeof(row.LightPool);++i)check(row.LightPool[i]==0xa5);for(unsigned i=0;i<sizeof(row.CurrentPixelShader);++i)check(row.CurrentPixelShader[i]==0xb6);for(unsigned i=0;i<sizeof(row.BoneMatrices);++i)check(row.BoneMatrices[i]==0xc7);
  check(same(row.FogTransform,2,4,8,16)&&row.CameraPos.X==4&&row.CameraPos.Y==8&&row.CameraPos.Z==16&&row.CycleTextures==&sentinel&&row.NextCycleTexture==31&&row.CycleTextureStage==37);
 }
 if(errors){fprintf(stderr,"%d errors in %d scenarios\n",errors,scenarios);return 1;}puts("0098acf0_TEST PASS");return 0;
}
