#include <cstdio>
#include <cstdlib>
#include <cstring>
#if !defined(_MSC_VER)
#define FABLE_PORTABLE
#else
#define FABLE_DUAL_PATH_TEST
#endif
#include "engine/GFFloatToLongFloor.h"
static void Check(bool b){if(!b){std::puts("00b484b0_TEST FAIL");std::exit(1);}}
static unsigned trace[32],trace_count;
static void Mark(unsigned n){Check(trace_count<32);trace[trace_count++]=n;}
static unsigned long GFastOneOverSqrtTable[128];
// Portable fixture of the same grouped Newton arithmetic;32-bit word memcpy is explicit on LP64.
inline float GFFastOneOverSqrt(float value){unsigned int bits;std::memcpy(&bits,&value,4);unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|((unsigned int)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r;std::memcpy(&r,&estimate,4);return ((3.0f-((r*r)*value))*r)*0.5f;}
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector& Normalise(){float x=X;float y=Y;float z=Z;float one_on_magnitude=GFFastOneOverSqrt(x*x+y*y+z*z);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
};
class C4DVector{public:float X,Y,Z,W;C4DVector(){}C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}};
class CRGBFloatColour{public:float R,G,B,A;};
class CMatrix3x4{public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;CMatrix3x4(){}
    CMatrix3x4(float e11,float e12,float e13,float e21,float e22,float e23,float e31,float e32,float e33,float e41,float e42,float e43) {
        E11=e11; E12=e12; E13=e13; E21=e21; E22=e22; E23=e23; E31=e31; E32=e32; E33=e33; E41=e41; E42=e42; E43=e43;
    }
    inline void PreMultiply(const CMatrix3x4& l) {
        CMatrix3x4 a(*this);
        E11=a.E21*l.E12+a.E31*l.E13+a.E11*l.E11;
        E12=a.E22*l.E12+a.E32*l.E13+a.E12*l.E11;
        E13=a.E23*l.E12+a.E33*l.E13+a.E13*l.E11;
        E21=a.E21*l.E22+a.E31*l.E23+a.E11*l.E21;
        E22=a.E22*l.E22+a.E32*l.E23+a.E12*l.E21;
        E23=a.E23*l.E22+a.E33*l.E23+a.E13*l.E21;
        E31=a.E21*l.E32+a.E31*l.E33+a.E11*l.E31;
        E32=a.E22*l.E32+a.E32*l.E33+a.E12*l.E31;
        E33=a.E23*l.E32+a.E33*l.E33+a.E13*l.E31;
        E41=a.E21*l.E42+a.E31*l.E43+a.E11*l.E41+a.E41;
        E42=a.E22*l.E42+a.E32*l.E43+a.E12*l.E41+a.E42;
        E43=a.E23*l.E42+a.E33*l.E43+a.E13*l.E41+a.E43;
    }
};
template<class T>class CArray{public:T values[8];unsigned count;CArray():count(1){}unsigned size()const{return count;}T& operator[](unsigned n){Check(n<count);return values[n];}const T& operator[](unsigned n)const{Check(n<count);return values[n];}void resize(unsigned n){Check(n>=1&&n<=8);count=n;}};
class CEngineCamera{public:C3DVector pos;CMatrix3x4 view;const C3DVector& PeekCameraPos()const{return pos;}const CMatrix3x4& PeekNonHomogenousViewTransform()const{return view;}};
class CEngineComponentParams{public:unsigned long SceneFilterFlags;};
struct CIDrawEnvironmentParams{struct CThemeChannel{CRGBFloatColour Colours[19];float Scalers[30];};C3DVector SunVector,MoonVector,DiffuseVector;CArray<CThemeChannel> ThemeChannels;};
class CIDrawEnvironment{public:CIDrawEnvironmentParams params;const CIDrawEnvironmentParams& PeekInterpolatedParams()const{Mark(2);return params;}};
class CIDrawWorld{public:CIDrawEnvironment* environment;const CIDrawEnvironment* DrawGetEnvironment()const{Mark(1);return environment;}};
class CEngineWorldMap{public:CIDrawWorld* world;const CIDrawWorld* PeekGameWorld()const{return world;}};
struct CEngineLightingManagerSpotlightParams{unsigned sentinel;};
static const CEngineCamera* shadow_camera;static const CEngineLightingManagerSpotlightParams* shadow_params;static bool shadow_spot;
class CEngineShadowRenderer{public:void UpdateShadowScene(const CEngineCamera& c,bool spot,const CEngineLightingManagerSpotlightParams& p){Mark(7);shadow_camera=&c;shadow_params=&p;shadow_spot=spot;}};
enum EEngineRenderLayer{};enum ELightingManagerShadowMode{};
class CEngineScene{public:void RenderEntireScene(EEngineRenderLayer layer){Check((int)layer==1);Mark(3);}};
struct CEnginePrimitiveThemeChannelDesc{unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;};
class CEngineLightingManager{public:
 struct CThemeChannel{CRGBFloatColour DiffuseColour,AmbientColour,BacklightColour,ReflectionColour,FogColour;float FogStartZ,FogEndZ,ShadowFactor,FadedShadowFactor;C4DVector FogTransform;};
 C3DVector SunVector,MoonVector,DiffuseVector;
 CArray<CThemeChannel> ThemeChannels;
 CRGBFloatColour PrimitiveDiffuseColour,PrimitiveAmbientColour,PrimitiveBacklightColour,MistEffectColour;
 bool FadedShadowDiffuseFactorEnabled;long SelfIlluminationAmbientPercentage;CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;unsigned long StateUpdateFlags;
 float LookupGridWorldX,LookupGridWorldY;void* CurrentShaderLocalLights;ELightingManagerShadowMode ShadowMode;CEngineLightingManagerSpotlightParams CurrentShadowedSpotLightParams;CArray<CEngineScene*>* SceneList;
 void FrameStart(const CEngineComponentParams&);
 void UpdateShadowScene(const CEngineCamera&);
 C4DVector CalculateFogTransformMatrix(float,float);
 void UpdateShadowedSpotLights(){Mark(4);}void AddLightsToLookupGrid(){Mark(5);}void UpdateSIMDVarBlock(){Check(CurrentShaderLocalLights==0);Mark(6);}
};
namespace NEngineInternal{CEngineWorldMap* GPEngineWorldMap;CEngineCamera* GPCamera;CEngineShadowRenderer* GPShadowRenderer;}
inline void CEngineLightingManager::UpdateShadowScene(const CEngineCamera& camera)
{
 if(ShadowMode==(ELightingManagerShadowMode)2)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,true,CurrentShadowedSpotLightParams);
 else if(ShadowMode==(ELightingManagerShadowMode)1)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,false,CurrentShadowedSpotLightParams);
}

