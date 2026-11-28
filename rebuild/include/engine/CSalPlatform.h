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
#ifdef FABLE_RETAIL_PASS22_CCODA3D_CREATE
class CCodaSource;class CSalVoice;
#endif
class CSalContext:public CCodaObjectBase {
#ifdef FABLE_RETAIL_PASS22_CCODA3D_CREATE
public: int AllocateSw3DVoice(const CCodaSource*,CSalVoice**);
#endif
#ifdef FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
public: static int Create(const CRESOURCEDESC*,const CPLATFORMDESC*,CSalPlatform**);
#endif
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
public: unsigned int GetUpdate()const{return m_ffUpdate;}
#endif
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
protected:
#else
private:
#endif
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
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView public ?GetReverb@CSalPlatform@@QBEPAVCSalBaseReverb@@XZ (+0x17c).
 CSalBaseReverb* GetReverb()const{return m_pReverb;}
#endif
private:
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 // SDK private CreateSwVoices; native00c3f800/caller00c3fca9 proves bool in this fork.
 bool CreateSwVoices();
#endif
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
 // SDK CreateSw has an int result in the newer library; this native fork returns bool.
 bool CreateSw(const CRESOURCEDESC*,const CPLATFORMDESC*);
 bool SetSWOutputConfig();
 // Descriptive native-only split helpers; exact original names unavailable.
 bool CreateSwDirectSound(const CPLATFORMDESC*);
 bool CreateSwBufferedDirectSound(const CPLATFORMDESC*);
#endif
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
#ifdef FABLE_RETAIL_CSALBUFFER_USE_PORT30
 // Native CSalBuffer::Use tests the platform hardware pointer. The SDK symbol is private.
 // Inferred: friendship allows the native direct use without changing the proven method access.
 friend class CSalBuffer;
 bool IsHwCreated() const { return m_pHw != 0; }
#endif
 CSalHw *m_pHw;
};
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
extern CSalPlatform* g_pSal;
#endif
#endif
