#ifndef FABLE_CSAL_BASE_VOICE_H
#define FABLE_CSAL_BASE_VOICE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CSalVoice.h"
// Layout: native flags+28, params+2c, pitch LFO ends+4c.
struct EAXSOURCE_tag;
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
class CCodaVector;
#endif
class CSalBaseVoice:public CSalVoice {
public: virtual ~CSalBaseVoice();
public: virtual bool IsCreated() const;
public: virtual int Create(const CCodaSource *);
public: virtual int SetBuffer(const CCodaSource *);
public: virtual void Release();
public: virtual unsigned int PlayBuffer(const CCodaSource *);
public: virtual unsigned int Resume(CREFTIME );
public: virtual void ReleaseEnvelopes(CREFTIME );
public: virtual int EAXSet(const _GUID *,int ,void * ,int );
public: virtual int EAXGet(const _GUID *,int ,void * ,int );
protected: virtual void InitPlay();
protected: virtual void SetPlayBuffer(const CSalBuffer &,bool );
protected:
#ifdef FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
 static void FABLE_CDECL PanToGains(float,float&,float&);
#endif
#ifdef FABLE_RETAIL_CSALBASEVOICE_QUEUE_PORT30
 // Original SDK release PlayQueue calls this protected method before virtual InitPlay.
 void InitQueue(CCodaSource*);
#endif
 static int FABLE_CDECL TimeToFrames(float);
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
 bool IsStarting() const { return (m_ffStatus & 1) != 0; }
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
 bool IsReleasingEnvelopes() const { return (m_ffStatus & 0x20) != 0; }
 bool IsReleasing() const { return (m_ffParams & 0x200) != 0; }
 bool IsStopping() const { return (m_ffParams & 0x80) != 0; }
#else
 bool IsReleasingEnvelopes() const { return (m_ffParams & 0x200) != 0; }
#endif
 bool IsLevelEnvReleased() const { return (m_ffStatus & 0x40) != 0; }
 bool CalcLevelEnvelope(const _CLEVELENVELOPE*,bool);
 void UpdatePitchLFO(const _CPITCHLFO*);
 void CalcPitchLFO(const _CPITCHLFO*);
 static float FABLE_CDECL CalcExpDelta(float,float);
 static float FABLE_CDECL CalcPitchDelta(float,float,float);
#endif
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView protected members of CSalBaseVoice. GetHrtfMode (unsigned, const)
 // is the mode field of m_ffParams (release 0x00C54D65 `and ecx,3C00h`); CalcAzimuthElevation is the protected static
 // ?CalcAzimuthElevation@CSalBaseVoice@@KAXAAM0ABVCCodaVector@@@Z (native 0x00C599C0).
 unsigned int GetHrtfMode() const { return m_ffParams & 0x3c00; }
 static void FABLE_CDECL CalcAzimuthElevation(float&,float&,const CCodaVector&);
#endif
 const CSalBuffer *m_pcBuffer;
 CREFTIME m_rtBufferPlay;
 CREFTIME m_rtStop;
 CREFTIME m_rtRelease;
 unsigned int m_ffStatus;
 unsigned int m_ffParams;
 float m_fLevelEnvGain;
 float m_fLevelEnvDelta;
 int m_nLevelEnvFrames;
 float m_fPitchLFOPitch;
 float m_fPitchLFODeltaUp;
 float m_fPitchLFODeltaDown;
 int m_nPitchLFOFrames;
 int m_nPitchLFOFrameCount;
 // Layout: source EAX pointer is copied to+50 by Create at00c596e0;
 // EAXSet/EAXGet pass it onward. The receiving member spelling is inferred
 // from the proven CCodaSource member. Natural tail alignment gives size58.
 EAXSOURCE_tag *m_pEAXSource;
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CSalBaseVoiceSize[(sizeof(CSalBaseVoice)==0x58)?1:-1];
#endif
#endif
