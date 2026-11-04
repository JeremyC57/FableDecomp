#ifndef FABLE_PASS11_CBASEVIDEORENDERER_H
#define FABLE_PASS11_CBASEVIDEORENDERER_H
// Layout: DirectShow BaseClasses renbase.h/amfilter.h/combase.h; native base 0xe0, renderer statistics through 0x158.
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER
// Retail opt-in (movie row 0x006286F0): GUID and CRITICAL_SECTION (with Enter/LeaveCriticalSection) come from the one
// shared SDK shim, so this model can share a TU with engine/CSystemManager.h and engine/CVideoSys.h.
#include "engine/Win32Types.h"
#else
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
#endif
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
typedef __int64 REFERENCE_TIME;
typedef __int64 LONGLONG;
#define NOERROR 0L
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((long)0x80004003L)
#define E_OUTOFMEMORY ((long)0x8007000EL)
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;   // 0
    virtual unsigned long __stdcall AddRef() = 0;                              // 1 (+0x04)
    virtual unsigned long __stdcall Release() = 0;                             // 2 (+0x08)
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
#ifndef FABLE_RETAIL_CBASEVIDEORENDERER
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
#endif
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                              // IAT 0x0143FDE0
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);                            // IAT 0x0143FED4
class CCritSec {                                 // wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                // wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CAMEvent {                                 // wxutil.h (holds only the HANDLE)
    void* m_hEvent;
public:
    operator void*() const { return m_hEvent; }
    void Set() { SetEvent(m_hEvent); }
    void Reset() { ResetEvent(m_hEvent); }
};
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER
// Fable's strmbase renames combase.h's CBaseObject to CMyBaseObject (RTTI of vtable 0x0129D08C: CUnknown's bases are
// INonDelegatingUnknown and CMyBaseObject; GOTCHAS chunk 715), which also keeps it apart from the engine's CBaseObject
// (engine/CBaseObject.h) in a shared TU.
class CMyBaseObject {
private:
    CMyBaseObject(const CMyBaseObject&);
    void operator=(const CMyBaseObject&);
    static long m_cObjects;
public:
    CMyBaseObject(const unsigned short*);
    ~CMyBaseObject();
};
#else
class CBaseObject {
private:
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static long m_cObjects;
public:
    CBaseObject(const unsigned short*);
    ~CBaseObject();
};
#endif
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER
class CUnknown : public INonDelegatingUnknown, public CMyBaseObject {
#else
class CUnknown : public INonDelegatingUnknown, public CBaseObject {
#endif
private:
    IUnknown* const m_pUnknown;                        // +0x04 (const LPUNKNOWN)
protected:
    long m_cRef;                                 // +0x08
public:
    CUnknown(const unsigned short*, IUnknown*);
    unsigned long __stdcall NonDelegatingAddRef();
    unsigned long __stdcall NonDelegatingRelease();
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CACCB0
    virtual ~CUnknown();                 // primary slot 3 (the virtual destructor)
};
class CRefTime {                                 // reftime.h: exactly one REFERENCE_TIME, no virtuals
public:
    REFERENCE_TIME m_time;
    inline CRefTime() { m_time = 0; }
    inline CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    inline operator REFERENCE_TIME() const { return m_time; }
    inline CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    inline CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
};
struct IReferenceClock : IUnknown {
    virtual long __stdcall GetTime(REFERENCE_TIME* pTime) = 0;                                  // 3 (+0x0c)
    virtual long __stdcall AdviseTime(REFERENCE_TIME baseTime, REFERENCE_TIME streamTime,
                                      unsigned long hEvent, unsigned long* pdwAdviseCookie) = 0;
    virtual long __stdcall AdvisePeriodic(REFERENCE_TIME,REFERENCE_TIME,unsigned long,unsigned long*) = 0;
    virtual long __stdcall Unadvise(unsigned long) = 0;
};
struct IPin;
struct IQualityControl;
struct IEnumPins;
struct IFilterGraph;
struct IMediaEventSink;
struct _AMMediaType;
struct IMediaSample : IUnknown {
 virtual long __stdcall GetPointer(unsigned char**) = 0;
 virtual long __stdcall GetSize() = 0;
 virtual long __stdcall GetTime(REFERENCE_TIME*,REFERENCE_TIME*) = 0;
 virtual long __stdcall SetTime(REFERENCE_TIME*,REFERENCE_TIME*) = 0;
 virtual long __stdcall IsSyncPoint() = 0;
 virtual long __stdcall SetSyncPoint(int) = 0;
 virtual long __stdcall IsPreroll() = 0;
 virtual long __stdcall SetPreroll(int) = 0;
 virtual long __stdcall GetActualDataLength() = 0;
 virtual long __stdcall SetActualDataLength(long) = 0;
 virtual long __stdcall GetMediaType(_AMMediaType**) = 0;
 virtual long __stdcall SetMediaType(_AMMediaType*) = 0;
 virtual long __stdcall IsDiscontinuity() = 0;
 virtual long __stdcall SetDiscontinuity(int) = 0;
 virtual long __stdcall GetMediaTime(LONGLONG*,LONGLONG*) = 0;
 virtual long __stdcall SetMediaTime(LONGLONG*,LONGLONG*) = 0;
};
struct FILTER_INFO;
struct AMOVIESETUP_FILTER;
class CBasePin;
class CMediaType;
struct IPersist : IUnknown {
    virtual long __stdcall GetClassID(GUID* clsid) = 0;                        // 3
};
enum FILTER_STATE { State_Stopped, State_Paused, State_Running };
struct IMediaFilter : IPersist {
    virtual long __stdcall Stop() = 0;                                         // 4
    virtual long __stdcall Pause() = 0;                                        // 5
    virtual long __stdcall Run(REFERENCE_TIME start) = 0;                      // 6
    virtual long __stdcall GetState(unsigned long dwMSecs, FILTER_STATE* State) = 0;   // 7
    virtual long __stdcall SetSyncSource(IReferenceClock* clock) = 0;          // 8
    virtual long __stdcall GetSyncSource(IReferenceClock** clock) = 0;         // 9
};
struct IBaseFilter : IMediaFilter {
    virtual long __stdcall EnumPins(IEnumPins** ppEnum) = 0;                   // 10
    virtual long __stdcall FindPin(const unsigned short* Id, IPin** ppPin) = 0;   // 11
    virtual long __stdcall QueryFilterInfo(FILTER_INFO* pInfo) = 0;            // 12
    virtual long __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName) = 0;   // 13
    virtual long __stdcall QueryVendorInfo(unsigned short** pVendorInfo) = 0;  // 14
};
struct IAMovieSetup : IUnknown {
    virtual long __stdcall Register() = 0;       // 3
    virtual long __stdcall Unregister() = 0;     // 4
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // amfilter.h: IBaseFilter +0x0C, IAMovieSetup +0x10
protected:
    FILTER_STATE m_State;                                // +0x14 FILTER_STATE (0 Stopped, 1 Paused, 2 Running)
    IReferenceClock* m_pClock;                   // +0x18
    CRefTime m_tStart;                           // +0x20
    GUID m_clsid;                                // +0x28
    CCritSec* m_pLock;                           // +0x38
    unsigned short* m_pName;                     // +0x3c WCHAR*
    IFilterGraph* m_pGraph;                      // +0x40
    IMediaEventSink* m_pSink;                    // +0x44
    long m_PinVersion;                           // +0x48
public:
    CBaseFilter(const unsigned short*,IUnknown*,CCritSec*,const GUID&);
    ~CBaseFilter();
    // DECLARE_IUNKNOWN
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CA7500
    long __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName);   // 0x00CA7850
    virtual long StreamTime(CRefTime& rtStream);         // primary slot 4
    virtual long GetPinVersion();                        // 5
    virtual int GetPinCount() = 0;                       // 6
    virtual CBasePin* GetPin(int n) = 0;                 // 7 (+0x1c)
    virtual AMOVIESETUP_FILTER* GetSetupData();          // 8
    long NotifyEvent(long EventCode, long EventParam1, long EventParam2);   // 0x00CA7930
};
class CRendererInputPin;
class CRendererPosPassThru;
class CBaseRenderer : public CBaseFilter {      // renbase.h; 0xE0 bytes
protected:
    CRendererPosPassThru* m_pPosition;                           // +0x50 CRendererPosPassThru*
    CAMEvent m_RenderEvent;                      // +0x54
    CAMEvent m_ThreadSignal;                     // +0x58
    CAMEvent m_evComplete;                       // +0x5c
    int m_bAbort;                                // +0x60
    int m_bStreaming;                            // +0x64
    unsigned long m_dwAdvise;                    // +0x68 DWORD_PTR
    IMediaSample* m_pMediaSample;                // +0x6c
    int m_bEOS;                                  // +0x70
    int m_bEOSDelivered;                         // +0x74
    CRendererInputPin* m_pInputPin;              // +0x78
    CCritSec m_InterfaceLock;                    // +0x7c
    CCritSec m_RendererLock;                     // +0x94
    IQualityControl* m_pQSink;                   // +0xac
    int m_bRepaintStatus;                        // +0xb0
    int m_bInReceive;                            // +0xb4 (cv-qualified BOOL in renbase.h; modelled plain)
    REFERENCE_TIME m_SignalTime;                 // +0xb8
    unsigned int m_EndOfStreamTimer;             // +0xc0
    CCritSec m_ObjectCreationLock;               // +0xc4

public:
    CBaseRenderer(const GUID&,const unsigned short*,IUnknown*,long*);
    ~CBaseRenderer();
    // primary-vtable virtuals in renbase.h declaration order (slots 9..44)
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES   // vendor naming V1: strmbase override declarations
    int GetPinCount();                                                      // 6 (+0x18) 0x00CA48F0 (renbase.h override)
    CBasePin* GetPin(int n);                                                // 7 (+0x1c) 0x00CA6A30 (renbase.h override)
#endif
    virtual long GetMediaPositionInterface(const GUID& riid, void** ppv);   // 9 (+0x24)
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
    virtual long SourceThreadCanWait(int bCanWait);                         // 10 (+0x28)
    virtual long WaitForRenderTime();                                       // 11 (+0x2c)
    virtual long CompleteStateChange(long OldState);                        // 12 (+0x30)
    int IsEndOfStream() { return m_bEOS; }
    virtual void OnReceiveFirstSample(IMediaSample* pMediaSample);          // 13 (+0x34)
    void Ready() { m_evComplete.Set(); }
    void NotReady() { m_evComplete.Reset(); }
    long GetRealState() { return m_State; }
    void SetRepaintStatus(int bRepaint);                                    // 0x00CA4EA0
    virtual void OnRenderStart(IMediaSample* pMediaSample);                 // 14 (+0x38)
    virtual void OnRenderEnd(IMediaSample* pMediaSample);                   // 15 (+0x3c)
    virtual long OnStartStreaming();                                        // 16 (+0x40)
    virtual long OnStopStreaming();                                         // 17 (+0x44)
    virtual void OnWaitStart();                                             // 18 (+0x48)
    virtual void OnWaitEnd();                                               // 19 (+0x4c)
    virtual void PrepareRender();                                           // 20 (+0x50)
    virtual int ScheduleSample(IMediaSample* pMediaSample);                 // 21 (+0x54)
    virtual long GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime,
                                REFERENCE_TIME* pEndTime);                  // 22 (+0x58)
    virtual long ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart,
                                     REFERENCE_TIME* ptrEnd);               // 23 (+0x5c)
    long NotifyEndOfStream();                                               // 0x00CA4CC0
    virtual long SendEndOfStream();                                         // 24 (+0x60)
    virtual long ResetEndOfStream();                                        // 25 (+0x64)
    virtual long EndOfStream();                                             // 26 (+0x68)
    void SignalTimerFired() { m_dwAdvise = 0; }
    virtual long CancelNotification();                                      // 27 (+0x6c)
    virtual long ClearPendingSample();                                      // 28 (+0x70)
    virtual long Active();                                                  // 29 (+0x74)
    virtual long Inactive();                                                // 30 (+0x78)
    virtual long StartStreaming();                                          // 31 (+0x7c)
    virtual long StopStreaming();                                           // 32 (+0x80)
    virtual long BeginFlush();                                              // 33 (+0x84)
    virtual long EndFlush();                                                // 34 (+0x88)
    virtual long BreakConnect();                                            // 35 (+0x8c)
    virtual long SetMediaType(const CMediaType* pmt);                       // 36 (+0x90)
    virtual long CompleteConnect(IPin* pReceivePin);                        // 37 (+0x94)
    virtual long PrepareReceive(IMediaSample* pMediaSample);                // 38 (+0x98)
    virtual long Receive(IMediaSample* pMediaSample);                       // 39 (+0x9c)
    virtual int HaveCurrentSample();                                        // 40 (+0xa0)
    virtual IMediaSample* GetCurrentSample();                               // 41 (+0xa4)
    virtual long Render(IMediaSample* pMediaSample);                        // 42 (+0xa8)
    virtual long DoRenderSample(IMediaSample* pMediaSample) = 0;            // 43 (+0xac)
    virtual long CheckMediaType(const CMediaType* pmt) = 0;                 // 44 (+0xb0)
    void WaitForReceiveToComplete();                                        // 0x00CA47D0
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES   // vendor naming V1: strmbase override declarations
    void ResetEndOfStreamTimer();                                           // renbase.cpp (inlined into ResetEndOfStream 0x00CA7030)
