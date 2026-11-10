#ifndef FABLE_CSAL_SW_BASE_VOICE_H
#define FABLE_CSAL_SW_BASE_VOICE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CSalBaseVoice.h"
#include "engine/CSalSwBuffer.h"
#include "engine/CSalSwFilter.h"
struct _CBUFFERDESC{
 unsigned int ffFormat;
 unsigned int nChannels;
 unsigned int nSamplesPerSec;
 unsigned int cbSize;
 unsigned int cbLoopStart;
 unsigned int cbLoopEnd;
 void * pvData;
};
class CSalReverb;
class CSalSwBaseVoice:public CSalBaseVoice{
public: virtual ~CSalSwBaseVoice();
public: virtual bool IsSoftware() const;
public: virtual int Create(const CCodaSource *);
public: virtual void Release();
public: virtual unsigned int PlayQueue(CCodaSource *,CREFTIME );
public: virtual bool Stop(CREFTIME );
public: virtual unsigned int Update(CCodaSource *,CREFTIME );
public: virtual unsigned int GetPlayCursor() const;
public: virtual void SetPlayCursor(unsigned int );
protected: virtual void InitPlay();
protected: virtual void SetPlayBuffer(const CSalBuffer &,bool );
protected: virtual unsigned int GetPlatformObjectSize();
protected: virtual unsigned int GetPlatformBufferLoopStart();
protected: virtual unsigned int GetPlatformBufferEnd();
protected: virtual unsigned int InitParams(const CCodaSource *,const CODASOURCE &);
protected: virtual void ConvertFormat(CSalSwBuffer &,const void *,unsigned int ,unsigned int );
protected: virtual void Mix(CSalSwBuffer &,unsigned int ,bool );
protected: virtual void MixReverbInput(CSalReverb *,CSalSwBuffer &,unsigned int );
protected: virtual void MixChannel(CSalSwBuffer &,unsigned int ,unsigned int );
protected: virtual void StartRunOut(CREFTIME );
private:
 void UpdateMultiFuncLFO(const _CMULTIFUNCLFO*);
protected:
 _CBUFFERDESC m_BufferDesc;
 float m_fMultiEnvPitch;
 float m_fMultiEnvPitchDelta;
 float m_fMultiEnvCutoff;
 float m_fMultiEnvCutoffDelta;
 int m_nMultiEnvFrames;
 float m_fMultiLFOPitch;
 float m_fMultiLFOPitchDeltaUp;
 float m_fMultiLFOPitchDeltaDown;
 float m_fMultiLFOGain;
 float m_fMultiLFOGainDeltaUp;
 float m_fMultiLFOGainDeltaDown;
 float m_fMultiLFOCutoff;
 float m_fMultiLFOCutoffDeltaUp;
 float m_fMultiLFOCutoffDeltaDown;
 int m_nMultiLFOFrames;
 int m_nMultiLFOFrameCount;
 CSalSwBuffer m_PlayBuffer;
 unsigned int m_nRunOutFrames;
 CSalSwBuffer *m_pMixBuffer;
 CSalSwFilter m_Filter[2];
private:
 short m_asHistory[2][16];
 unsigned int m_nHistorySamples;
 unsigned int m_uiSrcRate;
 unsigned int m_uiSrcCount;
};
#endif
