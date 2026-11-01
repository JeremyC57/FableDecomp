#include <stdio.h>
#include <stddef.h>
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
#define __int64 CTestInt64
struct CTestInt64 { unsigned int low, high; };
#endif
// Fixture-only access projection; target implementation pasted unchanged.
#define private public
#define protected public
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

#undef private
#undef protected
#ifdef _MSC_VER
typedef char source_queue_offset[(offsetof(CCodaSource,m_Queue)==0x54)?1:-1];
typedef char source_buffer_offset[(offsetof(CCodaSource,m_pBuffer)==0x78)?1:-1];
typedef char buffer_desc_offset[(offsetof(CSalBufferBase,m_Desc)==0x10)?1:-1];
typedef char queue_size[(sizeof(CCodaBufferQueue)==36)?1:-1];
#endif
CCodaObjectBase::~CCodaObjectBase() {}
CCodaContext CODA;
static int eventCount,eventKinds[8],errorValue;
static const CSalBufferBase* seenBuffer;
static CCodaSource* seenSource;
static unsigned int seenCursor;
static CCodaSource* changedSource;
static int changeDuringDesc;
const _CBUFFERDESC& CSalBufferBase::GetDesc() const {
 eventKinds[eventCount++]=1; seenBuffer=this;
 if(changeDuringDesc) changedSource->m_pBuffer=0;
 return m_Desc;
}
void CCodaSource::SetPlayCursor(unsigned int cbCursor) {
 eventKinds[eventCount++]=2; seenSource=this; seenCursor=cbCursor;
}
void CCodaContext::SetError(int nError) {
 eventKinds[eventCount++]=3; errorValue=nError;
}
static int failures;
static void Check(bool condition) { if(!condition) ++failures; }
int main() {
 unsigned int values[5]={0u,1u,0x7fffffffu,0x80000000u,0xffffffffu};
 int cases=0;
 for(int overrideIndex=0;overrideIndex<3;++overrideIndex)
 for(unsigned int count=0;count<5;++count)
 for(unsigned int format=0;format<3;++format)
 for(unsigned int index=0;index<4;++index)
 for(int valueIndex=0;valueIndex<5;++valueIndex)
 for(int mutation=0;mutation<2;++mutation) {
  CCodaSource source;
  CSalBuffer buffers[4];
  CSalBuffer* pointers[4];
  for(int i=0;i<4;++i) {
   pointers[i]=&buffers[i];
   memset(&buffers[i].m_Desc,0,sizeof(buffers[i].m_Desc));
   buffers[i].m_Desc.cbSize=64u+unsigned(i);
  }
  source.m_Queue.m_ppBuffer=pointers;
  source.m_Queue.m_pPlayTime=0;
  source.m_Queue.m_nBuffers=count;
  source.m_Queue.m_nAdd=0;
  source.m_Queue.m_nPlay=count?index%count:0;
  source.m_Queue.m_hLastBuffer=0;
  source.m_Queue.m_Format.ffFormat=format;
  source.m_Queue.m_Format.nChannels=2;
  source.m_Queue.m_Format.nSamplesPerSec=44100;
  source.m_pBuffer=overrideIndex?&buffers[overrideIndex]:0;
  CSalBuffer* expected=source.m_pBuffer;
  bool valid=expected!=0 || (count!=0 && format!=0);
  if(!expected && valid) expected=pointers[source.m_Queue.m_nPlay];
  eventCount=0; errorValue=0; seenBuffer=0; seenSource=0; seenCursor=0;
  changeDuringDesc=mutation; changedSource=&source;
  CODA_SourceSetPlayCursor(&source,values[valueIndex]);
  if(!valid) {
   Check(eventCount==1 && eventKinds[0]==3 && errorValue==-7);
   Check(seenBuffer==0 && seenSource==0);
  } else {
   Check(eventCount==2 && eventKinds[0]==1 && eventKinds[1]==2);
   Check(seenBuffer==expected && seenSource==&source && seenCursor==values[valueIndex]);
   Check(errorValue==0);
  }
  ++cases;
 }
 Check(cases==1800);
 if(failures) { fprintf(stderr,"failures: %d\n",failures); return 1; }
 puts("00c35e30_TEST PASS"); return 0;
}