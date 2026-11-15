// 00c6b050 full-body fixture. Used data/member/API projections only; opaque native vptr copied as a word.
#define FABLE_DUAL_PATH_TEST
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include "libpk/FtoL.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _MSC_VER
#include <float.h>
#endif
static int eventCount,events[8];static bool bad,createResult;static float constructorDecay;
static void Event(int n){if(eventCount>=8)bad=true;else events[eventCount++]=n;}
struct CReverbConstants {unsigned int vptrTag,m_uiNumOutputs,m_uiSamplesPerSecond,m_uiMaxReflectionsInputSamples,m_uiMaxShortReverbInputSamples,m_uiMaxLongReverbInputSamples,m_uiMaxModulationSamples;};
struct CFloatShortReverbConstants {unsigned int uiBufferLength;};
struct CFloatReverbConstants:public CReverbConstants {CFloatShortReverbConstants m_ShortRvbConstants[4];};
struct CFloatReverbParameters {unsigned int marker;};
struct CHLRvbParams {float m_fDecayTime;CHLRvbParams(){Event(3);m_fDecayTime=constructorDecay;}float GetDecayTime()const{return m_fDecayTime;}};
static CFloatReverbConstants*expectedConstants;static CFloatReverbParameters*expectedParams;
struct CFloatReverbParameterTranslator {void Create(CFloatReverbConstants*p){Event(1);if(p!=expectedConstants)bad=true;}void Translate(CFloatReverbParameters*p,CHLRvbParams*q){Event(4);if(p!=expectedParams||q->m_fDecayTime!=constructorDecay)bad=true;p->marker=0x12345678;}};
struct CReverbDspEngine {virtual~CReverbDspEngine(){}virtual void Commit(void*p,bool force){Event(5);if(p!=expectedParams||!force)bad=true;}};
struct CFloatReverbDspEngine:public CReverbDspEngine {bool Create(CFloatReverbConstants*p){Event(2);if(p!=expectedConstants)bad=true;return createResult;}};
class CFloatReverbEngine {public:CFloatReverbParameters m_RvbParams;CFloatReverbConstants m_RvbConsts;CFloatReverbParameterTranslator m_Translator;CFloatReverbDspEngine m_DspEngine;int m_iDecayTimeSamples,m_iSamplesSinceInputInvalid;bool Create(CReverbConstants*);};

