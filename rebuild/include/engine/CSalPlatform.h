#ifndef FABLE_CSALPLATFORM_H
#define FABLE_CSALPLATFORM_H
#include "engine/CCodaContext.h"
#include "engine/CodaProcessingTypes.h"
#include "engine/CODA_PlatformTypes.h"
// Original Windows SDK union and API; no windows.h dependency.
union _LARGE_INTEGER {struct {unsigned long LowPart;long HighPart;}u;CREFTIME QuadPart;};
#ifdef _MSC_VER
extern "C" __declspec(dllimport) int FABLE_STDCALL QueryPerformanceCounter(_LARGE_INTEGER*);
#else
extern "C" int FABLE_STDCALL QueryPerformanceCounter(_LARGE_INTEGER*);
#endif
class CSalContext:public CCodaObjectBase {
private:
 CCodaObjectList m_Hw2DVoices;
 CCodaObjectList m_Hw3DVoices;
 CCodaObjectList m_Sw2DVoices;
 CCodaObjectList m_Sw3DVoices;
 unsigned int m_ffUpdate;
};
class CSalMixer;class CSalSw2DVoice;class CSalSw3DVoice;class CSalBaseReverb;class CSalOutput;class CSalOutputConfig;class CSalHw;struct _CBUFFERDESC;
// Layout: original complete SDK472-byte platform; native timerStart400 exactly.
class CSalPlatform:public CSalContext {
public:
 bool ReadyToProcess();
 void ProcessXtc();
 int ProcessOutput();
 CREFTIME GetReferenceTime(){_LARGE_INTEGER time;QueryPerformanceCounter(&time);return time.QuadPart-timerStart.QuadPart;}
private:
 CSalMixer *m_pMixer;
 CSalXTC m_xtcFront;
 CSalXTC m_xtcRear;
 CSalSw2DVoice *m_Sw2DVoiceMem;
 CSalSw3DVoice *m_Sw3DVoiceMem;
 CSalBaseReverb *m_pReverb;
 CSalOutput *m_pOutputStream;
 CSalOutput *m_pReverbOutputStream;
 CSalOutputConfig *m_pOutputConfig;
 _LARGE_INTEGER timerStart;
 _LARGE_INTEGER timerPerfStart;
 float fTimerTicksPerMs;
 void (FABLE_STDCALL *m_pfnSwMixOutputCallback)(const _CBUFFERDESC *,int );
 void (FABLE_STDCALL *m_pfnSwMixPreLimiterCallback)(const _CBUFFERDESC *,int );
 CREFTIME m_rtFrameTime;
 COUTPUTCONFIG m_SWOutputConfig;
 unsigned int m_nSWOutputChannels;
 COUTPUTCONFIG m_OutputConfig;
 bool m_bXtcProcessing;
 unsigned int m_nSw2DVoicesPlaying;
 unsigned int m_nSw3DVoicesPlaying;
 unsigned int m_nHwVoicesPlaying;
 CSalHw *m_pHw;
};
#endif
