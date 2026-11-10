#ifndef FABLE_CSAL_BASE_VOICE_H
#define FABLE_CSAL_BASE_VOICE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CSalVoice.h"
// Layout: native flags+28, params+2c, pitch LFO ends+4c.
struct EAXSOURCE_tag;
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
 static int FABLE_CDECL TimeToFrames(float);
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
