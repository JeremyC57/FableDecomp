#ifndef FABLE_CSALSW3DVOICE_H
#define FABLE_CSALSW3DVOICE_H
#include "engine/CSalSwVoice.h"
class CCoda3DSource;
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#include "engine/CSalIIRFilter.h"
class CSalRender;
#endif
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw3DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda3DSource*,unsigned int);
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 CSalSw3DVoice();virtual ~CSalSw3DVoice();
private:
 // SDK names and native ctor00c555e0: frame1f0, render220, filters224/230.
 CSalSwBuffer m_FrameBuffer;
 float m_fAzimuth;
 float m_fElevation;
 float m_fMacroFXGain[2];
 CSalRender* m_pRender;
 CSalIIRFilter m_filterDirect;
 CSalIIRFilter m_filterEnvironment;
 unsigned short m_gainEnvironment;
 unsigned short m_gainEnvironmentNew;
 bool m_bUpdateEnvironmentGain;
#endif
};
#endif
