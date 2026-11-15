// 00c41060 complete production-body fixture. Declared native callees are recording doubles; ABI checked separately.
#include <stdio.h>
#include <string.h>
#include <float.h>
#define C_CAPS_3D_HRTF_FULL 0x100000u
#define C_CAPS_3D_HRTF_DELAY 0x200000u
#define C_CAPS_3D_VOLPAN 0x400000u
#define C_CAPS_REVERB 0x80u
enum CHRTFMODE {C_HRTF_DISABLED=0,C_HRTF_FULL=0x100000,C_HRTF_DELAY=0x200000,C_HRTF_VOLPAN=0x400000};
class CCodaVector {public:float x,y,z;void Set(float a,float b,float c){x=a;y=b;z=c;}void Zero(){x=y=z=0.0f;}};
struct CODA3DSOURCE {CCodaVector vPosition,vVelocity,vConeDirection,vDirection;float fDistance,fDistanceGain,fConeGain,fDoppler;int iDirect,iDirectHF;unsigned int uiDirectHFReference;int iEnvFx,iEnvFxHF;unsigned int uiEnvFxHFReference;float fMacroFx;};
struct _C3DSOURCEENVFX {int iEnvironment,iEnvironmentHF,iObstruction;float fObstructionLFRatio;int iOcclusion;float fOcclusionLFRatio;int iExclusion;float fExclusionLFRatio,fRolloffFactor,fAirAbsorptionFactor;};
struct _CVOICECAPS {int nVoices,nFreeVoices;unsigned int ffCaps,ffFormats,nMinSamplesPerSec,nMaxSamplesPerSec;};
struct _CCAPS {_CVOICECAPS HW2DVoice,HW3DVoice,SW2DVoice,SW3DVoice;};static _CCAPS g_SalCaps;
struct CSalVoice{};static CSalVoice oldVoice,newVoice;
static int events[16],nevents;static bool bad,baseResult;static int allocateError,CODA_Error,callbackError;static unsigned int expectedCaps,expectedMode,hfReference,baseStatus;static void Event(int n){if(nevents>=16)bad=true;else events[nevents++]=n;}
class CCoda3DSource;static CCoda3DSource*expectedSource;
class CCodaSource {public:unsigned int m_ffStatus,m_ffCaps;CSalVoice*m_pVoice;virtual~CCodaSource(){}virtual bool Create(unsigned int);virtual void Release(){bad=true;}};
struct SourceState {CODA3DSOURCE m_3D;CCodaVector m_vPosition,m_vVelocity,m_vConeDirection;float m_fMaxDistance,m_fMinDistance,m_fRefDistance,m_fRolloff,m_fDoppler;unsigned int m_uiConeInner,m_uiConeOuter;float m_fConeOuterGain;int m_iDirect,m_iDirectHF;_C3DSOURCEENVFX m_EnvFx;CHRTFMODE m_HrtfMode;float(*m_pfnDistanceCallback)(void*,void*,float,unsigned int);unsigned int m_DistanceCallbackData,m_ffCommit;};
class CCoda3DSource:public CCodaSource,public SourceState {public:virtual bool Create(unsigned int);};
class TestSource:public CCoda3DSource {public:virtual void Release(){Event(6);if(this!=expectedSource)bad=true;m_pVoice=0;m_ffStatus|=0x40000;m_ffCommit=0x12345678;}};
bool CCodaSource::Create(unsigned int caps){Event(1);if(this!=expectedSource||caps!=expectedCaps||expectedSource->m_HrtfMode!=(CHRTFMODE)expectedMode)bad=true;m_ffCaps=~caps;m_ffStatus=baseStatus;m_pVoice=&oldVoice;return baseResult;}
class CSalContext {public:int AllocateSw3DVoice(const CCodaSource*p,CSalVoice**out){Event(3);if(p!=expectedSource||out!=&expectedSource->m_pVoice)bad=true;*out=&newVoice;return allocateError;}};
class CSalPlatform:public CSalContext{};static CSalPlatform platform;
class CCodaContext {public:void(*hook)(int);CSalPlatform&GetSal(){Event(2);return platform;}unsigned int GetHFReference()const{Event(7);return hfReference;}void SetError(int);};static CCodaContext CODA;
static void Callback(int n){Event(5);callbackError=n;}
void CCodaContext::SetError(int n){Event(4);if(this!=&CODA)bad=true;if(!CODA_Error){CODA_Error=n;if(hook)hook(n);}}

