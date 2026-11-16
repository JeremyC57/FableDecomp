// Self-contained test of the real shared classes and the pasted production source.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#define __declspec(x)
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define private public
#define protected public
// 012152aa CSalPlatform::CreateSw parent00c3f9d0
// Leaf: ?CreateSw@CSalPlatform@@AAE_NPBU_CRESOURCEDESC@@PBUCPLATFORMDESC@@@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#include "engine/CSalPlatform.h"
#include "engine/CSalCpuInfo.h"
#include "engine/CSalMixer.h"
#include "engine/CSalOutputMM.h"
#include "engine/CSalOutputNull.h"
#include "engine/CDSoundAccelerationQuery.h"
#include "engine/CodaCaps.h"
#include "engine/CSalHrtf.h"
bool CSalPlatform::CreateSw(const CRESOURCEDESC* resources,const CPLATFORMDESC* params)
{
    CSalCpuInfo cpu;
    if (!CSalCpuInfo::HasMMX())
        return false;
    if (CSalCpuInfo::HasSSE2())
        m_pMixer = new CSalMixerLimiterSSE2;
    else
        m_pMixer = new CSalMixerLimiterMMX;
    if (!m_pMixer)
        return false;
    switch (params->SWOutputDevice) {
    case 1:
        m_pOutputStream = new CSalOutputNull;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    case 4:
        m_pOutputStream = new CSalOutputKS;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    case 2:
        if (CreateSwDirectSound(params))
            goto ready;
        return false;
    case 5:
        if (CreateSwBufferedDirectSound(params))
            goto ready;
        return false;
    case 0: {
        CDSoundAccelerationQuery devices;
        if (devices.GetAccelerationLevel(params->pGuid) > 0 && CreateSwDirectSound(params))
            goto ready;
    }
    // Fall through.
    case 3:
        m_pOutputStream = new CSalOutputMM;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    default:
        return false;
    }
ready:
    bool hasMixOut = m_pHw != 0;
    int noSecond = m_pReverbOutputStream == 0 ? 1 : 0;
    if (noSecond & static_cast<int>(hasMixOut)) {
        g_SalCaps.SW2DVoice.ffCaps &= ~0x80u;
        g_SalCaps.SW3DVoice.ffCaps &= ~0x80u;
        g_SalCaps.SW2DVoice.ffCaps &= ~0x2000u;
        g_SalCaps.SW3DVoice.ffCaps &= ~0x2000u;
    }
    unsigned int blockSize = m_pOutputStream->GetMinBufferSamples();
    g_SalCaps.SWMixer.nMinFrameSamples = blockSize;
    g_SalCaps.SWMixer.nFrameSamples = blockSize;
    if (g_HRTFMode == HRTF_NONE)
        g_SalCaps.SW3DVoice.ffCaps &= ~0x300000u;
    unsigned int start = params->nSW2DVoices;
    g_SalCaps.SW2DVoice.nVoices = start;
    g_SalCaps.SW2DVoice.nFreeVoices = params->nSW2DVoices;
    unsigned int length = params->nSW3DVoices;
    g_SalCaps.SW3DVoice.nVoices = length;
    g_SalCaps.SW3DVoice.nFreeVoices = params->nSW3DVoices;
    int end = (start != (unsigned int)-1 && length != (unsigned int)-1) ? (int)(start + length) : -1;
    g_SalCaps.nSWAllVoices = end;
    g_SalCaps.nSWFreeVoices = end;
    unsigned int size = params->nSWMixFrameSamples;
    if (size > blockSize) {
        if (size & 0x7f)
            size = (size / 128 + 1) * 128;
        g_SalCaps.SWMixer.nFrameSamples = size;
    }
    if (!CreateSwVoices())
        return false;
    m_bXtcProcessing = false;
    m_nSw2DVoicesPlaying = 0;
    m_nSw3DVoicesPlaying = 0;
    if (!SetSWOutputConfig()) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        delete m_pReverbOutputStream;
        m_pReverbOutputStream = 0;
        return false;
    }
    return true;
}