bool CFloatReverbEngine::Create(CReverbConstants* pConsts)
{
    if (!pConsts->m_uiMaxReflectionsInputSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.5404f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxReflectionsInputSamples=n;
    }
    if (!pConsts->m_uiMaxShortReverbInputSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.1f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxShortReverbInputSamples=n;
    }
    if (!pConsts->m_uiMaxLongReverbInputSamples) {
        long n=FtoL((float)pConsts->m_uiSamplesPerSecond - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxLongReverbInputSamples=n;
    }
    if (!pConsts->m_uiMaxModulationSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.5f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxModulationSamples=n;
    }
    memcpy(&m_RvbConsts,pConsts,sizeof(CReverbConstants));
    static const float sc_fDefaultShortReverbBufferLength[4]={0.048649f,0.058005f,0.070295f,0.085238f};
    for(unsigned int i=0;i<4;++i) {
        long n=FtoL(m_RvbConsts.m_uiSamplesPerSecond * sc_fDefaultShortReverbBufferLength[i] - 0.5f);
        if(n<0)++n;
        m_RvbConsts.m_ShortRvbConstants[i].uiBufferLength=n;
    }
    m_Translator.Create(&m_RvbConsts);
    bool bCreated=m_DspEngine.Create(&m_RvbConsts);
    CHLRvbParams params;
    m_Translator.Translate(&m_RvbParams,&params);
    CReverbDspEngine* pEngine=&m_DspEngine;
    pEngine->Commit(&m_RvbParams,true);
    long n=FtoL(params.GetDecayTime() - 0.5f);
    if(n<0)++n;
    m_iDecayTimeSamples=m_RvbConsts.m_uiSamplesPerSecond * n;
    return bCreated;
}


// Independent mathematical reference for the legacy biased-nearest operation (not ordinary truncation at exact odd integer inputs).
static int RoundAndAdjust(float v){
    double x=(double)v;int q;
    if(!(x>=-2147483648.0&&x<2147483648.0))q=(-2147483647-1);
    else{double lo=floor(x),f=x-lo;if(f>.5||(f==.5&&fmod(lo,2.0)!=0.0))lo+=1.0;if(lo>2147483647.0)q=(-2147483647-1);else q=(int)lo;}
    if(q<0)++q;
    return q;
}
static unsigned int Ref(unsigned int rate,float factor){float biased=(float)(rate*factor-.5f);return(unsigned int)RoundAndAdjust(biased);}
static int Fail(int n,const char*s){printf("00c6b050_TEST FAIL %d %s\n",n,s);return 1;}
int main(){
#ifdef _MSC_VER
    _controlfp(_PC_53|_RC_NEAR|_MCW_EM,_MCW_PC|_MCW_RC|_MCW_EM);
#endif
    unsigned int rates[]={0,1,2,3,8000,11025,22050,44100,44101,48000,96000,192000};
    float decays[]={0,.25f,.5f,.75f,1.0f,1.25f,2.0f,3.0f,4.25f,-1.0f};
    const float factors[]={.048649f,.058005f,.070295f,.085238f};
    int id=0;
    for(unsigned int r=0;r<sizeof(rates)/sizeof(rates[0]);++r)for(unsigned int mask=0;mask<16;++mask)for(int result=0;result<2;++result)for(unsigned int d=0;d<sizeof(decays)/sizeof(decays[0]);++d,++id){
        CReverbConstants c;c.vptrTag=0x12730000+mask;c.m_uiNumOutputs=mask&3;c.m_uiSamplesPerSecond=rates[r];
        c.m_uiMaxReflectionsInputSamples=(mask&1)?0x123:0;c.m_uiMaxShortReverbInputSamples=(mask&2)?0x234:0;c.m_uiMaxLongReverbInputSamples=(mask&4)?0x345:0;c.m_uiMaxModulationSamples=(mask&8)?0x456:0;
        CReverbConstants expect=c;
        if(!(mask&1))expect.m_uiMaxReflectionsInputSamples=Ref(rates[r],.5404f);
        if(!(mask&2))expect.m_uiMaxShortReverbInputSamples=Ref(rates[r],.1f);
        if(!(mask&4))expect.m_uiMaxLongReverbInputSamples=Ref(rates[r],1.0f);
        if(!(mask&8))expect.m_uiMaxModulationSamples=Ref(rates[r],.5f);
        CFloatReverbEngine e;e.m_RvbParams.marker=0;e.m_iDecayTimeSamples=3456;e.m_iSamplesSinceInputInvalid=0x1234567;
        for(int n=0;n<4;++n)e.m_RvbConsts.m_ShortRvbConstants[n].uiBufferLength=0xaaaa;
        expectedConstants=&e.m_RvbConsts;expectedParams=&e.m_RvbParams;constructorDecay=decays[d];createResult=result!=0;eventCount=0;bad=false;
        bool actual=e.Create(&c);
        if(actual!=createResult||bad||eventCount!=5)return Fail(id,"boundary/result");
        for(int k=0;k<5;++k)if(events[k]!=k+1)return Fail(id,"boundary order");
        if(memcmp(&c,&expect,sizeof(expect))!=0)return Fail(id,"caller constants/default preservation");
        if(memcmp(static_cast<CReverbConstants*>(&e.m_RvbConsts),&expect,sizeof(expect))!=0)return Fail(id,"whole constants copy");
        for(int j=0;j<4;++j)if(e.m_RvbConsts.m_ShortRvbConstants[j].uiBufferLength!=Ref(rates[r],factors[j]))return Fail(id,"short buffer lengths");
        unsigned int expectedDecay=rates[r]*(unsigned int)RoundAndAdjust((float)(decays[d]-.5f));
        if((unsigned int)e.m_iDecayTimeSamples!=expectedDecay||e.m_iSamplesSinceInputInvalid!=0x1234567||e.m_RvbParams.marker!=0x12345678)return Fail(id,"decay and unrelated state");
    }
    if(id!=3840)return Fail(id,"coverage");
    if(RoundAndAdjust(.5f)!=0||RoundAndAdjust(1.5f)!=2)return Fail(id,"ties-to-even reference");
    puts("00c6b050_TEST PASS");return 0;
}