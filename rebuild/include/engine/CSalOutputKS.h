#ifndef FABLE_CSAL_OUTPUT_KS_H
#define FABLE_CSAL_OUTPUT_KS_H
#include "KernelStreaming.h"
#include "CodaEnvironment.h"
// All class and member names and access levels from original GameCODA SDK COFF.
class CSalFile {
public:
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
};
class CSalEvent {
public:
 virtual ~CSalEvent();
 bool Create(SECURITY_ATTRIBUTES* attributes,bool manual,bool initial,const char* name){m_handle=CreateEventA(attributes,manual,initial,name);return m_handle!=0;}
 void Close();
 operator void*(){return m_handle;}
private:HANDLE m_handle;
};
class CSalThread {
public:
 virtual ~CSalThread();
 bool Create(LPTHREAD_START_ROUTINE,void*,SECURITY_ATTRIBUTES*,unsigned int,unsigned int);
 void SetPriority(int priority){SetThreadPriority(m_handle,priority);}
private:HANDLE m_handle;DWORD m_threadId;
};
class CSalMutex {
public:virtual ~CSalMutex();
private:CRITICAL_SECTION m_critSection;bool m_bActive;
};
class CSalOutput:public CCodaObjectBase {
public:
 virtual ~CSalOutput();
 virtual bool Create(const GUID*,HWND,bool)=0;
 virtual void Release()=0;
 virtual bool CreateBuffer(unsigned int,unsigned int)=0;
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
#endif