C4DVector CEngineLightingManager::CalculateFogTransformMatrix(float fog_start,float fog_end)
{
 float zscale=1.0f/(fog_end-fog_start);
 float zofs=-zscale*fog_start;
 CMatrix3x4 fog_transform(0,0,0,0,0,0,zscale,0,0,zofs,0,0);
 fog_transform.PreMultiply(NEngineInternal::GPCamera->PeekNonHomogenousViewTransform());
 return C4DVector(fog_transform.E11,fog_transform.E21,fog_transform.E31,fog_transform.E41);
}
void CEngineLightingManager::FrameStart(const CEngineComponentParams& comp_params)
{
    if (NEngineInternal::GPEngineWorldMap) {
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        SunVector = params.SunVector;
        MoonVector = params.MoonVector;
        DiffuseVector = params.DiffuseVector;
        DiffuseVector.Normalise();
        ThemeChannels.resize(params.ThemeChannels.size());
        for (long channel = 0; channel < ThemeChannels.size(); ++channel) {
            ThemeChannels[channel].DiffuseColour = params.ThemeChannels[channel].Colours[0];
            ThemeChannels[channel].AmbientColour = params.ThemeChannels[channel].Colours[1];
            ThemeChannels[channel].BacklightColour = params.ThemeChannels[channel].Colours[2];
            ThemeChannels[channel].ReflectionColour = params.ThemeChannels[channel].Colours[3];
            ThemeChannels[channel].FogStartZ = params.ThemeChannels[channel].Scalers[6];
            ThemeChannels[channel].FogEndZ = params.ThemeChannels[channel].Scalers[7];
            ThemeChannels[channel].FogColour = params.ThemeChannels[channel].Colours[5];
            ThemeChannels[channel].ShadowFactor = params.ThemeChannels[channel].Scalers[21];
            ThemeChannels[channel].FadedShadowFactor = params.ThemeChannels[channel].Scalers[22];
            ThemeChannels[channel].FogTransform = CalculateFogTransformMatrix(ThemeChannels[channel].FogStartZ, ThemeChannels[channel].FogEndZ);
        }
        MistEffectColour = params.ThemeChannels[0].Colours[4];
    }
    PrimitiveDiffuseColour = ThemeChannels[0].DiffuseColour;
    PrimitiveAmbientColour = ThemeChannels[0].AmbientColour;
    PrimitiveBacklightColour = ThemeChannels[0].BacklightColour;
    FadedShadowDiffuseFactorEnabled = false;
    SelfIlluminationAmbientPercentage = 0;
    CurrentThemeChannel.ChannelIndex0 = 0;
    CurrentThemeChannel.ChannelIndex1 = 0;
    CurrentThemeChannel.ChannelBlendFactor = 0;
    StateUpdateFlags = 0xf;
    LookupGridWorldX = (float)(GFFloatToLongFloor((NEngineInternal::GPCamera->PeekCameraPos().X - 128.0f) * 0.0625f) << 4);
    LookupGridWorldY = (float)(GFFloatToLongFloor((NEngineInternal::GPCamera->PeekCameraPos().Y - 128.0f) * 0.0625f) << 4);
    (*SceneList)[0]->RenderEntireScene((EEngineRenderLayer)1);
    UpdateShadowedSpotLights();
    AddLightsToLookupGrid();
    CurrentShaderLocalLights = 0;
    UpdateSIMDVarBlock();
    if (comp_params.SceneFilterFlags & 1)
        UpdateShadowScene(*NEngineInternal::GPCamera);
}

