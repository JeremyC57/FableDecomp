// CFloatFilterParams::Set_ShelfLfHf(float,float,float,float,unsigned int) -- derive cascaded low/high shelf coefficients.
// Leaf: ?Set_ShelfLfHf@CFloatFilterParams@@QAEXMMMMI@Z
// Flags: (none)
// Evidence: GameCODA2.10 release FloatFilter.obj identity; debug CodeView exact parameters and scoped locals.
#include <math.h>
#pragma intrinsic(exp,sqrt)
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#include "engine/CFloatFilterParams.h"
const float _gc_fPI=3.14159274f;
void CFloatFilterParams::Set_ShelfLfHf(float fLFRatio,float fFreqLF,float fHFRatio,float fFreqHF,unsigned int uiSamplesPerSecond)
{
    if(fLFRatio>50.0f)fLFRatio=50.0f;
    if(fHFRatio>50.0f)fHFRatio=50.0f;
    if(fLFRatio<0.00001)fLFRatio=0.00001f;
    if(fHFRatio<0.00001)fHFRatio=0.00001f;
    float fLF_A1,fLF_B1,fHF_A1,fHF_B1;
    float norm=1.0f;
    float fFreqRef=sqrtf(fFreqLF*fFreqHF);
    if(fLFRatio==1.0f) {
        float fFreqCentre=sqrtf(fFreqRef*fFreqLF);
        float expf1=expf(-_gc_fPI*fFreqCentre/uiSamplesPerSecond);
        fLF_B1=fLF_A1=expf1;
    } else {
        float fSqrtGain=sqrtf(fLFRatio);
        float fFreqCentre=sqrtf(fFreqRef*fFreqLF);
        float fFilterF1=fFreqCentre/fSqrtGain;
        float fFilterF2=fFreqCentre*fSqrtGain;
        float expf1=expf(-_gc_fPI*fFilterF1/uiSamplesPerSecond);
        float expf2=expf(-_gc_fPI*fFilterF2/uiSamplesPerSecond);
        fLF_A1=expf1;
        fLF_B1=expf2;
    }
    if(fHFRatio==1.0f) {
        float fFreqCentre=sqrtf(fFreqRef*fFreqHF);
        float expf1=expf(-_gc_fPI*fFreqCentre/uiSamplesPerSecond);
        fHF_B1=fHF_A1=expf1;
    } else {
        float fSqrtGain=sqrtf(fHFRatio);
        float fFreqCentre=sqrtf(fFreqRef*fFreqHF);
        float fFilterF1=fFreqCentre*fSqrtGain;
        float fFilterF2=fFreqCentre/fSqrtGain;
        float expf1=expf(-_gc_fPI*fFilterF1/uiSamplesPerSecond);
        float expf2=expf(-_gc_fPI*fFilterF2/uiSamplesPerSecond);
        norm*=-(1.0f-expf1)/(expf2-1.0f);
        fHF_A1=expf1;
        fHF_B1=expf2;
    }
    m_fA[0]=1.0f;
    m_fA[1]=fLF_A1+fHF_A1;
    m_fA[2]=-fLF_A1*fHF_A1;
    m_fB[0]=1.0f*norm;
    m_fB[1]=-(fLF_B1+fHF_B1)*norm;
    m_fB[2]=fLF_B1*fHF_B1*norm;
}

#include <cassert>
#include <cstdio>
#include <cstring>
static void Coeff(CFloatFilterParams& p,float* c){float all[7];std::memcpy(all,&p,sizeof(all));for(int i=0;i<6;++i)c[i]=all[i+1];}
static bool Near(float a,float b,float epsilon=0.00005f){return fabs(a-b)<=epsilon*(1.0f+fabs(b));}
static void Evaluate(float lf,float hf,float rateScale,float* out){CFloatFilterParams p;p.SetGain(0.75f);p.Set_ShelfLfHf(lf,1000.0f*rateScale,hf,8000.0f*rateScale,(unsigned int)(48000.0f*rateScale));Coeff(p,out);float gain;std::memcpy(&gain,&p,sizeof(gain));assert(gain==0.75f);assert(out[0]==1.0f);for(int i=0;i<6;++i)assert(out[i]==out[i]);}
int main(){
    float a[6],b[6];
    Evaluate(1.0f,1.0f,1.0f,a);assert(a[3]==1.0f&&Near(a[4],-a[1])&&Near(a[5],-a[2]));
    Evaluate(2.0f,3.0f,1.0f,a);Evaluate(2.0f,3.0f,2.0f,b);for(int i=0;i<6;++i)assert(Near(a[i],b[i]));assert(a[1]>0.1f&&a[2]<0.0f&&a[3]>0.0f);
    Evaluate(1.0f,3.0f,1.0f,a);assert(Near(a[3]+a[4]+a[5],1.0f-a[1]-a[2]));
    Evaluate(2.0f,1.0f,1.0f,a);assert(a[3]==1.0f&&a[4]<0.0f&&a[5]>0.0f);
    const float hi[2]={51.0f,1000.0f};for(int j=0;j<2;++j){Evaluate(hi[j],2.0f,1.0f,a);Evaluate(50.0f,2.0f,1.0f,b);for(int i=0;i<6;++i)assert(Near(a[i],b[i]));Evaluate(2.0f,hi[j],1.0f,a);Evaluate(2.0f,50.0f,1.0f,b);for(int i=0;i<6;++i)assert(Near(a[i],b[i]));}
    const float lo[2]={0.0f,-3.0f};for(int j=0;j<2;++j){Evaluate(lo[j],2.0f,1.0f,a);Evaluate(0.00001f,2.0f,1.0f,b);for(int i=0;i<6;++i)assert(Near(a[i],b[i]));Evaluate(2.0f,lo[j],1.0f,a);Evaluate(2.0f,0.00001f,1.0f,b);for(int i=0;i<6;++i)assert(Near(a[i],b[i]));}
    Evaluate(50.0f,50.0f,1.0f,a);Evaluate(0.00001f,0.00001f,1.0f,a);
    std::puts("00c74850_TEST PASS");return 0;
}