bool CCoda3DSource::Create(unsigned int ffCaps)
{
    if(ffCaps&C_CAPS_3D_HRTF_FULL)
        m_HrtfMode=C_HRTF_FULL;
    else if(ffCaps&C_CAPS_3D_HRTF_DELAY)
        m_HrtfMode=C_HRTF_DELAY;
    else
        m_HrtfMode=static_cast<CHRTFMODE>(ffCaps&C_CAPS_3D_VOLPAN);
    if(!CCodaSource::Create(ffCaps))
        return false;
    if(!g_SalCaps.HW3DVoice.nFreeVoices || (ffCaps&~g_SalCaps.HW3DVoice.ffCaps)) {
        if(g_SalCaps.SW3DVoice.nFreeVoices==-1) {
            int error=CODA.GetSal().AllocateSw3DVoice(this,&m_pVoice);
            if(error) {
                CODA.SetError(error);
                Release();
                return false;
            }
            m_ffStatus|=0x20000;
        }
    }
    m_vPosition.Set(0.0f,0.0f,1.0f);
    m_vVelocity.Zero();
    m_vConeDirection.Set(0.0f,0.0f,1.0f);
    m_fMaxDistance=FLT_MAX;
    m_fMinDistance=1.0f;
    m_fRefDistance=1.0f;
    m_fRolloff=1.0f;
    m_fDoppler=1.0f;
    m_uiConeInner=360;
    m_uiConeOuter=360;
    m_fConeOuterGain=1.0f;
    m_iDirect=0;
    m_iDirectHF=0;
    if(ffCaps&C_CAPS_REVERB) {
        m_EnvFx.iEnvironment=0;
        m_EnvFx.iEnvironmentHF=0;
        m_EnvFx.iObstruction=0;
        m_EnvFx.fObstructionLFRatio=0.0f;
        m_EnvFx.iOcclusion=0;
        m_EnvFx.fOcclusionLFRatio=0.0f;
        m_EnvFx.iExclusion=0;
        m_EnvFx.fExclusionLFRatio=0.0f;
        m_EnvFx.fRolloffFactor=1.0f;
        m_EnvFx.fAirAbsorptionFactor=1.0f;
    }
    m_pfnDistanceCallback=0;
    m_DistanceCallbackData=0;
    m_3D.vPosition=m_vPosition;
    m_3D.vVelocity=m_vVelocity;
    m_3D.vConeDirection=m_vConeDirection;
    m_3D.vDirection=m_vPosition;
    m_3D.fDistance=1.0f;
    m_3D.fDistanceGain=1.0f;
    m_3D.fConeGain=1.0f;
    m_3D.fDoppler=1.0f;
    m_3D.fMacroFx=1.0f;
    m_3D.iDirect=0;
    m_3D.iDirectHF=0;
    m_3D.uiDirectHFReference=CODA.GetHFReference();
    m_3D.iEnvFx=(ffCaps&C_CAPS_REVERB)?0:-10000;
    m_3D.iEnvFxHF=0;
    m_3D.uiEnvFxHFReference=5000;
    m_ffCommit=0;
    return true;
}