#undef private
#undef protected
static int case_number;
static void check(bool ok){if(!ok){fprintf(stderr,"mixer case %d failed\n",case_number);exit(1);}}
_CCAPS g_SalCaps;
extern "C" {HrtfMode g_HRTFMode=HRTF_NONE;}
bool CSalCpuInfo::m_bInitialized=false;
bool CSalCpuInfo::m_bHasMMX=false;
bool CSalCpuInfo::m_bHasSSE2=false;
static int cpu_init,alloc_calls,fail_alloc,create_calls,voice_calls,config_calls,helper_calls,probe_calls;
static int constructed_kind,output_dtors,mixer_dtors,limiter_live,throw_kind;
static int helper_depth,block_count;
static void* blocks[16];
static bool create_ok,voice_ok,config_ok,helper_ok;
static int acceleration;
static unsigned int minimum_samples;
static const CPLATFORMDESC* expected_params;
static CSalPlatform* active;
static unsigned int expected_flags2,expected_flags3;
void CSalCpuInfo::Initialize(){++cpu_init;m_bInitialized=true;}
extern "C" void* __cdecl CodaMalloc(unsigned int size){
 if(!helper_depth){++alloc_calls;if(fail_alloc==alloc_calls)return 0;}
 check(block_count<16);void* p=malloc(size);check(p!=0);blocks[block_count++]=p;return p;
}
extern "C" void __cdecl CodaFree(void* p){if(!p)return;int i;for(i=0;i<block_count;++i)if(blocks[i]==p)break;check(i<block_count);blocks[i]=0;free(p);}
CCodaObjectBase::~CCodaObjectBase(){}
CCodaObject::~CCodaObject(){}
CSalMixer::CSalMixer(){}
CSalMixer::~CSalMixer(){}
CSalLimiter::CSalLimiter(){++limiter_live;}
CSalLimiter::~CSalLimiter(){--limiter_live;}
CSalMixerLimiter::CSalMixerLimiter(){
#ifndef _MSC_VER
 if(throw_kind==1)throw 31;
#endif
}
CSalMixerLimiter::~CSalMixerLimiter(){++mixer_dtors;}
static void output_construct(int kind){
 if(!helper_depth)constructed_kind=kind;
#ifndef _MSC_VER
 if(!helper_depth&&throw_kind==kind)throw 31;
#endif
}
CSalOutput::~CSalOutput(){}
CSalFile::~CSalFile(){}
CSalKsPin::~CSalKsPin(){}
CSalEvent::~CSalEvent(){}
CSalThread::~CSalThread(){}
CSalMutex::~CSalMutex(){}
CSalOutputNull::CSalOutputNull(){output_construct(2);}
CSalOutputKS::CSalOutputKS(){output_construct(3);}
CSalOutputMM::CSalOutputMM(){output_construct(4);}
CSalOutputNull::~CSalOutputNull(){++output_dtors;}
CSalOutputKS::~CSalOutputKS(){++output_dtors;}
CSalOutputMM::~CSalOutputMM(){++output_dtors;}
static bool output_create(const GUID* guid,HWND window,bool mode){++create_calls;check(guid==expected_params->pGuid&&window==(HWND)expected_params->hWnd&&!mode);return create_ok;}
bool CSalOutputNull::Create(const GUID* g,HWND w,bool b){return output_create(g,w,b);}
bool CSalOutputKS::Create(const GUID* g,HWND w,bool b){return output_create(g,w,b);}
bool CSalOutputMM::Create(const GUID* g,HWND w,bool b){return output_create(g,w,b);}
unsigned int CSalOutputNull::GetMinBufferSamples()const{return minimum_samples;}
unsigned int CSalOutputKS::GetMinBufferSamples()const{return minimum_samples;}
unsigned int CSalOutputMM::GetMinBufferSamples()const{return minimum_samples;}
CDSoundAccelerationQuery::CDSoundAccelerationQuery(){}
int CDSoundAccelerationQuery::GetAccelerationLevel(const _GUID* p){++probe_calls;check(p==expected_params->pGuid);return acceleration;}
static bool create_helper(CSalPlatform* platform,const CPLATFORMDESC* params){
 ++helper_calls;check(platform==active&&params==expected_params);
 if(!helper_ok)return false;
 ++helper_depth;platform->m_pOutputStream=new CSalOutputNull;--helper_depth;check(platform->m_pOutputStream!=0);return true;
}
bool CSalPlatform::CreateSwDirectSound(const CPLATFORMDESC* p){return create_helper(this,p);}
bool CSalPlatform::CreateSwBufferedDirectSound(const CPLATFORMDESC* p){return create_helper(this,p);}
bool CSalPlatform::CreateSwVoices(){
 ++voice_calls;check(this==active&&g_SalCaps.SW2DVoice.ffCaps==expected_flags2&&g_SalCaps.SW3DVoice.ffCaps==expected_flags3);
 check(g_SalCaps.SW2DVoice.nVoices==expected_params->nSW2DVoices&&g_SalCaps.SW3DVoice.nVoices==expected_params->nSW3DVoices);
 check(g_SalCaps.SW2DVoice.nFreeVoices==expected_params->nSW2DVoices&&g_SalCaps.SW3DVoice.nFreeVoices==expected_params->nSW3DVoices);
 int sum=(expected_params->nSW2DVoices==-1||expected_params->nSW3DVoices==-1)?-1:expected_params->nSW2DVoices+expected_params->nSW3DVoices;
 check(g_SalCaps.nSWAllVoices==sum&&g_SalCaps.nSWFreeVoices==sum);
 unsigned int wanted=expected_params->nSWMixFrameSamples;
 if(wanted<=minimum_samples)wanted=minimum_samples;else wanted=((wanted+127)/128)*128;
 check(g_SalCaps.SWMixer.nMinFrameSamples==minimum_samples&&g_SalCaps.SWMixer.nFrameSamples==wanted);
 check(g_SalCaps.HW2DVoice.nVoices==117&&g_SalCaps.HW3DVoice.nVoices==119);
 return voice_ok;
}
bool CSalPlatform::SetSWOutputConfig(){++config_calls;check(this==active&&!m_bXtcProcessing&&m_nSw2DVoicesPlaying==0&&m_nSw3DVoicesPlaying==0);return config_ok;}
static void reset(){cpu_init=alloc_calls=create_calls=voice_calls=config_calls=helper_calls=probe_calls=0;constructed_kind=output_dtors=mixer_dtors=limiter_live=helper_depth=block_count=0;throw_kind=0;memset(blocks,0,sizeof(blocks));}
static void init(CSalPlatform& p){p.m_pMixer=0;p.m_pOutputStream=0;p.m_pReverbOutputStream=0;p.m_pHw=0;p.m_bXtcProcessing=true;p.m_nSw2DVoicesPlaying=61;p.m_nSw3DVoicesPlaying=62;active=&p;}
static void release(CSalPlatform& p){delete p.m_pOutputStream;delete p.m_pReverbOutputStream;delete p.m_pMixer;p.m_pOutputStream=p.m_pReverbOutputStream=0;p.m_pMixer=0;check(limiter_live==0);for(int i=0;i<block_count;++i)check(blocks[i]==0);}
int main(){
 const int modes[8]={-1,0,1,2,3,4,5,6};const int counts2[3]={-1,0,2},counts3[3]={3,-1,4};const unsigned int sizes[3]={64,128,129},mins[3]={128,64,32};
 const HrtfMode hrtfs[3]={HRTF_NONE,HRTF_FULL,HRTF_HORIZONTAL_HQ};const int accelerations[3]={0,1,3};
 for(int cpu=0;cpu<3;++cpu)for(int initialized=0;initialized<2;++initialized)for(int mode=0;mode<8;++mode)for(int failure=0;failure<7;++failure)for(int hw=0;hw<2;++hw)for(int rev=0;rev<2;++rev)for(int shape=0;shape<3;++shape){
  ++case_number;reset();CSalPlatform platform;init(platform);CPLATFORMDESC params;memset(&params,0,sizeof(params));_GUID guid;memset(&guid,0,sizeof(guid));params.pGuid=shape?&guid:0;params.hWnd=(void*)&params;params.SWOutputDevice=(COUTPUTDEVICE)modes[mode];params.nSW2DVoices=counts2[shape];params.nSW3DVoices=counts3[shape];params.nSWMixFrameSamples=sizes[shape];expected_params=&params;
  CSalCpuInfo::m_bInitialized=initialized!=0;CSalCpuInfo::m_bHasMMX=cpu>0;CSalCpuInfo::m_bHasSSE2=cpu==2;
  fail_alloc=failure==1?1:failure==2?2:0;create_ok=failure!=3;voice_ok=failure!=4;config_ok=failure!=5;helper_ok=failure!=6;minimum_samples=mins[shape];acceleration=accelerations[shape];g_HRTFMode=hrtfs[shape];
  if(hw)platform.m_pHw=reinterpret_cast<CSalHw*>(&params);
  if(rev){++helper_depth;platform.m_pReverbOutputStream=new CSalOutputNull;--helper_depth;}
  memset(&g_SalCaps,0,sizeof(g_SalCaps));g_SalCaps.SW2DVoice.ffCaps=g_SalCaps.SW3DVoice.ffCaps=0xffffffu;g_SalCaps.HW2DVoice.nVoices=117;g_SalCaps.HW3DVoice.nVoices=119;expected_flags2=0xffffffu;if(hw&&!rev)expected_flags2&=~0x2080u;expected_flags3=expected_flags2;if(g_HRTFMode==HRTF_NONE)expected_flags3&=~0x300000u;
  bool reached=cpu>0&&fail_alloc!=1;bool valid=modes[mode]>=0&&modes[mode]<=5;
  bool helper=reached&&valid&&(modes[mode]==2||modes[mode]==5||(modes[mode]==0&&acceleration>0));
  bool usehelper=helper&&helper_ok;bool direct=reached&&valid&&!usehelper&&(modes[mode]!=2&&modes[mode]!=5);
  bool output_ready=(usehelper||(direct&&fail_alloc!=2&&create_ok));bool expected=output_ready&&voice_ok&&config_ok;
  bool result=platform.CreateSw(0,&params);check(result==expected&&cpu_init==(!initialized));
  check(probe_calls==(reached&&modes[mode]==0?1:0)&&helper_calls==(helper?1:0));
  check(create_calls==(direct&&fail_alloc!=2?1:0)&&voice_calls==(output_ready?1:0)&&config_calls==(output_ready&&voice_ok?1:0));
  check(alloc_calls==(cpu==0?0:1+(direct?1:0)));
  if(direct&&fail_alloc!=2)check(constructed_kind==(modes[mode]==1?2:modes[mode]==4?3:4));
  if(output_ready&&voice_ok){check(!platform.m_bXtcProcessing&&platform.m_nSw2DVoicesPlaying==0&&platform.m_nSw3DVoicesPlaying==0);if(!config_ok)check(platform.m_pOutputStream==0&&platform.m_pReverbOutputStream==0);}
  else check(platform.m_bXtcProcessing&&platform.m_nSw2DVoicesPlaying==61&&platform.m_nSw3DVoicesPlaying==62);
  if(!output_ready)check(g_SalCaps.SW2DVoice.ffCaps==0xffffffu&&g_SalCaps.SW3DVoice.ffCaps==0xffffffu);
  release(platform);
 }
 check(case_number==4032);
#ifndef _MSC_VER
 for(int kind=0;kind<=4;++kind){reset();CSalPlatform platform;init(platform);CPLATFORMDESC params;memset(&params,0,sizeof(params));params.SWOutputDevice=(COUTPUTDEVICE)(kind==3?4:kind==4?3:1);expected_params=&params;CSalCpuInfo::m_bInitialized=true;CSalCpuInfo::m_bHasMMX=true;CSalCpuInfo::m_bHasSSE2=kind==0;fail_alloc=0;throw_kind=kind==0?1:kind;bool caught=false;try{platform.CreateSw(0,&params);}catch(int n){caught=n==31;}check(caught);release(platform);}
#endif
 puts("012152aa_TEST PASS");return 0;
}

