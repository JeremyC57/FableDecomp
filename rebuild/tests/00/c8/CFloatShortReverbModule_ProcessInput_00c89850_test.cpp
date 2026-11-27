// 00c89850 CFloatShortReverbModule::ProcessInput
// Leaf: ?ProcessInput@CFloatShortReverbModule@@QAEXPAUCFloatShortReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z
// Flags: none
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT_FADING
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include <string.h>
#define private public
#define protected public
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatShortReverbModuleParams.h"
#include "engine/CFloatShortReverbModule.h"
#undef private
#undef protected


void CFloatShortReverbModule::ProcessInput(CFloatShortReverbModuleParams* pParamsFrom, const CFloatShortReverbModuleParams* pParamsTo,
    const CFloatCyclicBuffer* pInputBuffer, float* pfInternalBuffer, float* pfOutputBuffer,
    unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    float* pfBase = m_Buffer.m_pBase;
    unsigned int iRdFeedbackCursor = m_Buffer.GetWriteCursor();
    unsigned int iWrInputCursor = pInputBuffer->GetWriteCursor() >= uiSamples
        ? pInputBuffer->GetWriteCursor() - uiSamples : pInputBuffer->GetWriteCursor() - uiSamples + pInputBuffer->GetLength();
    float fFadeOut = (float)uiSamplesToFade / 512.0f;
    float fFadeIn = 1.0f - fFadeOut;
    float fFadeOutFinal = ((float)uiSamplesToFade - (float)uiSamples) / 512.0f;
    float fFadeInFinal = 1.0f - fFadeOutFinal;
    memset(pfInternalBuffer, 0, uiSamples * sizeof(float));
    unsigned int uiSamplesToFilter = uiSamples;
    if (iRdFeedbackCursor + uiSamples > m_Buffer.GetLength())
        uiSamplesToFilter = m_Buffer.GetLength() - iRdFeedbackCursor;
    m_Filter.Process(&pParamsFrom->m_fFilter, &pParamsTo->m_fFilter,
        pfBase + iRdFeedbackCursor, pfInternalBuffer, true, uiSamplesToFilter, uiSamplesToFade);
    if (uiSamplesToFilter < uiSamples)
        m_Filter.Process(&pParamsFrom->m_fFilter, &pParamsTo->m_fFilter,
            pfBase, pfInternalBuffer + uiSamplesToFilter, true, uiSamples - uiSamplesToFilter, uiSamplesToFade);
    for (unsigned int iInput = 0; iInput < 2; ++iInput) {
        float fScale = pParamsFrom->m_afInputGain[iInput];
        unsigned int iRdInputCursor = iWrInputCursor >= pParamsFrom->m_auiInputDelay[iInput]
            ? iWrInputCursor - pParamsFrom->m_auiInputDelay[iInput] : iWrInputCursor - pParamsFrom->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
        m_pDSP->ScaleAddSamples(pfInternalBuffer,pInputBuffer,iRdInputCursor,
            fScale * fFadeOut, (fFadeOutFinal - fFadeOut) * fScale / (float)uiSamples,uiSamples);
        fScale = pParamsTo->m_afInputGain[iInput];
        iRdInputCursor = iWrInputCursor >= pParamsTo->m_auiInputDelay[iInput]
            ? iWrInputCursor - pParamsTo->m_auiInputDelay[iInput] : iWrInputCursor - pParamsTo->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
        m_pDSP->ScaleAddSamples(pfInternalBuffer,pInputBuffer,iRdInputCursor,
            fScale * fFadeIn, (fFadeInFinal - fFadeIn) * fScale / (float)uiSamples,uiSamples);
    }
    float fScaleStart = fFadeOut * pParamsFrom->m_fOutputGain + fFadeIn * pParamsTo->m_fOutputGain;
    float fScaleFinish = fFadeOutFinal * pParamsFrom->m_fOutputGain + fFadeInFinal * pParamsTo->m_fOutputGain;
    m_pDSP->ScaleAddSamples(pfOutputBuffer,pfInternalBuffer,fScaleStart,(fScaleFinish-fScaleStart)/(float)uiSamples,uiSamples);
    if (uiSamples >= uiSamplesToFade) *pParamsFrom = *pParamsTo;
}

