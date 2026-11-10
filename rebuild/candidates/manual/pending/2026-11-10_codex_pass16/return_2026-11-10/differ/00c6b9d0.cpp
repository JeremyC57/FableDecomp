// 00c6b9d0 CSalSwFilter::ProcessEQ -- apply the equalizer to signed16 samples.
// Grade: DIFFER(245v269) -- native rounding instruction has no matched portable form.
// Retail: 0x00c6b9d0,269 bytes,thiscall.
// Flags:
// Leaf: ?ProcessEQ@CSalSwFilter@@QAEXPAFI@Z
#include <math.h>
#include "engine/CSalSwFilter.h"
#include "engine/CodaCaps.h"
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
