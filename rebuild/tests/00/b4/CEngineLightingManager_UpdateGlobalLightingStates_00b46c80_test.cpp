#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
static unsigned caseNumber,ctorCount,dtorCount,initCount,globalCount,ambientCount,shadowCount;static int events[8],ne;
static void Check(bool ok){if(!ok){std::printf("00b46c80_TEST FAIL case%u event%d\n",caseNumber,ne);std::exit(1);}}
static void Event(int n){Check(ne<8);events[ne++]=n;}
static bool Near(float a,float b){return std::fabs((double)a-b)<=0.00001*(1.0+std::fabs((double)b));}
inline float GFSqr(float n){return n*n;}inline float GFRoot(float n){return (float)std::sqrt((double)n);}
class C3DVector{public:float X,Y,Z;};
class CRGBFloatColour{public:
 float R,G,B,A;CRGBFloatColour(){}CRGBFloatColour(float r,float g,float b){Initialise(r,g,b);}CRGBFloatColour(float r,float g,float b,float a){Initialise(r,g,b,a);}
 void Initialise(float r,float g,float b){R=r;G=g;B=b;A=1;}void Initialise(float r,float g,float b,float a){R=r;G=g;B=b;A=a;}
 float GetR()const{return R;}float GetG()const{return G;}float GetB()const{return B;}float GetA()const{return A;}
 CRGBFloatColour operator*(float n)const{return CRGBFloatColour(R*n,G*n,B*n,A*n);}CRGBFloatColour operator+(const CRGBFloatColour& c)const{return CRGBFloatColour(R+c.R,G+c.G,B+c.B,A+c.A);}
 CRGBFloatColour Interpolate(const CRGBFloatColour& c,float t)const{return *this*(1.0f-t)+c*t;}
};
class C4DVector{public:float X,Y,Z,W;C4DVector(const CRGBFloatColour& c){InitialiseFromColour(c);}void InitialiseFromColour(const CRGBFloatColour& c){X=c.GetR();Y=c.GetG();Z=c.GetB();W=c.GetA();}};
static void Equal(const CRGBFloatColour& a,const CRGBFloatColour& b){Check(Near(a.R,b.R)&&Near(a.G,b.G)&&Near(a.B,b.B)&&Near(a.A,b.A));}
static CRGBFloatColour ReferenceBlend(const CRGBFloatColour& a,const CRGBFloatColour& b,double t){return CRGBFloatColour((float)((1-t)*a.R+t*b.R),(float)((1-t)*a.G+t*b.G),(float)((1-t)*a.B+t*b.B),(float)((1-t)*a.A+t*b.A));}
static CRGBFloatColour ReferenceScale(const CRGBFloatColour& a,double n){return CRGBFloatColour((float)(a.R*n),(float)(a.G*n),(float)(a.B*n),(float)(a.A*n));}
struct CLightingSIMDVarBlock{float Zero[4],One[4],DiffuseVectorX[4],DiffuseVectorY[4],DiffuseVectorZ[4],DiffuseColourR[4],DiffuseColourG[4],DiffuseColourB[4],BackColourR[4],BackColourG[4],BackColourB[4],AmbientColourR[4],AmbientColourG[4],AmbientColourB[4],ObjectPosX[4],ObjectPosY[4],ObjectPosZ[4];};
static CRGBFloatColour wantedDiffuse,wantedBack,wantedAmbient;static C3DVector wantedDirection;static float wantedShadow[3];
static void CheckDiffusePublished();
class CShaderLight{public:virtual ~CShaderLight(){CheckDiffusePublished();++dtorCount;Event(3);}};
class CShaderDirectionalLight:public CShaderLight{public:
 C3DVector Direction,Position;CRGBFloatColour Colour,BackColour;
 CShaderDirectionalLight(const C3DVector& d,const CRGBFloatColour& c,const CRGBFloatColour& b):Direction(d),Colour(c),BackColour(b){++ctorCount;Equal(c,wantedDiffuse);Equal(b,wantedBack);Check(d.X==wantedDirection.X&&d.Y==wantedDirection.Y&&d.Z==wantedDirection.Z);}
};
class CShaderRenderManager{public:
 void InitialiseLight(unsigned long index,const CShaderLight& light){Check(index==0&&ctorCount==1&&dtorCount==0);const CShaderDirectionalLight& d=static_cast<const CShaderDirectionalLight&>(light);Equal(d.Colour,wantedDiffuse);Equal(d.BackColour,wantedBack);++initCount;Event(1);}
 void SetLightGlobal(unsigned long index,const C4DVector& v){Check(index==0&&initCount==1&&dtorCount==0);Check(Near(v.X,wantedBack.R)&&Near(v.Y,wantedBack.G)&&Near(v.Z,wantedBack.B)&&Near(v.W,wantedBack.A));++globalCount;Event(2);}
 void SetAmbientColour(const CRGBFloatColour& c){Equal(c,wantedAmbient);++ambientCount;Event(4);}
 void SetShadowFadeFactors(float a,float b,float c){Check(Near(a,wantedShadow[0])&&Near(b,wantedShadow[1])&&Near(c,wantedShadow[2]));++shadowCount;Event(5);}
};
template<class T>class CArray{public:T values[3];T& operator[](unsigned long n){Check(n<3);return values[n];}};
enum ELightingManagerShadowMode{LIGHTING_MANAGER_SHADOW_MODE_NULL=0,LIGHTING_MANAGER_SHADOW_MODE_MAIN_LIGHT=1,LIGHTING_MANAGER_SHADOW_MODE_SPOT_LIGHTS=2};
class CEngineLightingManager{public:
 struct CThemeChannel{CRGBFloatColour DiffuseColour,AmbientColour,BacklightColour,ReflectionColour,FogColour;float FogStartZ,FogEndZ,ShadowFactor,FadedShadowFactor,FogTransform[4];};
 struct CEnginePrimitiveThemeChannelDesc{unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;}CurrentThemeChannel;
 C3DVector DiffuseVector;CArray<CThemeChannel>ThemeChannels;CRGBFloatColour PrimitiveDiffuseColour,PrimitiveAmbientColour,PrimitiveBacklightColour;bool FadedShadowDiffuseFactorEnabled;unsigned long StateUpdateFlags;long SelfIlluminationAmbientPercentage;CLightingSIMDVarBlock* SIMDVarBlock;long ShadowMode;
 void UpdateGlobalLightingStates();
};
static CEngineLightingManager* activeManager;
static void CheckDiffusePublished(){Check((activeManager->StateUpdateFlags&1)==0);for(unsigned i=0;i<4;++i){Check(Near(activeManager->SIMDVarBlock->DiffuseColourR[i],wantedDiffuse.R));Check(Near(activeManager->SIMDVarBlock->DiffuseColourG[i],wantedDiffuse.G));Check(Near(activeManager->SIMDVarBlock->DiffuseColourB[i],wantedDiffuse.B));}}
namespace NEngineInternal{CShaderRenderManager* GPShaderRenderManager;}
void CEngineLightingManager::UpdateGlobalLightingStates()
{
    float channel_blend_factor1=CurrentThemeChannel.ChannelBlendFactor/255.0f;
    if(StateUpdateFlags&1) {
        if(CurrentThemeChannel.ChannelBlendFactor>0) {
            PrimitiveDiffuseColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].DiffuseColour,channel_blend_factor1);
            PrimitiveBacklightColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].BacklightColour,channel_blend_factor1);
        } else {
            PrimitiveDiffuseColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour;
            PrimitiveBacklightColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour;
        }
        if(FadedShadowDiffuseFactorEnabled) {
            float shadow_factor;
            if(CurrentThemeChannel.ChannelBlendFactor>0)
                shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor;
            else
                shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
            PrimitiveDiffuseColour=PrimitiveDiffuseColour*shadow_factor;
        }
        CShaderDirectionalLight sunlight(DiffuseVector,PrimitiveDiffuseColour,PrimitiveBacklightColour);
        NEngineInternal::GPShaderRenderManager->InitialiseLight(0,sunlight);
        NEngineInternal::GPShaderRenderManager->SetLightGlobal(0,C4DVector(PrimitiveBacklightColour));
        StateUpdateFlags&=~1;
        for(long c=0;c<4;++c) {
            SIMDVarBlock->DiffuseColourR[c]=PrimitiveDiffuseColour.R;
            SIMDVarBlock->DiffuseColourG[c]=PrimitiveDiffuseColour.G;
            SIMDVarBlock->DiffuseColourB[c]=PrimitiveDiffuseColour.B;
        }
    }
    if(StateUpdateFlags&2) {
        if(CurrentThemeChannel.ChannelBlendFactor>0)
            PrimitiveAmbientColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].AmbientColour,channel_blend_factor1);
        else
            PrimitiveAmbientColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour;
        if(SelfIlluminationAmbientPercentage>0) {
            float sqr_illum=GFSqr(SelfIlluminationAmbientPercentage*0.01f);
            CRGBFloatColour ambient(GFRoot(GFSqr(PrimitiveAmbientColour.GetR())+sqr_illum),GFRoot(GFSqr(PrimitiveAmbientColour.GetG())+sqr_illum),GFRoot(GFSqr(PrimitiveAmbientColour.GetB())+sqr_illum));
            if(ambient.R>1.0f)ambient.R=1.0f;
            if(ambient.G>1.0f)ambient.G=1.0f;
            if(ambient.B>1.0f)ambient.B=1.0f;
            PrimitiveAmbientColour=ambient;
        }
        NEngineInternal::GPShaderRenderManager->SetAmbientColour(PrimitiveAmbientColour);
        StateUpdateFlags&=~2;
    }
    if(ShadowMode==LIGHTING_MANAGER_SHADOW_MODE_MAIN_LIGHT&&(StateUpdateFlags&8)&&!FadedShadowDiffuseFactorEnabled) {
        float shadow_factor,faded_shadow_factor;
        if(CurrentThemeChannel.ChannelBlendFactor>0) {
            shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].ShadowFactor;
            faded_shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor;
        } else {
            shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor;
            faded_shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
        }
        shadow_factor=1.0f-shadow_factor;
        float std_light_factor=1.0f-faded_shadow_factor-shadow_factor;
        float std_light_offset=faded_shadow_factor;
        NEngineInternal::GPShaderRenderManager->SetShadowFadeFactors(shadow_factor,std_light_factor,std_light_offset);
    }
}