#include <assert.h>
#include <stdio.h>
static unsigned filterCalls, ringCalls, finalCalls, fading;
static CFloatShortReverbModuleParams *oldRecord, *newRecord;
static CReverbFns dsp;
CReverbFns* CReverbDspProcessor::m_pDSP=&dsp;
CReverbDspProcessor::CReverbDspProcessor(){}
CReverbDspProcessor::~CReverbDspProcessor(){}
CFloatShortReverbModule::~CFloatShortReverbModule(){}
CFloatFilter::CFloatFilter(){}
CFloatFilter::~CFloatFilter(){}
void CFloatFilter::Process(CFloatFilterParams* a,const CFloatFilterParams* b,const float* src,float* dst,bool add,unsigned n,unsigned fade) {
    assert(a==&oldRecord->m_fFilter && b==&newRecord->m_fFilter && add && fade==fading);
    ++filterCalls; for(unsigned i=0;i<n;++i){assert(dst[i]==0);dst[i]=src[i];}
}
CReverbFns::eReverbType CReverbFns::Type(){return eReverb_Float;}
void CReverbFns::ScaleSamples(float*,float,unsigned){assert(false);}
void CReverbFns::ScaleSamples(float*,const float*,float,unsigned){assert(false);}
void CReverbFns::ScaleSamples(float*,float,float,unsigned){assert(false);}
void CReverbFns::ScaleSamples(float*,const float*,float,float,unsigned){assert(false);}
void CReverbFns::AddSamples(float*,const float*,unsigned){assert(false);}
void CReverbFns::ScaleAddSamples(float*,const float*,float,unsigned){assert(false);}
void CReverbFns::ApplyFilterOrder2(float*,const float*,const float*,const float*,float*,unsigned){assert(false);}
void CReverbFns::ScaleAddSamples(float* dst,const float* src,float gain,float change,unsigned n) {
    ++finalCalls;for(unsigned j=0;j<n;++j)dst[j]+=src[j]*(gain+j*change);
}
void CReverbFns::ScaleAddSamples(float* dst,const CFloatCyclicBuffer* buf,unsigned cursor,float gain,float change,unsigned n) {
    ++ringCalls;assert(cursor<buf->GetLength());
    for(unsigned j=0;j<n;++j)dst[j]+=buf->m_pBase[(cursor+j)%buf->GetLength()]*(gain+j*change);
}
int main(){
 float history[]={2,1,-1,4}, feedback[]={1,4,-4,8};
 const unsigned spans[]={1,2,4}, fades[]={1,2,4,128,512,0x80000000U};
 CFloatShortReverbModule module;
 CFloatCyclicBuffer input;
 module.m_Buffer.m_pBase=feedback;module.m_Buffer.m_uiLength=4;
 input.m_pBase=history;input.m_uiLength=4;
 for(unsigned pos=0;pos<4;++pos)for(unsigned ci=0;ci<4;++ci)
 for(unsigned is=0;is<3;++is)for(unsigned fs=0;fs<6;++fs){
    const unsigned n=spans[is];fading=fades[fs]; module.m_Buffer.m_uiWriteCursor=pos;input.m_uiWriteCursor=ci;
    CFloatShortReverbModuleParams a,b;memset(&a,0,sizeof a);memset(&b,0,sizeof b);
    a.m_auiInputDelay[0]=0;a.m_auiInputDelay[1]=3;b.m_auiInputDelay[0]=2;b.m_auiInputDelay[1]=1;
    a.m_afInputGain[0]=2;a.m_afInputGain[1]=1;b.m_afInputGain[0]=-1;b.m_afInputGain[1]=2;
    a.m_fOutputGain=1;b.m_fOutputGain=2;
    CFloatShortReverbModuleParams orig=a;oldRecord=&a;newRecord=&b;
    float scratch[]={9,9,9,9,77},output[]={2,2,2,2,77};
    filterCalls=ringCalls=finalCalls=0;
    module.ProcessInput(&a,&b,&input,scratch,output,n,fading);
    assert(filterCalls==(pos+n>4?2U:1U) && ringCalls==4 && finalCalls==1);
    for(unsigned j=0;j<n;++j){
        const unsigned idx=(ci+4-n+j)%4;
        const float f=static_cast<float>(fading)/512.0f;
        const float end=(static_cast<float>(fading)-static_cast<float>(n))/512.0f;
        const float u=f+(end-f)*j/n, v=(1-f)+((1-end)-(1-f))*j/n;
        float val=feedback[(pos+j)%4];
        for(unsigned t=0;t<2;++t){
            val+=history[(idx+4-orig.m_auiInputDelay[t])%4]*(orig.m_afInputGain[t]*u);
            val+=history[(idx+4-b.m_auiInputDelay[t])%4]*(b.m_afInputGain[t]*v);
        }
        assert(scratch[j]==val);
        const float first=orig.m_fOutputGain*f+b.m_fOutputGain*(1-f);
        const float last=orig.m_fOutputGain*end+b.m_fOutputGain*(1-end);
        const float expectedOutput=2+val*(first+(last-first)*j/n);
        if(output[j]!=expectedOutput){printf("case %u %u %u %u %u %.9g %.9g\n", pos,ci,n,fading,j,output[j],expectedOutput);return 2;}
    }
    assert(scratch[4]==77 && output[4]==77 && memcmp(&a,n>=fading?&b:&orig,sizeof a)==0);
 }
 puts("00c89850_TEST PASS");
}