static bool SameColour(const CRGBFloatColour& a,const CRGBFloatColour& b){return a.R==b.R&&a.G==b.G&&a.B==b.B&&a.A==b.A;}
static bool SameVector(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){
 CEngineCamera camera;CEngineShadowRenderer renderer;CEngineScene scene;CArray<CEngineScene*> scenes;scenes.values[0]=&scene;
 CIDrawEnvironment environment;CIDrawWorld world;world.environment=&environment;CEngineWorldMap map;map.world=&world;
 NEngineInternal::GPCamera=&camera;NEngineInternal::GPShadowRenderer=&renderer;
 unsigned cases=0;
 for(unsigned present=0;present<2;++present)for(unsigned channels=1;channels<=4;++channels)for(unsigned mode=0;mode<4;++mode)for(unsigned filter=0;filter<4;++filter)for(unsigned position=0;position<4;++position)for(unsigned vector_case=0;vector_case<4;++vector_case){
  CEngineLightingManager manager;std::memset(&manager,0,sizeof manager);manager.ThemeChannels.count=2;manager.SceneList=&scenes;manager.CurrentThemeChannel.Pad=0xa5;manager.StateUpdateFlags=0x777;manager.ShadowMode=(ELightingManagerShadowMode)mode;manager.CurrentShadowedSpotLightParams.sentinel=0x1234;manager.FadedShadowDiffuseFactorEnabled=true;manager.SelfIlluminationAmbientPercentage=99;manager.CurrentShaderLocalLights=&manager;
  manager.SunVector=C3DVector(7,8,9);manager.MoonVector=C3DVector(10,11,12);manager.DiffuseVector=C3DVector(13,14,15);
  for(unsigned i=0;i<8;++i){float a[sizeof(manager.ThemeChannels.values[i])/sizeof(float)];for(unsigned j=0;j<sizeof(a)/sizeof(a[0]);++j)a[j]=(float)(200+i*32+j);std::memcpy(&manager.ThemeChannels.values[i],a,sizeof a);}
  manager.MistEffectColour.R=101;manager.MistEffectColour.G=102;manager.MistEffectColour.B=103;manager.MistEffectColour.A=104;
  CEngineLightingManager::CThemeChannel old_channels[8];std::memcpy(old_channels,manager.ThemeChannels.values,sizeof old_channels);
  environment.params.ThemeChannels.count=channels;environment.params.SunVector=C3DVector(1,2,3);environment.params.MoonVector=C3DVector(4,5,6);
  const C3DVector input[4]={C3DVector(1,0,0),C3DVector(0,2,0),C3DVector(0,0,4),C3DVector(1,1,0)};const C3DVector expected[4]={C3DVector(0.6875f,0,0),C3DVector(0,0.6875f,0),C3DVector(0,0,0.6875f),C3DVector(0.625f,0.625f,0)};environment.params.DiffuseVector=input[vector_case];
  for(unsigned i=0;i<channels;++i){CIDrawEnvironmentParams::CThemeChannel& c=environment.params.ThemeChannels.values[i];for(unsigned j=0;j<19;++j){c.Colours[j].R=(float)(i*32+j);c.Colours[j].G=(float)(i*32+j+1);c.Colours[j].B=(float)(i*32+j+2);c.Colours[j].A=(float)(i*32+j+3);}for(unsigned j=0;j<30;++j)c.Scalers[j]=(float)(i+j);c.Scalers[6]=16.0f*(float)i;c.Scalers[7]=16.0f*(float)(i+1);}
  camera.pos=C3DVector(128.0f+16.0f*position+0.5f,128.0f+16.0f*(3-position)+0.25f,9);camera.view=CMatrix3x4(1,0,0,0,1,0,0,0,1,0,0,16.0f*(position&1));
  NEngineInternal::GPEngineWorldMap=present?&map:0;CEngineComponentParams comp;comp.SceneFilterFlags=filter;trace_count=0;shadow_camera=0;shadow_params=0;
  manager.FrameStart(comp);
  unsigned cursor=0;if(present){Check(trace[cursor++]==1&&trace[cursor++]==2);}for(unsigned n=3;n<=6;++n)Check(trace[cursor++]==n);if((filter&1)&&(mode==1||mode==2)){Check(trace[cursor++]==7);Check(shadow_camera==&camera&&shadow_params==&manager.CurrentShadowedSpotLightParams&&shadow_spot==(mode==2));}else Check(shadow_camera==0&&shadow_params==0);Check(cursor==trace_count);
  Check(manager.ThemeChannels.size()==(present?channels:2));
  if(present){Check(SameVector(manager.SunVector,environment.params.SunVector)&&SameVector(manager.MoonVector,environment.params.MoonVector)&&SameVector(manager.DiffuseVector,expected[vector_case]));for(unsigned i=0;i<channels;++i){const CEngineLightingManager::CThemeChannel& a=manager.ThemeChannels.values[i];const CIDrawEnvironmentParams::CThemeChannel& b=environment.params.ThemeChannels.values[i];Check(SameColour(a.DiffuseColour,b.Colours[0])&&SameColour(a.AmbientColour,b.Colours[1])&&SameColour(a.BacklightColour,b.Colours[2])&&SameColour(a.ReflectionColour,b.Colours[3])&&SameColour(a.FogColour,b.Colours[5]));Check(a.FogStartZ==b.Scalers[6]&&a.FogEndZ==b.Scalers[7]&&a.ShadowFactor==b.Scalers[21]&&a.FadedShadowFactor==b.Scalers[22]);Check(a.FogTransform.X==0&&a.FogTransform.Y==0&&a.FogTransform.Z==0.0625f&&a.FogTransform.W==(float)(position&1)-(float)i);}Check(SameColour(manager.MistEffectColour,environment.params.ThemeChannels.values[0].Colours[4]));}
  else{Check(std::memcmp(old_channels,manager.ThemeChannels.values,sizeof old_channels)==0);Check(SameVector(manager.SunVector,C3DVector(7,8,9))&&SameVector(manager.MoonVector,C3DVector(10,11,12))&&SameVector(manager.DiffuseVector,C3DVector(13,14,15)));Check(manager.MistEffectColour.R==101&&manager.MistEffectColour.A==104);}
  Check(SameColour(manager.PrimitiveDiffuseColour,manager.ThemeChannels.values[0].DiffuseColour)&&SameColour(manager.PrimitiveAmbientColour,manager.ThemeChannels.values[0].AmbientColour)&&SameColour(manager.PrimitiveBacklightColour,manager.ThemeChannels.values[0].BacklightColour));
  Check(!manager.FadedShadowDiffuseFactorEnabled&&manager.SelfIlluminationAmbientPercentage==0&&manager.CurrentThemeChannel.ChannelIndex0==0&&manager.CurrentThemeChannel.ChannelIndex1==0&&manager.CurrentThemeChannel.ChannelBlendFactor==0&&manager.CurrentThemeChannel.Pad==0xa5&&manager.StateUpdateFlags==15);Check(manager.LookupGridWorldX==16.0f*position&&manager.LookupGridWorldY==16.0f*(3-position));Check(manager.CurrentShaderLocalLights==0&&manager.CurrentShadowedSpotLightParams.sentinel==0x1234&&(unsigned)manager.ShadowMode==mode&&comp.SceneFilterFlags==filter);++cases;
 }
 Check(cases==2048);
#if defined(_MSC_VER)
 unsigned bits=0x12345678u;for(unsigned i=0;i<32768;++i){bits=bits*1664525u+1013904223u;float value;std::memcpy(&value,&bits,4);Check(GFFloatToLongFloor(value)==fable_portable::GFFloatToLongFloor(value));}
#endif
 const float values[]={0.0f,-0.0f,0.5f,-0.5f,1.0f,-1.0f,1.5f,-1.5f,2.0f,-2.0f};const long answer[]={0,0,0,-1,1,-1,1,-2,2,-2};for(unsigned i=0;i<10;++i)Check(GFFloatToLongFloor(values[i])==answer[i]);
 std::puts("00b484b0_TEST PASS");return 0;
}