bool CSalOutput::CreateReverbBuffer(unsigned int){abort();}
void CSalOutputNull::Release(){abort();}
bool CSalOutputNull::CreateBuffer(unsigned int,unsigned int){abort();}
void CSalOutputNull::ReleaseBuffer(){abort();}
bool CSalOutputNull::FrameIsReady(){abort();}
void* CSalOutputNull::FrameLock(){abort();}
void CSalOutputNull::FrameUnlock(void*){abort();}
void CSalOutputMM::Release(){abort();}
bool CSalOutputMM::CreateBuffer(unsigned int,unsigned int){abort();}
void CSalOutputMM::ReleaseBuffer(){abort();}
bool CSalOutputMM::FrameIsReady(){abort();}
void* CSalOutputMM::FrameLock(){abort();}
void CSalOutputMM::FrameUnlock(void*){abort();}
void CSalOutputKS::Release(){abort();}
bool CSalOutputKS::CreateBuffer(unsigned int,unsigned int){abort();}
void CSalOutputKS::ReleaseBuffer(){abort();}
bool CSalOutputKS::FrameIsReady(){abort();}
void* CSalOutputKS::FrameLock(){abort();}
void CSalOutputKS::FrameUnlock(void*){abort();}
void CSalMixer::Reset(){abort();}
void CSalMixer::MixFrameClear(){abort();}
void CSalMixer::MixFrame(const float*,int*){abort();}
void CSalMixer::MixFrame(const float*,int*,unsigned short){abort();}
void CSalMixer::MixFrame(const float*,int*,float){abort();}
void CSalMixer::MixFrame(const int*,int*){abort();}
void CSalMixer::MixFrame(const int*,int*,unsigned short){abort();}
void CSalMixer::MixFrame(const short*,int*,unsigned short){abort();}
void CSalMixer::MixFrameFade(const short*,int*,unsigned short,short){abort();}
void CSalMixer::GetFrame(short*){abort();}
void CSalMixerLimiter::Reset(){abort();}
void CSalMixerLimiter::GetFrame(short*){abort();}
void CSalMixerLimiterMMX::MixFrameClear(){abort();}
void CSalMixerLimiterMMX::MixFrame(const short*,int*,unsigned short){abort();}
void CSalMixerLimiterMMX::MixFrameFade(const short*,int*,unsigned short,short){abort();}
void CSalMixerLimiterSSE2::MixFrame(const float*,int*){abort();}
void CSalMixerLimiterSSE2::MixFrame(const int*,int*){abort();}
void CSalMixerLimiterSSE2::MixFrame(const short*,int*,unsigned short){abort();}
void CSalMixerLimiterSSE2::MixFrameFade(const short*,int*,unsigned short,short){abort();}
bool CCodaObject::IsBuffer()const{abort();}
bool CCodaObject::IsContext()const{abort();}
bool CCodaObject::IsCreated()const{abort();}
bool CCodaObject::IsEnvironment()const{abort();}
bool CCodaObject::IsEAX()const{abort();}
bool CCodaObject::IsListener()const{abort();}
bool CCodaObject::IsSource()const{abort();}
bool CCodaObject::IsReverb()const{abort();}
bool CCodaObject::IsVoice()const{abort();}