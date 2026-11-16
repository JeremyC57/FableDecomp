#ifndef FABLE_CSALSW2DVOICE_H
#define FABLE_CSALSW2DVOICE_H
#include "engine/CSalSwVoice.h"
class CCoda2DSource;
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw2DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda2DSource*,unsigned int);
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 CSalSw2DVoice();virtual ~CSalSw2DVoice();
private:
 // Original SDK member names; native fork has six gain channels and history channels.
 CSalSwBuffer m_FrameBuffer;
 float m_fGains[6];
 bool m_bUpdateGain;
 unsigned short m_gainArray[6];
 unsigned short m_gainArrayNew[6];
 int m_iReverbLevel;
 bool m_bUpdateEnvironmentGain;
 unsigned short m_gainEnvironment;
 unsigned short m_gainEnvironmentNew;
#endif
};
#endif
