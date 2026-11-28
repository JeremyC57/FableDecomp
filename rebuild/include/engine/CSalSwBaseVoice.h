#ifndef FABLE_CSAL_SW_BASE_VOICE_H
#define FABLE_CSAL_SW_BASE_VOICE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CSalBaseVoice.h"
#include "engine/CSalSwBuffer.h"
#include "engine/CSalSwFilter.h"
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "engine/CodaBufferDesc.h"
#else
struct _CBUFFERDESC{
 unsigned int ffFormat;
 unsigned int nChannels;
 unsigned int nSamplesPerSec;
 unsigned int cbSize;
 unsigned int cbLoopStart;
 unsigned int cbLoopEnd;
 void * pvData;
};
#endif
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
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
 void CalcParams(const CCodaSource*,CREFTIME);
 bool CalcMultiFuncEnvelope(const _CMULTIFUNCENVELOPE*,bool);
 void CalcMultiFuncLFO(const _CMULTIFUNCLFO*);
#endif
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
public:
 bool IsStopped() const { return (m_ffParams & 0x100) || (!m_PlayBuffer.IsCreated() && !IsRunningOut()); }
private:
 static CSalSwBuffer m_ProcessBuffer;
protected:
 static CSalSwBuffer m_FormatBuffer;
private:
 void ConvertSampleRate(CSalSwBuffer&, CSalSwBuffer&, unsigned int, unsigned int);
 bool IsRunningOut() const { return m_nRunOutFrames != 0; }
 bool IsHistory() const { return m_nHistorySamples != 0; }
 bool IsFiltering() const { return (m_ffStatus & 0x1e0000) != 0; }
 bool IsLPFiltering() const { return (m_ffStatus & 0xa0000) != 0; }
 bool IsEQFiltering() const { return (m_ffStatus & 0x140000) != 0; }
 bool IsSrcActive() const { return m_uiSrcRate != 0x1000 || m_uiSrcCount != 0; }
 bool IsStereoFiltering(const CCodaSource* pSource) const;
 bool IsFormatConverted() const;
 void ProcessSilence(unsigned int);
 unsigned int ProcessSamples(CCodaSource*,bool,unsigned int);
 void SetNextPlayBuffer(CCodaSource*);
 void StereoFilter(short*,unsigned int,unsigned int);
#endif
 void UpdateMultiFuncLFO(const _CMULTIFUNCLFO*);
protected:
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
 void UpdatePlayBuffer(bool);
 bool CalcLevelEnvelope(const _CLEVELENVELOPE*,bool);
 bool IsMultiEnvReleased() const {return (m_ffStatus & 0x4000)!=0;}
#endif
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView protected members. SetPitch is
 // ?SetPitch@CSalSwBaseVoice@@IAEXMABUCODASOURCE@@@Z (native 0x00C5A500); ReleasePlayBuffer (the Dbg CalcParams calls it) is
 // expanded by the release: 0x00C54C46 `lea ecx,[esi+0B4h] ; call CSalSwBuffer::Release ; mov [esi+0Ch],0` (m_pcBuffer).
 void SetPitch(float,const CODASOURCE&);
 void ReleasePlayBuffer(){m_PlayBuffer.Release();m_pcBuffer=0;}
#endif
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
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
 // Native history begins124, advances20 per channel, and ends at1e4: six channels.
 short m_asHistory[6][16];
#else
 short m_asHistory[2][16];
#endif
 unsigned int m_nHistorySamples;
 unsigned int m_uiSrcRate;
 unsigned int m_uiSrcCount;
};
#ifdef FABLE_RETAIL_SAL_SW_BASE_VOICE_UPDATE_INLINES_PASS30
#include "engine/CCodaSource.h"
#include "engine/CSalBuffer.h"
#include "engine/CodaPCM.h"
inline bool CSalSwBaseVoice::IsStereoFiltering(const CCodaSource* pcSource) const {
    return IsFiltering() && pcSource->IsStereoFilter() ? 1 : 0;
}
inline bool CSalSwBaseVoice::IsFormatConverted() const {
    const _CBUFFERDESC& desc=m_pcBuffer->GetDesc();
    return (desc.ffFormat == C_FORMAT_INT16 && desc.nChannels <= 1 && 0 == ((size_t)desc.pvData % 16)) ? 0 : 1;
}

inline void CSalSwBaseVoice::StereoFilter(short* psData, unsigned int nSamples, unsigned int nChannel)
{
    bool bFilterActive;
    switch (nChannel) {
    case 0: bFilterActive = (m_ffStatus & 0x60000) != 0; break;
    case 1: bFilterActive = (m_ffStatus & 0x180000) != 0; break;
    default: bFilterActive = false; break;
    }
    if (bFilterActive) {
        if (IsLPFiltering()) {
            m_Filter[nChannel].ProcessLP(psData, nSamples);
        } else {
            m_Filter[nChannel].ProcessEQ(psData, nSamples);
        }
    }
}
#endif
#endif
