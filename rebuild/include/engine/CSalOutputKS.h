#ifndef FABLE_CSAL_OUTPUT_KS_H
#define FABLE_CSAL_OUTPUT_KS_H
#include "KernelStreaming.h"
#include "CodaEnvironment.h"
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
// NF404 opt-in (see the block after the classes): the Win32 imports the inline member bodies call.
extern "C" {
 __declspec(dllimport) BOOL __stdcall CloseHandle(HANDLE);
 __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION*);
 __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION*);
}
#endif
// All class and member names and access levels from original GameCODA SDK COFF.
class CSalFile {
public:
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
 CSalFile():m_hDevice((HANDLE)-1){}
#endif
 virtual ~CSalFile();
 void Attach(HANDLE);
 void Close();
 operator void*(){return m_hDevice;}
private:HANDLE m_hDevice;
};
class CSalKsPin:public CSalFile {
public:
 virtual ~CSalKsPin();
 bool SetConnectionState(KSSTATE);
#ifdef FABLE_RETAIL_CSAL_KS_PIN_PASS22_PROPERTIES
 // Retail opt-in (Codex pass22): original GameCODA SDK symbol and native00C5E050 agree.
 bool SetProperty(const GUID&,unsigned long,void*,unsigned long);
#endif
};
class CSalEvent {
public:
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
 CSalEvent():m_handle(0){}
#endif
 virtual ~CSalEvent();
 bool Create(SECURITY_ATTRIBUTES* attributes,bool manual,bool initial,const char* name){m_handle=CreateEventA(attributes,manual,initial,name);return m_handle!=0;}
 void Close();
 operator void*(){return m_handle;}
private:HANDLE m_handle;
};
class CSalThread {
public:
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
 CSalThread():m_handle(0){}
 void Close();
#endif
 virtual ~CSalThread();
 bool Create(LPTHREAD_START_ROUTINE,void*,SECURITY_ATTRIBUTES*,unsigned int,unsigned int);
 void SetPriority(int priority){SetThreadPriority(m_handle,priority);}
private:HANDLE m_handle;DWORD m_threadId;
};
class CSalMutex {
public:virtual ~CSalMutex();
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
 CSalMutex(){InitializeCriticalSection(&m_critSection);}
#endif
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
private:CRITICAL_SECTION m_critSection;
#else
private:CRITICAL_SECTION m_critSection;bool m_bActive;
#endif
};
class CSalOutput:public CCodaObjectBase {
public:
 virtual ~CSalOutput();
 virtual bool Create(const GUID*,HWND,bool)=0;
 virtual void Release()=0;
 virtual bool CreateBuffer(unsigned int,unsigned int)=0;
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
 // Descriptive method: native slot4 is called for the reverb output with frame samples.
 // C3F1B6 passes g_SalCaps.SWMixer.nFrameSamples; default C572C0 returns false/ret4.
 virtual bool CreateReverbBuffer(unsigned int);
#endif
 virtual void ReleaseBuffer()=0;
 virtual bool FrameIsReady()=0;
 virtual void* FrameLock()=0;
 virtual void FrameUnlock(void*)=0;
 virtual unsigned int GetMinBufferSamples()const=0;
};
struct KsPacketHeader {KSSTREAM_HEADER header;OVERLAPPED signal;};
// Layout: native receiver offsets and SDK full0x60 class agree.
class CSalOutputKS:public CSalOutput {
public:
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
 CSalOutputKS();
#endif
 virtual ~CSalOutputKS();
 virtual bool Create(const GUID*,HWND,bool);
 virtual void Release();
 virtual bool CreateBuffer(unsigned int,unsigned int);
 virtual void ReleaseBuffer();
 virtual bool FrameIsReady();
 virtual void* FrameLock();
 virtual void FrameUnlock(void*);
 virtual unsigned int GetMinBufferSamples()const;
private:
 static unsigned long __stdcall NotifyProc(void*);
 CSalFile m_filter;
 CSalKsPin m_pin;
 KsPacketHeader* m_pPacketHeaderTable;
 HANDLE* m_pEventTable;
 char* m_pBuffer;
 unsigned int m_nFrames,m_nFrameCount,m_nFrameIndex;
 CSalEvent m_eventTerminate;
 CSalThread m_threadNotify;
 CSalMutex m_mutexFrameCount;
};
#ifdef FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
// NF404 opt-in (rows 0x00C5ECA0 ??1CSalOutputKS@@UAE@XZ and 0x00C5ED90 ??0CSalOutputKS@@QAE@XZ; needs FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
// for the retail slot order: ReleaseBuffer is slot 5, `call [0x012B9160]`): the member constructors and destructors as the retail TU expands
// them. The original GameCODA 2.10 SDK CODAApi.lib Release/SalOutputKS.obj defines ??1CSalFile 33 B, ??1CSalKsPin 33 B, ??1CSalEvent 32 B,
// ??1CSalThread 32 B and ??1CSalOutput 7 B, byte-identical (relocations masked) to the retail out-of-line copies the dtor's unwind funclets
// jump to (0x00C5EBD0 / 0x00C5EC50 / 0x00C5E420 / 0x00C5E480 / 0x00C56E60), and its ??0CSalOutputKS stores the same handles (-1 / -1 / 0 / 0).
// Retail also expands CSalMutex (vtable 0x012B8FC8: InitializeCriticalSection / DeleteCriticalSection on +4; out-of-line copy 0x00C5D570),
// which that SDK build calls out of line. Close() is the SDK's own member of CSalFile / CSalEvent / CSalThread (?Close@...@@QAEXXZ).
// Users also define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR (engine/CCodaObjectBase.h: the root destructor these inline bodies end in).
inline void CSalFile::Close(){if(m_hDevice!=(HANDLE)-1){CloseHandle(m_hDevice);m_hDevice=(HANDLE)-1;}}
inline CSalFile::~CSalFile(){Close();}
// FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY (Fidelity: scaffolding, per TU): leaves ~CSalKsPin declared only, so the TU's ??_GCSalKsPin
// calls it out of line, as retail's 30-byte copy at 0x00C5EC80 does (vtable 0x012B9148 slot 0; the SDK 2.10 build expands it, 53 bytes).
// Users: 0x00C5ED90 (CSalOutputKS constructor, which emits that ??_G through the vtable it stores) and 0x00C5EC80. The TUs that destroy a
// CSalKsPin (0x00C5ECA0 ~CSalOutputKS, 0x00C5EC50) keep the inline body: retail ~CSalOutputKS expands ~CSalKsPin at +0xC.
#ifndef FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY
inline CSalKsPin::~CSalKsPin(){}
#endif
inline void CSalEvent::Close(){if(m_handle){CloseHandle(m_handle);m_handle=0;}}
inline CSalEvent::~CSalEvent(){Close();}
inline void CSalThread::Close(){if(m_handle){CloseHandle(m_handle);m_handle=0;}}
inline CSalThread::~CSalThread(){Close();}
inline CSalMutex::~CSalMutex(){DeleteCriticalSection(&m_critSection);}
inline CSalOutput::~CSalOutput(){}
#endif
#endif