static float DistanceCallback(void*,void*,float,unsigned int){bad=true;return 0;}
static bool V(const CCodaVector&v,float x,float y,float z){return v.x==x&&v.y==y&&v.z==z;}
static int Fail(int id,const char*s){printf("00c41060_TEST FAIL %d %s\n",id,s);return 1;}
static void Init(SourceState&s){memset(&s,0,sizeof(s));s.m_vPosition.Set(2,3,4);s.m_vVelocity.Set(5,6,7);s.m_vConeDirection.Set(8,9,10);s.m_fMaxDistance=s.m_fMinDistance=s.m_fRefDistance=s.m_fRolloff=s.m_fDoppler=s.m_fConeOuterGain=2;s.m_uiConeInner=111;s.m_uiConeOuter=222;s.m_iDirect=333;s.m_iDirectHF=444;s.m_EnvFx.iEnvironment=11;s.m_EnvFx.iEnvironmentHF=12;s.m_EnvFx.iObstruction=13;s.m_EnvFx.fObstructionLFRatio=2;s.m_EnvFx.iOcclusion=14;s.m_EnvFx.fOcclusionLFRatio=3;s.m_EnvFx.iExclusion=15;s.m_EnvFx.fExclusionLFRatio=4;s.m_EnvFx.fRolloffFactor=5;s.m_EnvFx.fAirAbsorptionFactor=6;s.m_HrtfMode=C_HRTF_VOLPAN;s.m_pfnDistanceCallback=DistanceCallback;s.m_DistanceCallbackData=999;s.m_ffCommit=0x76543210;}
int main(){
 const unsigned int priority[3]={C_CAPS_3D_HRTF_FULL,C_CAPS_3D_HRTF_DELAY,C_CAPS_3D_VOLPAN};int id=0;
 for(unsigned int modes=0;modes<8;++modes)for(int base=0;base<2;++base)for(int hw=-1;hw<=1;++hw)for(int supports=0;supports<2;++supports)for(int sw=-1;sw<=1;++sw)for(int alloc=0;alloc<2;++alloc)for(int reverb=0;reverb<2;++reverb)for(int latch=0;latch<2;++latch)for(int hook=0;hook<2;++hook,++id){
  TestSource e;Init(e);SourceState before;memcpy(&before,static_cast<SourceState*>(&e),sizeof(before));
  expectedCaps=(modes<<20)|(reverb?C_CAPS_REVERB:0u);expectedMode=0;for(int p=0;p<3;++p)if(expectedCaps&priority[p]){expectedMode=priority[p];break;}
  expectedSource=&e;baseResult=base!=0;baseStatus=0x100000u|((id&1)?0x20000u:0u);allocateError=alloc?13:0;CODA_Error=latch?17:0;callbackError=0;CODA.hook=hook?Callback:0;hfReference=6000u+(unsigned int)id;g_SalCaps.HW3DVoice.nFreeVoices=hw;g_SalCaps.HW3DVoice.ffCaps=supports?~0u:0u;g_SalCaps.SW3DVoice.nFreeVoices=sw;nevents=0;bad=false;
  bool hardwareAvailable=hw!=0&&((g_SalCaps.HW3DVoice.ffCaps|expectedCaps)==g_SalCaps.HW3DVoice.ffCaps);
  bool allocation=baseResult&&!hardwareAvailable&&sw==-1;bool failure=allocation&&allocateError!=0;bool success=baseResult&&!failure;
  int want[16],nwant=0;want[nwant++]=1;if(allocation){want[nwant++]=2;want[nwant++]=3;}if(failure){want[nwant++]=4;if(!latch&&hook)want[nwant++]=5;want[nwant++]=6;}if(success)want[nwant++]=7;
  bool result=e.Create(expectedCaps);
  if(result!=success||bad||nevents!=nwant)return Fail(id,"result/boundary count");
  for(int k=0;k<nwant;++k)if(events[k]!=want[k])return Fail(id,"boundary order");
  unsigned int status=baseStatus|(allocation&&!failure?0x20000u:0u)|(failure?0x40000u:0u);CSalVoice*voice=failure?0:allocation?&newVoice:&oldVoice;
  if(e.m_ffCaps!=~expectedCaps||e.m_ffStatus!=status||e.m_pVoice!=voice)return Fail(id,"base/allocation/release effects");
  if(CODA_Error!=(latch?17:failure?13:0)||callbackError!=((failure&&!latch&&hook)?13:0))return Fail(id,"first error latch");
  if(!success){before.m_HrtfMode=(CHRTFMODE)expectedMode;if(failure)before.m_ffCommit=0x12345678;if(memcmp(&before,static_cast<SourceState*>(&e),sizeof(before)))return Fail(id,"failure preserves derived state");continue;}
  if(e.m_HrtfMode!=(CHRTFMODE)expectedMode||!V(e.m_vPosition,0,0,1)||!V(e.m_vVelocity,0,0,0)||!V(e.m_vConeDirection,0,0,1))return Fail(id,"HRTF and vectors");
  float gains[5]={e.m_fMinDistance,e.m_fRefDistance,e.m_fRolloff,e.m_fDoppler,e.m_fConeOuterGain};for(int k=0;k<5;++k)if(gains[k]!=1)return Fail(id,"distance and gain defaults");
  if(e.m_fMaxDistance!=FLT_MAX||e.m_uiConeInner!=360||e.m_uiConeOuter!=360||e.m_iDirect||e.m_iDirectHF||e.m_pfnDistanceCallback||e.m_DistanceCallbackData||e.m_ffCommit)return Fail(id,"scalar callback/dirty defaults");
  if(!reverb){if(memcmp(&e.m_EnvFx,&before.m_EnvFx,sizeof(e.m_EnvFx)))return Fail(id,"non-reverb environment preserved");}
  else {const _C3DSOURCEENVFX&f=e.m_EnvFx;if(f.iEnvironment||f.iEnvironmentHF||f.iObstruction||f.fObstructionLFRatio||f.iOcclusion||f.fOcclusionLFRatio||f.iExclusion||f.fExclusionLFRatio||f.fRolloffFactor!=1||f.fAirAbsorptionFactor!=1)return Fail(id,"reverb environment defaults");}
  const CODA3DSOURCE&p=e.m_3D;if(!V(p.vPosition,0,0,1)||!V(p.vVelocity,0,0,0)||!V(p.vConeDirection,0,0,1)||!V(p.vDirection,0,0,1))return Fail(id,"processed vector copies");
  float processed[5]={p.fDistance,p.fDistanceGain,p.fConeGain,p.fDoppler,p.fMacroFx};for(int k=0;k<5;++k)if(processed[k]!=1)return Fail(id,"processed gains");
  if(p.iDirect||p.iDirectHF||p.uiDirectHFReference!=hfReference||p.iEnvFx!=(reverb?0:-10000)||p.iEnvFxHF||p.uiEnvFxHFReference!=5000)return Fail(id,"processed levels/references");
 }
 if(id!=4608)return Fail(id,"coverage");
 puts("00c41060_TEST PASS");return 0;
}