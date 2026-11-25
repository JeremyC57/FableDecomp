#ifndef FABLE_CSALSW3DVOICE_H
#define FABLE_CSALSW3DVOICE_H
#include "engine/CSalSwVoice.h"
class CCoda3DSource;
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#include "engine/CSalIIRFilter.h"
class CSalRender;
#ifdef FABLE_RETAIL_CSALSW3D_CREATE
#include "engine/CodaProcessingTypes.h"
#endif
#endif
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw3DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda3DSource*,unsigned int);
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 CSalSw3DVoice();virtual ~CSalSw3DVoice();
#ifdef FABLE_RETAIL_CSALSW3D_CREATE
 // NF445 opt-in (rows 0x00C556E0 Create, 0x00C55680 Initialize, 0x00C55270 CreateRenderer; needs PASS23): SDK 2.10 CODAApi.lib
 // SalSw3DVoice.obj symbols ?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z (public virtual, retail vftable 0x012B8AE0 slot 1),
 // ?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z and ?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z (private).
 // Create overrides CSalSwBaseVoice::Create, so the slot order is unchanged.
 virtual int Create(const CCodaSource*);
private:
 bool Initialize(CHRTFMODE);
 CSalRender* CreateRenderer(CHRTFMODE);
#endif
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
