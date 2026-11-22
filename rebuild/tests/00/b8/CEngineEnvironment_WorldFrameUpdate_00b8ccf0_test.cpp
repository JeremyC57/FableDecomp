#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
// Original source locals and member expression sequence recovered from FableWin02dc4700 and Ego_d.
// Leaf: ?WorldFrameUpdate@CEngineEnvironment@@UAEX_N@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_CENGINEENVIRONMENT_WORLDFRAMEUPDATE_PASS26
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#pragma intrinsic(sqrt)
namespace NEngineInternal {extern CEngineWorldMap* GPEngineWorldMap;}
void CEngineEnvironment::WorldFrameUpdate(bool time_for_full_update)
{
    if(time_for_full_update || EnvironmentalEffectsAlwaysUpdate) {
        const CIDrawEnvironment* penvironment=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment();
        C3DVector wind_dir=penvironment->PeekInterpolatedParams().ThemeWindDirection;
        if(wind_dir.GetAccurateMagnitude()<0.1f)
            wind_dir=PreviousWindVector;
        wind_dir.AccurateNormalise();
        float theme_wind_speed=penvironment->PeekInterpolatedParams().ThemeWindSpeed;
        const float wind_speed=theme_wind_speed<0.4f?theme_wind_speed:0.4f;
        PreviousWindVector=NextWindVector;
        NextWindVector=wind_dir;
        PreviousWindSpeed=NextWindSpeed;
        NextWindSpeed=wind_speed;
        C3DVector velocity=WindSkewHistory[0]-WindSkewHistory[1];
        C3DVector skew_vector=WindSkewHistory[0]*0.75f+velocity*0.9f;
        skew_vector+=wind_dir*wind_speed;
        for(long c1=63;c1>0;--c1)
            WindSkewHistory[c1]=WindSkewHistory[c1-1];
        WindSkewHistory[0]=skew_vector;
    }
}

#include "engine/CEngineInit.h"
#include <cstdio>
#include <cstring>
#include <cmath>
static bool fixture_always=false;
static int environment_calls=0,param_calls=0,normalise_calls=0;
static CIDrawWorld* fixture_world=0;
CCharString::CCharString() {}
CCharString::CCharString(const CCharString&) {}
CCharString::~CCharString() {}
CWideString::CWideString() {}
CWideString::~CWideString() {}
CEngineMemoryInit::CEngineMemoryInit() {}
CEngineInit::CEngineInit() {}
CEngineInit::~CEngineInit() {}
CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) {return false;}
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() {return CCharString();}
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() {return true;}
CEngineWorldMap::CEngineWorldMap(const CEngineInit&) : Map(0),MapArrayWidth(0),MapArrayHeight(0),MapOffsetX(0),MapOffsetY(0),GameWorld(fixture_world) {}
CEngineWorldMap::~CEngineWorldMap() {}
unsigned long CEngineWorldMap::PeekSceneFilterFlags()const {return 0;}
CEngineEnvironment::CEngineEnvironment() :CurrentWindVector(0,0,0),NextWindVector(0,1,0),PreviousWindVector(1,0,0),CurrentWindSpeed(0),NextWindSpeed(0.2f),PreviousWindSpeed(0.1f),EnvironmentalEffectsAlwaysUpdate(fixture_always) {
 for(int i=0;i<64;++i)WindSkewHistory[i].Initialise((float)(1+i),(float)(2+2*i),(float)(3-3*i));
}
unsigned long CEngineEnvironment::PeekSceneFilterFlags()const{return 0;}
C3DVector& C3DVector::AccurateNormalise() {
 ++normalise_calls;float n=(float)std::sqrt(X*X+Y*Y+Z*Z);if(n>0){X/=n;Y/=n;Z/=n;}return *this;
}
CIDrawEnvironment::~CIDrawEnvironment() {}
CIDrawWorld::~CIDrawWorld() {}
struct TestEnvironment:CIDrawEnvironment {
 CIDrawEnvironmentParams params;
 virtual const CIDrawEnvironmentParams& PeekInterpolatedParams()const {++param_calls;return params;}
 virtual const C3DVector& PeekWindDirection()const {return params.WindDirection;}
 virtual float PeekWindSpeed()const {return params.WindSpeed;}
 virtual const C3DVector& PeekSunVector()const {return params.SunVector;}
 virtual const C3DVector& PeekMoonVector()const {return params.MoonVector;}
};
struct TestWorld:CIDrawWorld {
 const TestEnvironment* env;explicit TestWorld(const TestEnvironment*e):env(e){}
 virtual const CProceduralTexture* DrawGetProceduralTexture()const{return 0;}
 virtual const CIDrawEnvironment* DrawGetEnvironment()const{++environment_calls;return env;}
 virtual const CIDrawWorldMap* DrawGetWorldMap()const{return 0;}
};
namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap=0; }
static int failed=0;
#define CHECK(x) do {if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
// Offset reads inspect the actual32-bit shared model; no replacement layout is defined.
static float At(const CEngineEnvironment& env,unsigned off){float f;std::memcpy(&f,(const unsigned char*)&env+off,4);return f;}
static bool Near(float x,float y){return std::fabs(x-y)<0.00002f;}
int main(){
 TestEnvironment environment;TestWorld game(&environment);fixture_world=&game;CEngineInit init;CEngineWorldMap world(init);NEngineInternal::GPEngineWorldMap=&world;
 unsigned nanbits=0x7fc00000;float nan;std::memcpy(&nan,&nanbits,4);
 const float speeds[]={-0.3f,0.0f,0.2f,0.4f,0.40001f,2.0f,nan};
 const float directions[]={0.0f,0.09999f,0.1f,0.10001f,2.0f,-2.0f};
 for(int always=0;always<2;++always)for(int full=0;full<2;++full)for(unsigned s=0;s<7;++s)for(unsigned v=0;v<6;++v){
  fixture_always=always!=0;CEngineEnvironment env;unsigned char before[sizeof(env)];std::memcpy(before,&env,sizeof(env));
  environment.params.ThemeWindDirection.Initialise(0,directions[v],0);environment.params.ThemeWindSpeed=speeds[s];
  environment_calls=param_calls=normalise_calls=0;env.WorldFrameUpdate(full!=0);
  if(!always&&!full){CHECK(!std::memcmp(before,&env,sizeof(env)));CHECK(environment_calls==0&&param_calls==0&&normalise_calls==0);continue;}
  CHECK(environment_calls==1&&param_calls==2&&normalise_calls==1);
  const bool fallback=std::fabs(directions[v])<0.1f;float x=fallback?1.0f:0.0f,y=fallback?0.0f:directions[v]<0?-1.0f:1.0f;
  const float speed=speeds[s]<0.4f?speeds[s]:0.4f;
  CHECK(At(env,0x634)==0&&At(env,0x638)==1&&At(env,0x63c)==0);
  CHECK(At(env,0x628)==x&&At(env,0x62c)==y&&At(env,0x630)==0);
  CHECK(At(env,0x648)==0.2f&&At(env,0x644)==speed);
  CHECK(Near(At(env,0x1c),-0.15f+x*speed)&&Near(At(env,0x20),-0.3f+y*speed)&&Near(At(env,0x24),4.95f));
  for(unsigned i=1;i<64;++i){CHECK(At(env,0x1c+i*12)==(float)i);CHECK(At(env,0x20+i*12)==(float)(2*i));CHECK(At(env,0x24+i*12)==(float)(6-3*(int)i));}
 }
 std::puts(failed?"00b8ccf0_TEST FAIL":"00b8ccf0_TEST PASS");return failed?1:0;
}