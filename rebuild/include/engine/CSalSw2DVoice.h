#ifndef FABLE_CSALSW2DVOICE_H
#define FABLE_CSALSW2DVOICE_H
#include "engine/CSalSwVoice.h"
#ifdef FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
#include "engine/Coda2DSourceTypes.h"
// SDK CodeView ChanId and the original six-way switch in SalSw2DVoice.obj.
enum ChanId { SAL_CHAN_UNKNOWN=-1, SAL_CHAN_LEFT, SAL_CHAN_RIGHT, SAL_CHAN_LEFT_REAR, SAL_CHAN_RIGHT_REAR, SAL_CHAN_CENTRE, SAL_CHAN_LFE };
#endif
class CCoda2DSource;
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw2DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda2DSource*,unsigned int);
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 CSalSw2DVoice();virtual ~CSalSw2DVoice();
#ifdef FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
private:
 static ChanId FABLE_CDECL SpeakerIdToChannel(CSPEAKERID);
 void SetSingleChannelGain(unsigned int,float);
 void SetReverbLevel(int);
#endif
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