#endif
};
enum QualityMessageType { Famine, Flood };
struct tagQuality { QualityMessageType Type; long Proportion; REFERENCE_TIME Late; REFERENCE_TIME TimeStamp; };
typedef tagQuality Quality;
struct IQualityControl : IUnknown {
    virtual long __stdcall Notify(IBaseFilter* pSelf, Quality q) = 0;          // 3
    virtual long __stdcall SetSink(IQualityControl* piqc) = 0;                 // 4
};
struct IQualProp : IUnknown {
    virtual long __stdcall get_FramesDroppedInRenderer(int* pcFrames) = 0;   // 3
    virtual long __stdcall get_FramesDrawn(int* pcFramesDrawn) = 0;          // 4
    virtual long __stdcall get_AvgFrameRate(int* piAvgFrameRate) = 0;        // 5
    virtual long __stdcall get_Jitter(int* iJitter) = 0;                     // 6
    virtual long __stdcall get_AvgSyncOffset(int* piAvg) = 0;                // 7
    virtual long __stdcall get_DevSyncOffset(int* piDev) = 0;                // 8
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {   // renbase.h: IQualProp +0xe0, IQualityControl +0xe4
protected:
    int m_nNormal;                               // +0xe8
    int m_bSupplierHandlingQuality;              // +0xec
    int m_trThrottle;                            // +0xf0
    int m_trRenderAvg;                           // +0xf4
    int m_trRenderLast;                          // +0xf8
    int m_tRenderStart;                          // +0xfc
    int m_trEarliness;                           // +0x100
    int m_trTarget;                              // +0x104
    int m_trWaitAvg;                             // +0x108
    int m_trFrameAvg;                            // +0x10c
    int m_trDuration;                            // +0x110
    REFERENCE_TIME m_trRememberStampForPerf;     // +0x118
    int m_cFramesDropped;                        // +0x120
    int m_cFramesDrawn;                          // +0x124
    LONGLONG m_iTotAcc;                          // +0x128
    LONGLONG m_iSumSqAcc;                        // +0x130
    REFERENCE_TIME m_trLastDraw;                 // +0x138
    LONGLONG m_iSumSqFrameTime;                  // +0x140
    LONGLONG m_iSumFrameTime;                    // +0x148
    int m_trLate;                                // +0x150
    int m_trFrame;                               // +0x154
    int m_tStreamingStart;                       // +0x158
public:
    CBaseVideoRenderer(const GUID&,const unsigned short*,IUnknown*,long*);
    ~CBaseVideoRenderer();
    void PreparePerformanceData(int trLate,int trFrame);
#ifdef FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES   // vendor naming V1: strmbase override declarations
    // renbase.h CBaseVideoRenderer overrides of CBaseRenderer slots (vtable 0x012BF5D8; vendor naming wave V1)
    void OnRenderStart(IMediaSample* pMediaSample);                         // 14 0x00CA55E0
    void OnRenderEnd(IMediaSample* pMediaSample);                           // 15 0x00CA5610
    void OnWaitStart();                                                     // 18 0x00CA53A0
    void OnWaitEnd();                                                       // 19 0x00CA53B0
    long OnStartStreaming();                                                // 16 0x00CA5360
    long OnStopStreaming();                                                 // 17 0x00CA5380
    int ScheduleSample(IMediaSample* pMediaSample);                         // 21 0x00CA5CF0
#endif
    virtual void RecordFrameLateness(int trLate,int trFrame);
    virtual void OnDirectRender(IMediaSample*);
    virtual long ResetStreamingTimes();
    virtual long ShouldDrawSampleNow(IMediaSample*,REFERENCE_TIME*,REFERENCE_TIME*);
    virtual long SendQuality(REFERENCE_TIME,REFERENCE_TIME);

};

#endif