int main(){
 CEngineLightingManager manager;CShaderRenderManager shader;CLightingSIMDVarBlock simd;NEngineInternal::GPShaderRenderManager=&shader;manager.SIMDVarBlock=&simd;activeManager=&manager;
 const unsigned char blends[6]={0,1,64,127,254,255};const long illuminations[6]={-7,0,1,50,100,200};const float values[6]={-1.0f,-0.25f,0.0f,0.25f,0.75f,1.5f};caseNumber=0;
 for(unsigned bi=0;bi<6;++bi)for(unsigned pair=0;pair<9;++pair)for(unsigned flags=0;flags<16;++flags)for(unsigned fade=0;fade<2;++fade)for(unsigned mode=0;mode<3;++mode)for(unsigned ii=0;ii<6;++ii){
  unsigned first=pair%3,second=pair/3;manager.CurrentThemeChannel.ChannelIndex0=(unsigned char)first;manager.CurrentThemeChannel.ChannelIndex1=(unsigned char)second;manager.CurrentThemeChannel.ChannelBlendFactor=blends[bi];manager.FadedShadowDiffuseFactorEnabled=fade!=0;manager.ShadowMode=(long)mode;manager.SelfIlluminationAmbientPercentage=illuminations[ii];manager.StateUpdateFlags=flags|0x40;
  for(unsigned c=0;c<3;++c){manager.ThemeChannels[c].DiffuseColour=CRGBFloatColour(values[(c+flags)%6],values[(c+1+flags)%6],values[(c+2+flags)%6],0.5f);manager.ThemeChannels[c].BacklightColour=CRGBFloatColour(0.125f*(c+1),-0.25f*(c+1),0.5f*(c+1),0.25f);manager.ThemeChannels[c].AmbientColour=CRGBFloatColour(values[(c+ii)%6],values[(c+ii+1)%6],values[(c+ii+2)%6],0.75f);manager.ThemeChannels[c].ShadowFactor=(float)c*0.25f;manager.ThemeChannels[c].FadedShadowFactor=(float)(3-c)*0.125f;}
  manager.DiffuseVector.X=-0.25f;manager.DiffuseVector.Y=0.5f;manager.DiffuseVector.Z=0.75f;wantedDirection=manager.DiffuseVector;
  CRGBFloatColour originalDiffuse(-8,-7,-6,-5),originalAmbient(-4,-3,-2,-1),originalBack(4,3,2,1);manager.PrimitiveDiffuseColour=originalDiffuse;manager.PrimitiveAmbientColour=originalAmbient;manager.PrimitiveBacklightColour=originalBack;
  CLightingSIMDVarBlock before;float initialWords[68];for(unsigned i=0;i<68;++i)initialWords[i]=(float)(i+10);std::memcpy(&simd,initialWords,sizeof(simd));before=simd;
  double t=(double)blends[bi]/255.0;const CEngineLightingManager::CThemeChannel& a=manager.ThemeChannels[first];const CEngineLightingManager::CThemeChannel& b=manager.ThemeChannels[second];
  wantedDiffuse=originalDiffuse;wantedBack=originalBack;wantedAmbient=originalAmbient;
  if(flags&1){wantedDiffuse=ReferenceBlend(a.DiffuseColour,b.DiffuseColour,t);wantedBack=ReferenceBlend(a.BacklightColour,b.BacklightColour,t);if(fade)wantedDiffuse=ReferenceScale(wantedDiffuse,(1-t)*a.FadedShadowFactor+t*b.FadedShadowFactor);}
  if(flags&2){wantedAmbient=ReferenceBlend(a.AmbientColour,b.AmbientColour,t);if(illuminations[ii]>0){double lift=(double)illuminations[ii]/100.0;double ar=std::sqrt((double)wantedAmbient.R*wantedAmbient.R+lift*lift),ag=std::sqrt((double)wantedAmbient.G*wantedAmbient.G+lift*lift),ab=std::sqrt((double)wantedAmbient.B*wantedAmbient.B+lift*lift);wantedAmbient=CRGBFloatColour((float)(ar>1?1:ar),(float)(ag>1?1:ag),(float)(ab>1?1:ab),1);}}
  double shadow=(1-t)*a.ShadowFactor+t*b.ShadowFactor,faded=(1-t)*a.FadedShadowFactor+t*b.FadedShadowFactor;wantedShadow[0]=(float)(1-shadow);wantedShadow[1]=(float)(shadow-faded);wantedShadow[2]=(float)faded;
  ctorCount=dtorCount=initCount=globalCount=ambientCount=shadowCount=0;ne=0;
  manager.UpdateGlobalLightingStates();
  Equal(manager.PrimitiveDiffuseColour,wantedDiffuse);Equal(manager.PrimitiveBacklightColour,wantedBack);Equal(manager.PrimitiveAmbientColour,wantedAmbient);
  Check(ctorCount==((flags&1)?1u:0u)&&dtorCount==ctorCount&&initCount==ctorCount&&globalCount==ctorCount);Check(ambientCount==((flags&2)?1u:0u));bool shadowExpected=mode==1&&(flags&8)&&!fade;Check(shadowCount==(shadowExpected?1u:0u));Check(manager.StateUpdateFlags==((flags|0x40)&~3ul));
  for(unsigned c=0;c<4;++c){Check(Near(simd.DiffuseColourR[c],(flags&1)?wantedDiffuse.R:before.DiffuseColourR[c]));Check(Near(simd.DiffuseColourG[c],(flags&1)?wantedDiffuse.G:before.DiffuseColourG[c]));Check(Near(simd.DiffuseColourB[c],(flags&1)?wantedDiffuse.B:before.DiffuseColourB[c]));}
  float oldwords[68],words[68];std::memcpy(oldwords,&before,sizeof(before));std::memcpy(words,&simd,sizeof(simd));for(unsigned i=0;i<68;++i)if(i<20||i>=32)Check(words[i]==oldwords[i]);
  int expected[5],n=0;if(flags&1){expected[n++]=1;expected[n++]=2;expected[n++]=3;}if(flags&2)expected[n++]=4;if(shadowExpected)expected[n++]=5;Check(ne==n);for(int e=0;e<n;++e)Check(events[e]==expected[e]);
  Check(manager.CurrentThemeChannel.ChannelIndex0==first&&manager.CurrentThemeChannel.ChannelIndex1==second&&manager.CurrentThemeChannel.ChannelBlendFactor==blends[bi]);Check(manager.FadedShadowDiffuseFactorEnabled==(fade!=0)&&manager.ShadowMode==(long)mode&&manager.SelfIlluminationAmbientPercentage==illuminations[ii]);Check(manager.DiffuseVector.X==wantedDirection.X&&manager.DiffuseVector.Y==wantedDirection.Y&&manager.DiffuseVector.Z==wantedDirection.Z);++caseNumber;
 }
 Check(caseNumber==31104);std::puts("00b46c80_TEST PASS");return 0;
}