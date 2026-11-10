// Exact selected target and pure C++ rounding. The separate fade operation is a controlled boundary.
#include <math.h>
#include <stdio.h>
#include <assert.h>
struct _CSWMIXERCAPS{unsigned int nSamplesPerSec,nMinFrameSamples,nFrameStepSamples,nFrameSamples,ffOutputFormat;};
struct _CCAPS{_CSWMIXERCAPS SWMixer;};
_CCAPS g_SalCaps;
static int fade_calls;static unsigned int fade_count;
class CSalSwFilter {
public:
 struct PARAMS {
 float fF;
 float fQ;
 float fG;
 };
 PARAMS m_Params;
 PARAMS m_NewParams;
 float m_fZ0;
 float m_fZ1;
 unsigned int m_nFadeSamples;
 void ProcessEQFade(short*,unsigned int);
public:
 void ProcessEQ(short*,unsigned int);
};

void CSalSwFilter::ProcessEQFade(short* buffer,unsigned int count){++fade_calls;fade_count=count;for(unsigned int k=0;k<count;++k)buffer[k]=-123;}
static inline short clip16(int value){if(value>32767)return 32767;else if(value<-32768)return -32768;return (short)value;}
static inline int sal_ftoi(float value){float x=value-.5f;int result=(int)x;float fraction=x-(float)result;if(fraction>.5f || (fraction==.5f && (result&1)))++result;else if(fraction<-.5f || (fraction==-.5f && (result&1)))--result;return result<0?result+1:result;}
void CSalSwFilter::ProcessEQ(short* pBuffer,unsigned int nSamples){
 unsigned int i=0;
 if(m_Params.fF!=m_NewParams.fF || m_Params.fG!=m_NewParams.fG || m_Params.fQ!=m_NewParams.fQ){
  unsigned int nFade=g_SalCaps.SWMixer.nFrameStepSamples-m_nFadeSamples;
  if(nFade>nSamples)nFade=nSamples;
  ProcessEQFade(pBuffer,nFade);m_nFadeSamples+=nFade;i=nFade;
  if(m_nFadeSamples==g_SalCaps.SWMixer.nFrameStepSamples){m_Params=m_NewParams;m_nFadeSamples=0;}
 }
 for(;i<nSamples;++i){
  float input=(float)pBuffer[i];
  float high=(input-m_fZ1)-m_fZ0*m_Params.fQ;
  m_fZ0=high*m_Params.fF+m_fZ0;
  m_fZ1=m_Params.fF*m_fZ0+m_fZ1;
  pBuffer[i]=clip16(sal_ftoi(high*m_Params.fQ*m_Params.fG+input));
 }
}

static int reference_round(float value){double x=(double)(float)(value-.5f);double low=floor(x);double rest=x-low;if(rest>.5 || (rest==.5 && fmod(low,2)!=0))low+=1;int result=(int)low;return result<0?result+1:result;}
static short reference_sample(CSalSwFilter& filter,short sample){float input=(float)sample;float high=(input-filter.m_fZ1)-filter.m_fZ0*filter.m_Params.fQ;filter.m_fZ0=high*filter.m_Params.fF+filter.m_fZ0;filter.m_fZ1=filter.m_Params.fF*filter.m_fZ0+filter.m_fZ1;int result=reference_round(high*filter.m_Params.fQ*filter.m_Params.fG+input);if(result>32767)result=32767;if(result<-32768)result=-32768;return(short)result;}
int main(){
 for(int k=-4096;k<=4096;++k){float value=k*.125f;assert(sal_ftoi(value)==reference_round(value));}
 const float coefficients[]={0,.25f,.5f,1};int cases=0;
 for(int f=0;f<4;++f)for(int q=0;q<4;++q)for(int gain=0;gain<4;++gain)for(unsigned int count=0;count<=4;++count){
  CSalSwFilter actual;actual.m_Params.fF=coefficients[f];actual.m_Params.fQ=coefficients[q];actual.m_Params.fG=coefficients[gain];actual.m_NewParams=actual.m_Params;actual.m_fZ0=.25f;actual.m_fZ1=-.5f;actual.m_nFadeSamples=0;CSalSwFilter reference=actual;
  short buffer[]={32767,-32768,7,-9,1234};short expected[5];for(unsigned int j=0;j<5;++j)expected[j]=j<count?reference_sample(reference,buffer[j]):buffer[j];
  fade_calls=0;actual.ProcessEQ(buffer,count);assert(!fade_calls);for(int j=0;j<5;++j)assert(buffer[j]==expected[j]);assert(actual.m_fZ0==reference.m_fZ0&&actual.m_fZ1==reference.m_fZ1);++cases;
 }
 g_SalCaps.SWMixer.nFrameStepSamples=4;
 for(unsigned int start=0;start<4;++start)for(unsigned int count=0;count<=6;++count){
  CSalSwFilter actual;actual.m_Params.fF=.25f;actual.m_Params.fQ=.5f;actual.m_Params.fG=.25f;actual.m_NewParams.fF=.5f;actual.m_NewParams.fQ=.25f;actual.m_NewParams.fG=.5f;actual.m_fZ0=actual.m_fZ1=0;actual.m_nFadeSamples=start;
  short buffer[7]={0,0,0,0,0,0,2222};fade_calls=0;fade_count=99;actual.ProcessEQ(buffer,count);unsigned int fade=4-start;if(fade>count)fade=count;assert(fade_calls==1&&fade_count==fade);for(unsigned int j=0;j<fade;++j)assert(buffer[j]==-123);assert(buffer[6]==2222);
  if(start+fade==4)assert(actual.m_nFadeSamples==0&&actual.m_Params.fF==.5f&&actual.m_Params.fQ==.25f&&actual.m_Params.fG==.5f);else assert(actual.m_nFadeSamples==start+fade&&actual.m_Params.fF==.25f);++cases;
 }
 assert(cases==348);puts("00c6b9d0_TEST PASS");return 0;
}
