// CSalSwBaseVoice::UpdateMultiFuncLFO
// Flags:
// Leaf: ?UpdateMultiFuncLFO@CSalSwBaseVoice@@AAEXPBU_CMULTIFUNCLFO@@@Z
// Retail: 0x00c5a9d0, 382 bytes, thiscall.
#include <math.h>
#include "engine/CSalSwBaseVoice.h"
#ifdef _MSC_VER
#pragma intrinsic(pow)
#endif
void CSalSwBaseVoice::UpdateMultiFuncLFO(const _CMULTIFUNCLFO* pLFO){
 m_ffStatus&=~0x18000;
 if(pLFO){
  if(pLFO->fFrequency>0) m_nMultiLFOFrameCount=TimeToFrames(0.5f/pLFO->fFrequency);
  else m_nMultiLFOFrameCount=0;
  if(m_nMultiLFOFrameCount){
   m_fMultiLFOPitchDeltaUp=powf(2.0f,2.0f*pLFO->fPitchModulation/m_nMultiLFOFrameCount);
   m_fMultiLFOPitchDeltaDown=powf(2.0f,-2.0f*pLFO->fPitchModulation/m_nMultiLFOFrameCount);
   m_fMultiLFOCutoffDeltaUp=powf(2.0f,2.0f*pLFO->fFilterCutOff/m_nMultiLFOFrameCount);
   m_fMultiLFOCutoffDeltaDown=powf(2.0f,-2.0f*pLFO->fFilterCutOff/m_nMultiLFOFrameCount);
   float gain=pLFO->iLevelModulation/1000.0f;
   m_fMultiLFOGainDeltaUp=powf(10.0f,gain/m_nMultiLFOFrameCount);
   m_fMultiLFOGainDeltaDown=powf(10.0f,-gain/m_nMultiLFOFrameCount);
  }else{
   m_fMultiLFOPitchDeltaUp=1.0f; m_fMultiLFOPitchDeltaDown=1.0f;
   m_fMultiLFOGainDeltaUp=1.0f; m_fMultiLFOGainDeltaDown=1.0f;
   m_fMultiLFOCutoffDeltaUp=1.0f; m_fMultiLFOCutoffDeltaDown=1.0f;
  }
 }else m_ffStatus&=~0x18000;
 m_fMultiLFOPitch=1.0f; m_fMultiLFOGain=1.0f; m_fMultiLFOCutoff=1.0f;
 m_nMultiLFOFrames=0;
}