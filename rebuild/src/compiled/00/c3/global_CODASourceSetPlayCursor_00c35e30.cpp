// 00c35e30 CODA_SourceSetPlayCursor
// Flags:
// Leaf: _CODA_SourceSetPlayCursor@8
// Layout: original GameCODA CodeView types and release COFF prove all fields below;
// see SDK_NAME_RECOVERY.md. CCodaObject's debug-only m_DebugTest is absent in retail.
class CCodaObjectBase { public: virtual ~CCodaObjectBase(); };
class CCodaObject : public CCodaObjectBase {
private: CCodaObject* m_pPrev; CCodaObject* m_pNext;
};
struct _CBUFFERDESC { unsigned int ffFormat,nChannels,nSamplesPerSec,cbSize,cbLoopStart,cbLoopEnd; void* pvData; };
class CCodaRefCounter { private: unsigned int m_uiRefCount; };
class CSalBufferBase : public CCodaObject, public CCodaRefCounter {
protected: _CBUFFERDESC m_Desc; unsigned int m_ffStatus;
public: const _CBUFFERDESC& GetDesc() const;
};
class CSalBuffer : public CSalBufferBase {};
struct _CLPFILTER { unsigned int uiFreq; float fQ; };
struct _CEQFILTER { unsigned int uiFreq; float fGain; float fQ; };
struct _CLEVELENVELOPE;
struct _CMULTIFUNCENVELOPE;
struct _CPITCHLFO;
struct _CMULTIFUNCLFO;
struct CODASOURCE {
 unsigned int ffUpdate; float fGain; int iLevel; float fPitch;
 _CLPFILTER* pLPFilter; _CEQFILTER* pEQFilter;
 _CLEVELENVELOPE* pLevelEnv; _CMULTIFUNCENVELOPE* pMultiEnv;
 _CPITCHLFO* pPitchLFO; _CMULTIFUNCLFO* pMultiLFO;
};
struct CQUEUEFORMAT { unsigned int ffFormat,nChannels,nSamplesPerSec; };
class CCodaBufferQueue {
private:
 CSalBuffer** m_ppBuffer; __int64* m_pPlayTime;
 unsigned int m_nBuffers,m_nAdd,m_nPlay; void* m_hLastBuffer; CQUEUEFORMAT m_Format;
public:
 bool IsCreated() const { return m_nBuffers != 0; }
 bool IsEmpty() const { return m_nBuffers && !m_Format.ffFormat; }
 CSalBuffer* GetPlayBuffer() const {
  if (m_nBuffers) return m_ppBuffer[m_nPlay];
  return 0;
 }
};
class CSalVoice;
class CCodaSource : public CCodaObject {
protected:
 unsigned int m_ffCaps,m_ffStatus; CODASOURCE m_Params;
 _CLPFILTER m_LPFilter; _CEQFILTER m_EQFilter; CSalVoice* m_pVoice;
private:
 CCodaBufferQueue m_Queue; CSalBuffer* m_pBuffer;
public:
 bool IsBufferSet() const { return m_pBuffer || (m_Queue.IsCreated() && !m_Queue.IsEmpty()); }
 const _CBUFFERDESC& GetBufferDesc() const {
  return (m_pBuffer ? m_pBuffer : m_Queue.GetPlayBuffer())->GetDesc();
 }
 void SetPlayCursor(unsigned int cbCursor);
};
class CCodaContext { public: void SetError(int nError); };
extern CCodaContext CODA;
extern "C" void __stdcall CODA_SourceSetPlayCursor(void* hSource, unsigned int cbCursor) {
 CCodaSource* pSource = static_cast<CCodaSource*>(hSource);
 if (!pSource->IsBufferSet()) { CODA.SetError(-7); return; }
 pSource->GetBufferDesc();
 pSource->SetPlayCursor(cbCursor);
}