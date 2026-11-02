#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long
#else
#define __declspec(x)
#endif
static int test_stage=0;
static void check(bool condition) { if(!condition) { fprintf(stderr,"check failed at stage %d\n",test_stage);exit(1); } }
#ifndef FABLE_PASS11_CBASEVIDEORENDERER_H
#define FABLE_PASS11_CBASEVIDEORENDERER_H
// Layout: DirectShow BaseClasses renbase.h/amfilter.h/combase.h; native base 0xe0, renderer statistics through 0x158.
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;

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
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
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
class CBaseObject {
private:
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static long m_cObjects;
public:
    CBaseObject(const unsigned short*);
    ~CBaseObject();
};
class CUnknown : public INonDelegatingUnknown, public CBaseObject {
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
    virtual void RecordFrameLateness(int trLate,int trFrame);
    virtual void OnDirectRender(IMediaSample*);
    virtual long ResetStreamingTimes();
    virtual long ShouldDrawSampleNow(IMediaSample*,REFERENCE_TIME*,REFERENCE_TIME*);
    virtual long SendQuality(REFERENCE_TIME,REFERENCE_TIME);

};

#endif

// CBaseVideoRenderer::ShouldDrawSampleNow(IMediaSample*, REFERENCE_TIME*, REFERENCE_TIME*) -- schedule, delay or drop a video frame
// Leaf: ?ShouldDrawSampleNow@CBaseVideoRenderer@@UAEJPAUIMediaSample@@PA_J1@Z

typedef long HRESULT;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define E_FAIL ((long)0x80004005L)
#define UNITS 10000000
#define AVGPERIOD 4
#define ASSERT(x) ((void)0)
#define MSR_INTEGER(a,b) ((void)0)
int inline TimeDiff(REFERENCE_TIME rt)
{
    if (rt < - (50 * UNITS)) {
        return -(50 * UNITS);
    } else
    if (rt > 50 * UNITS) {
        return 50 * UNITS;
    } else return (int)rt;
}
void CBaseVideoRenderer::PreparePerformanceData(int trLate, int trFrame)
{
    m_trLate = trLate;
    m_trFrame = trFrame;
}
HRESULT CBaseVideoRenderer::ShouldDrawSampleNow(IMediaSample *pMediaSample,
                                                REFERENCE_TIME *ptrStart,
                                                REFERENCE_TIME *ptrEnd)
{

    // Don't call us unless there's a clock interface to synchronise with
    ASSERT(m_pClock);

    MSR_INTEGER(m_idTimeStamp, (int)((*ptrStart)>>32));   // high order 32 bits
    MSR_INTEGER(m_idTimeStamp, (int)(*ptrStart));         // low order 32 bits

    // We lose a bit of time depending on the monitor type waiting for the next
    // screen refresh.  On average this might be about 8mSec - so it will be
    // later than we think when the picture appears.  To compensate a bit
    // we bias the media samples by -8mSec i.e. 80000 UNITs.
    // We don't ever make a stream time negative (call it paranoia)
    if (*ptrStart>=80000) {
        *ptrStart -= 80000;
        *ptrEnd -= 80000;       // bias stop to to retain valid frame duration
    }

    // Cache the time stamp now.  We will want to compare what we did with what
    // we started with (after making the monitor allowance).
    m_trRememberStampForPerf = *ptrStart;

    // Get reference times (current and late)
    REFERENCE_TIME trRealStream;     // the real time now expressed as stream time.
    m_pClock->GetTime(&trRealStream);
#ifdef PERF
    // While the reference clock is expensive:
    // Remember the offset from timeGetTime and use that.
    // This overflows all over the place, but when we subtract to get
    // differences the overflows all cancel out.
    m_llTimeOffset = trRealStream-timeGetTime()*10000;
#endif
    trRealStream -= m_tStart;     // convert to stream time (this is a reftime)

    // We have to wory about two versions of "lateness".  The truth, which we
    // try to work out here and the one measured against m_trTarget which
    // includes long term feedback.  We report statistics against the truth
    // but for operational decisions we work to the target.
    // We use TimeDiff to make sure we get an integer because we
    // may actually be late (or more likely early if there is a big time
    // gap) by a very long time.
    const int trTrueLate = TimeDiff(trRealStream - *ptrStart);
    const int trLate = trTrueLate;

    MSR_INTEGER(m_idSchLateTime, trTrueLate/10000);

    // Send quality control messages upstream, measured against target
    HRESULT hr = SendQuality(trLate, trRealStream);
    // Note: the filter upstream is allowed to this FAIL meaning "you do it".
    m_bSupplierHandlingQuality = (hr==S_OK);

    // Decision time!  Do we drop, draw when ready or draw immediately?

    const int trDuration = (int)(*ptrEnd - *ptrStart);
    {
        // We need to see if the frame rate of the file has just changed.
        // This would make comparing our previous frame rate with the current
        // frame rate inefficent.  Hang on a moment though.  I've seen files
        // where the frames vary between 33 and 34 mSec so as to average
        // 30fps.  A minor variation like that won't hurt us.
        int t = m_trDuration/32;
        if (  trDuration > m_trDuration+t
           || trDuration < m_trDuration-t
           ) {
            // There's a major variation.  Reset the average frame rate to
            // exactly the current rate to disable decision 9002 for this frame,
            // and remember the new rate.
            m_trFrameAvg = trDuration;
            m_trDuration = trDuration;
        }
    }

    MSR_INTEGER(m_idEarliness, m_trEarliness/10000);
    MSR_INTEGER(m_idRenderAvg, m_trRenderAvg/10000);
    MSR_INTEGER(m_idFrameAvg, m_trFrameAvg/10000);
    MSR_INTEGER(m_idWaitAvg, m_trWaitAvg/10000);
    MSR_INTEGER(m_idDuration, trDuration/10000);

#ifdef PERF
    if (S_OK==pMediaSample->IsDiscontinuity()) {
        MSR_INTEGER(m_idDecision, 9000);
    }
#endif

    // Control the graceful slide back from slow to fast machine mode.
    // After a frame drop accept an early frame and set the earliness to here
    // If this frame is already later than the earliness then slide it to here
    // otherwise do the standard slide (reduce by about 12% per frame).
    // Note: earliness is normally NEGATIVE
    BOOL bJustDroppedFrame
        = (  m_bSupplierHandlingQuality
          //  Can't use the pin sample properties because we might
          //  not be in Receive when we call this
          && (S_OK == pMediaSample->IsDiscontinuity())          // he just dropped one
          )
       || (m_nNormal==-1);                          // we just dropped one


    // Set m_trEarliness (slide back from slow to fast machine mode)
    if (trLate>0) {
        m_trEarliness = 0;   // we are no longer in fast machine mode at all!
    } else if (  (trLate>=m_trEarliness) || bJustDroppedFrame) {
        m_trEarliness = trLate;  // Things have slipped of their own accord
    } else {
        m_trEarliness = m_trEarliness - m_trEarliness/8;  // graceful slide
    }

    // prepare the new wait average - but don't pollute the old one until
    // we have finished with it.
    int trWaitAvg;
    {
        // We never mix in a negative wait.  This causes us to believe in fast machines
        // slightly more.
        int trL = trLate<0 ? -trLate : 0;
        trWaitAvg = (trL + m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;
    }


    int trFrame;
    {
        REFERENCE_TIME tr = trRealStream - m_trLastDraw; // Cd be large - 4 min pause!
        if (tr>10000000) {
            tr = 10000000;   // 1 second - arbitrarily.
        }
        trFrame = int(tr);
    }

    // We will DRAW this frame IF...
    if (
          // ...the time we are spending drawing is a small fraction of the total
          // observed inter-frame time so that dropping it won't help much.
          (3*m_trRenderAvg <= m_trFrameAvg)

         // ...or our supplier is NOT handling things and the next frame would
         // be less timely than this one or our supplier CLAIMS to be handling
         // things, and is now less than a full FOUR frames late.
       || ( m_bSupplierHandlingQuality
          ? (trLate <= trDuration*4)
          : (trLate+trLate < trDuration)
          )

          // ...or we are on average waiting for over eight milliseconds then
          // this may be just a glitch.  Draw it and we'll hope to catch up.
       || (m_trWaitAvg > 80000)

          // ...or we haven't drawn an image for over a second.  We will update
          // the display, which stops the video looking hung.
          // Do this regardless of how late this media sample is.
       || ((trRealStream - m_trLastDraw) > UNITS)

    ) {
        HRESULT Result;

        // We are going to play this frame.  We may want to play it early.
        // We will play it early if we think we are in slow machine mode.
        // If we think we are NOT in slow machine mode, we will still play
        // it early by m_trEarliness as this controls the graceful slide back.
        // and in addition we aim at being m_trTarget late rather than "on time".

        BOOL bPlayASAP = FALSE;

        // we will play it AT ONCE (slow machine mode) if...

            // ...we are playing catch-up
        if ( bJustDroppedFrame) {
            bPlayASAP = TRUE;
            MSR_INTEGER(m_idDecision, 9001);
        }

            // ...or if we are running below the true frame rate
            // exact comparisons are glitchy, for these measurements,
            // so add an extra 5% or so
        else if (  (m_trFrameAvg > trDuration + trDuration/16)

                   // It's possible to get into a state where we are losing ground, but
                   // are a very long way ahead.  To avoid this or recover from it
                   // we refuse to play early by more than 10 frames.
                && (trLate > - trDuration*10)
                ){
            bPlayASAP = TRUE;
            MSR_INTEGER(m_idDecision, 9002);
        }
#if 0
            // ...or if we have been late and are less than one frame early
        else if (  (trLate + trDuration > 0)
                && (m_trWaitAvg<=20000)
                ) {
            bPlayASAP = TRUE;
            MSR_INTEGER(m_idDecision, 9003);
        }
#endif
        // We will NOT play it at once if we are grossly early.  On very slow frame
        // rate movies - e.g. clock.avi - it is not a good idea to leap ahead just
        // because we got starved (for instance by the net) and dropped one frame
        // some time or other.  If we are more than 900mSec early, then wait.
        if (trLate<-9000000) {
            bPlayASAP = FALSE;
        }

        if (bPlayASAP) {

            m_nNormal = 0;
            MSR_INTEGER(m_idDecision, 0);
            // When we are here, we are in slow-machine mode.  trLate may well
            // oscillate between negative and positive when the supplier is
            // dropping frames to keep sync.  We should not let that mislead
            // us into thinking that we have as much as zero spare time!
            // We just update with a zero wait.
            m_trWaitAvg = (m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;

            // Assume that we draw it immediately.  Update inter-frame stats
            m_trFrameAvg = (trFrame + m_trFrameAvg*(AVGPERIOD-1))/AVGPERIOD;
#ifndef PERF
            // If this is NOT a perf build, then report what we know so far
            // without looking at the clock any more.  This assumes that we
            // actually wait for exactly the time we hope to.  It also reports
            // how close we get to the manipulated time stamps that we now have
            // rather than the ones we originally started with.  It will
            // therefore be a little optimistic.  However it's fast.
            PreparePerformanceData(trTrueLate, trFrame);
#endif
            m_trLastDraw = trRealStream;
            if (m_trEarliness > trLate) {
                m_trEarliness = trLate;  // if we are actually early, this is neg
            }
            Result = S_OK;                   // Draw it now

        } else {
            ++m_nNormal;
            // Set the average frame rate to EXACTLY the ideal rate.
            // If we are exiting slow-machine mode then we will have caught up
            // and be running ahead, so as we slide back to exact timing we will
            // have a longer than usual gap at this point.  If we record this
            // real gap then we'll think that we're running slow and go back
            // into slow-machine mode and vever get it straight.
            m_trFrameAvg = trDuration;
            MSR_INTEGER(m_idDecision, 1);

            // Play it early by m_trEarliness and by m_trTarget

            {
                int trE = m_trEarliness;
                if (trE < -m_trFrameAvg) {
                    trE = -m_trFrameAvg;
                }
                *ptrStart += trE;           // N.B. earliness is negative
            }

            int Delay = -trTrueLate;
            Result = Delay<=0 ? S_OK : S_FALSE;     // OK = draw now, FALSE = wait

            m_trWaitAvg = trWaitAvg;

            // Predict when it will actually be drawn and update frame stats

            if (Result==S_FALSE) {   // We are going to wait
                trFrame = TimeDiff(*ptrStart-m_trLastDraw);
                m_trLastDraw = *ptrStart;
            } else {
                // trFrame is already = trRealStream-m_trLastDraw;
                m_trLastDraw = trRealStream;
            }
#ifndef PERF
            int iAccuracy;
            if (Delay>0) {
                // Report lateness based on when we intend to play it
                iAccuracy = TimeDiff(*ptrStart-m_trRememberStampForPerf);
            } else {
                // Report lateness based on playing it *now*.
                iAccuracy = trTrueLate;     // trRealStream-RememberStampForPerf;
            }
            PreparePerformanceData(iAccuracy, trFrame);
#endif
        }
        return Result;
    }

    // We are going to drop this frame!
    // Of course in DirectDraw mode the guy upstream may draw it anyway.

    // This will probably give a large negative wack to the wait avg.
    m_trWaitAvg = trWaitAvg;

#ifdef PERF
    // Respect registry setting - debug only!
    if (m_bDrawLateFrames) {
       return S_OK;                        // draw it when it's ready
    }                                      // even though it's late.
#endif

    // We are going to drop this frame so draw the next one early
    // n.b. if the supplier is doing direct draw then he may draw it anyway
    // but he's doing something funny to arrive here in that case.

    MSR_INTEGER(m_idDecision, 2);
    m_nNormal = -1;
    return E_FAIL;                         // drop it

}


CBaseObject::CBaseObject(const unsigned short*) { }
CBaseObject::~CBaseObject() { }
CUnknown::CUnknown(const unsigned short* n,IUnknown* u) : CBaseObject(n),m_pUnknown(u),m_cRef(1) { }
CUnknown::~CUnknown() { }
CBaseFilter::CBaseFilter(const unsigned short* n,IUnknown* u,CCritSec*,const GUID&) : CUnknown(n,u) { }
CBaseFilter::~CBaseFilter() { }
CBaseRenderer::CBaseRenderer(const GUID& g,const unsigned short* n,IUnknown* u,long*) : CBaseFilter(n,u,0,g) { }
CBaseRenderer::~CBaseRenderer() { }
CBaseVideoRenderer::CBaseVideoRenderer(const GUID& g,const unsigned short* n,IUnknown* u,long* p) : CBaseRenderer(g,n,u,p) { }
CBaseVideoRenderer::~CBaseVideoRenderer() { }
unsigned long __stdcall CUnknown::NonDelegatingAddRef() { return 0; }
unsigned long __stdcall CUnknown::NonDelegatingRelease() { return 0; }
long __stdcall CUnknown::NonDelegatingQueryInterface(const GUID& riid, void** ppv) { return 0; }
long __stdcall CBaseFilter::QueryInterface(const GUID& riid, void** ppv) { return 0; }
unsigned long __stdcall CBaseFilter::AddRef() { return 0; }
unsigned long __stdcall CBaseFilter::Release() { return 0; }
long __stdcall CBaseFilter::NonDelegatingQueryInterface(const GUID& riid, void** ppv) { return 0; }
long __stdcall CBaseFilter::JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName) { return 0; }
long CBaseFilter::StreamTime(CRefTime& rtStream) { return 0; }
long CBaseFilter::GetPinVersion() { return 0; }
AMOVIESETUP_FILTER* CBaseFilter::GetSetupData() { return 0; }
long CBaseFilter::NotifyEvent(long EventCode, long EventParam1, long EventParam2) { return 0; }
long CBaseRenderer::GetMediaPositionInterface(const GUID& riid, void** ppv) { return 0; }
long __stdcall CBaseRenderer::NonDelegatingQueryInterface(const GUID& riid, void** ppv) { return 0; }
long CBaseRenderer::SourceThreadCanWait(int bCanWait) { return 0; }
long CBaseRenderer::WaitForRenderTime() { return 0; }
long CBaseRenderer::CompleteStateChange(long OldState) { return 0; }
void CBaseRenderer::OnReceiveFirstSample(IMediaSample* pMediaSample) {  }
void CBaseRenderer::SetRepaintStatus(int bRepaint) {  }
void CBaseRenderer::OnRenderStart(IMediaSample* pMediaSample) {  }
void CBaseRenderer::OnRenderEnd(IMediaSample* pMediaSample) {  }
long CBaseRenderer::OnStartStreaming() { return 0; }
long CBaseRenderer::OnStopStreaming() { return 0; }
void CBaseRenderer::OnWaitStart() {  }
void CBaseRenderer::OnWaitEnd() {  }
void CBaseRenderer::PrepareRender() {  }
int CBaseRenderer::ScheduleSample(IMediaSample* pMediaSample) { return 0; }
long CBaseRenderer::GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime,
                                REFERENCE_TIME* pEndTime) { return 0; }
long CBaseRenderer::ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart,
                                     REFERENCE_TIME* ptrEnd) { return 0; }
long CBaseRenderer::NotifyEndOfStream() { return 0; }
long CBaseRenderer::SendEndOfStream() { return 0; }
long CBaseRenderer::ResetEndOfStream() { return 0; }
long CBaseRenderer::EndOfStream() { return 0; }
long CBaseRenderer::CancelNotification() { return 0; }
long CBaseRenderer::ClearPendingSample() { return 0; }
long CBaseRenderer::Active() { return 0; }
long CBaseRenderer::Inactive() { return 0; }
long CBaseRenderer::StartStreaming() { return 0; }
long CBaseRenderer::StopStreaming() { return 0; }
long CBaseRenderer::BeginFlush() { return 0; }
long CBaseRenderer::EndFlush() { return 0; }
long CBaseRenderer::BreakConnect() { return 0; }
long CBaseRenderer::SetMediaType(const CMediaType* pmt) { return 0; }
long CBaseRenderer::CompleteConnect(IPin* pReceivePin) { return 0; }
long CBaseRenderer::PrepareReceive(IMediaSample* pMediaSample) { return 0; }
long CBaseRenderer::Receive(IMediaSample* pMediaSample) { return 0; }
int CBaseRenderer::HaveCurrentSample() { return 0; }
IMediaSample* CBaseRenderer::GetCurrentSample() { return 0; }
long CBaseRenderer::Render(IMediaSample* pMediaSample) { return 0; }
void CBaseRenderer::WaitForReceiveToComplete() {  }
void CBaseVideoRenderer::RecordFrameLateness(int trLate,int trFrame) {  }
void CBaseVideoRenderer::OnDirectRender(IMediaSample*) {  }
long CBaseVideoRenderer::ResetStreamingTimes() { return 0; }
long CBaseVideoRenderer::SendQuality(REFERENCE_TIME,REFERENCE_TIME) { return 0; }

class CTestClock : public IReferenceClock {
public:
 REFERENCE_TIME Now;int Calls;
 CTestClock() : Now(0),Calls(0) { }
 long __stdcall QueryInterface(const GUID&,void**) { return E_FAIL; }
 unsigned long __stdcall AddRef() { return 1; }
 unsigned long __stdcall Release() { return 1; }
 long __stdcall GetTime(REFERENCE_TIME* out) { ++Calls;*out=Now;return S_OK; }
 long __stdcall AdviseTime(REFERENCE_TIME,REFERENCE_TIME,unsigned long,unsigned long*) { return E_FAIL; }
 long __stdcall AdvisePeriodic(REFERENCE_TIME,REFERENCE_TIME,unsigned long,unsigned long*) { return E_FAIL; }
 long __stdcall Unadvise(unsigned long) { return E_FAIL; }
};
class CTestSample : public IMediaSample {
public:
 bool Discontinuity;int Calls;
 CTestSample() : Discontinuity(false),Calls(0) { }
 long __stdcall QueryInterface(const GUID&,void**) { return E_FAIL; }
 unsigned long __stdcall AddRef() { return 1; }
 unsigned long __stdcall Release() { return 1; }
 long __stdcall GetPointer(unsigned char**) { return E_FAIL; }
 long __stdcall GetSize() { return 0; }
 long __stdcall GetTime(REFERENCE_TIME*,REFERENCE_TIME*) { return E_FAIL; }
 long __stdcall SetTime(REFERENCE_TIME*,REFERENCE_TIME*) { return E_FAIL; }
 long __stdcall IsSyncPoint() { return S_OK; }
 long __stdcall SetSyncPoint(int) { return S_OK; }
 long __stdcall IsPreroll() { return S_FALSE; }
 long __stdcall SetPreroll(int) { return S_OK; }
 long __stdcall GetActualDataLength() { return 0; }
 long __stdcall SetActualDataLength(long) { return S_OK; }
 long __stdcall GetMediaType(_AMMediaType**) { return E_FAIL; }
 long __stdcall SetMediaType(_AMMediaType*) { return S_OK; }
 long __stdcall IsDiscontinuity() { ++Calls;return Discontinuity?S_OK:S_FALSE; }
 long __stdcall SetDiscontinuity(int) { return S_OK; }
 long __stdcall GetMediaTime(LONGLONG*,LONGLONG*) { return E_FAIL; }
 long __stdcall SetMediaTime(LONGLONG*,LONGLONG*) { return S_OK; }
};
static const GUID zero_guid={0,0,0,{0,0,0,0,0,0,0,0}};
class CTestRenderer : public CBaseVideoRenderer {
public:
 long QualityResult;int QualityCalls;REFERENCE_TIME QualityLate,QualityNow;
 CTestRenderer() : CBaseVideoRenderer(zero_guid,0,0,0) { }
 // Remaining pure interfaces are unrelated to scheduling, and deliberately fail.
 long __stdcall QueryInterface(const GUID&,void**) { return E_FAIL; }
 unsigned long __stdcall AddRef() { return 1; }
 unsigned long __stdcall Release() { return 1; }
 long __stdcall GetClassID(GUID*) { return E_FAIL; }
 long __stdcall Stop() { return E_FAIL; }
 long __stdcall Pause() { return E_FAIL; }
 long __stdcall Run(REFERENCE_TIME) { return E_FAIL; }
 long __stdcall GetState(unsigned long,FILTER_STATE*) { return E_FAIL; }
 long __stdcall SetSyncSource(IReferenceClock*) { return E_FAIL; }
 long __stdcall GetSyncSource(IReferenceClock**) { return E_FAIL; }
 long __stdcall EnumPins(IEnumPins**) { return E_FAIL; }
 long __stdcall FindPin(const unsigned short*,IPin**) { return E_FAIL; }
 long __stdcall QueryFilterInfo(FILTER_INFO*) { return E_FAIL; }
 long __stdcall QueryVendorInfo(unsigned short**) { return E_FAIL; }
 long __stdcall Register() { return E_FAIL; }
 long __stdcall Unregister() { return E_FAIL; }
 int GetPinCount() { return 0; }
 CBasePin* GetPin(int) { return 0; }
 long DoRenderSample(IMediaSample*) { return E_FAIL; }
 long CheckMediaType(const CMediaType*) { return E_FAIL; }
 long __stdcall get_FramesDroppedInRenderer(int*) { return E_FAIL; }
 long __stdcall get_FramesDrawn(int*) { return E_FAIL; }
 long __stdcall get_AvgFrameRate(int*) { return E_FAIL; }
 long __stdcall get_Jitter(int*) { return E_FAIL; }
 long __stdcall get_AvgSyncOffset(int*) { return E_FAIL; }
 long __stdcall get_DevSyncOffset(int*) { return E_FAIL; }
 long __stdcall Notify(IBaseFilter*,Quality) { return E_FAIL; }
 long __stdcall SetSink(IQualityControl*) { return E_FAIL; }
 long SendQuality(REFERENCE_TIME late,REFERENCE_TIME now) {
  ++QualityCalls;QualityLate=late;QualityNow=now;return QualityResult;
 }
 void Reset(CTestClock& clock) {
  m_pClock=&clock;m_tStart=0;m_nNormal=3;m_bSupplierHandlingQuality=0;
  m_trEarliness=-80000;m_trWaitAvg=0;m_trRenderAvg=100000;m_trFrameAvg=400000;
  m_trDuration=400000;m_trLastDraw=clock.Now-400000;m_trLate=123;m_trFrame=456;
  m_trRememberStampForPerf=0;m_trTarget=0;QualityResult=E_FAIL;QualityCalls=0;
  clock.Calls=0;
 }
 void Configure(int normal,int render,int waiting,REFERENCE_TIME last) {
  m_nNormal=normal;m_trRenderAvg=render;m_trWaitAvg=waiting;m_trLastDraw=last;
 }
 void Verify(int normal,int early,int wait,int average,REFERENCE_TIME last,int late,int frame) {
  check(m_nNormal==normal);check(m_trEarliness==early);check(m_trWaitAvg==wait);
  check(m_trFrameAvg==average);check(m_trLastDraw==last);check(m_trLate==late);check(m_trFrame==frame);
 }
 void VerifyLayout() {
#ifdef _MSC_VER
  check(sizeof(CBaseVideoRenderer)==0x160);
  check((char*)&m_pClock-(char*)this==0x18);
  check((char*)&m_nNormal-(char*)this==0xe8);
  check((char*)&m_trRememberStampForPerf-(char*)this==0x118);
  check((char*)&m_trFrame-(char*)this==0x154);
#endif
 }
 REFERENCE_TIME Remembered() const { return m_trRememberStampForPerf; }
};
int main() {
 CTestClock clock;CTestSample sample;CTestRenderer r;r.VerifyLayout();
 REFERENCE_TIME start,end;long result;
 test_stage=1;clock.Now=10000000;r.Reset(clock);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK&&start==9920000&&end==10320000);
 r.Verify(4,0,0,400000,10000000,80000,400000);
 check(r.QualityLate==80000&&r.QualityNow==10000000&&r.QualityCalls==1&&clock.Calls==1&&sample.Calls==0);
 test_stage=2;clock.Now=9500000;r.Reset(clock);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_FALSE&&start==9850000&&end==10320000&&r.Remembered()==9920000);
 r.Verify(4,-70000,105000,400000,9850000,-70000,750000);
 test_stage=3;clock.Now=9500000;r.Reset(clock);r.Configure(-1,100000,8000,9100000);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK&&start==9920000);r.Verify(0,-420000,6000,400000,9500000,-420000,400000);
 test_stage=4;clock.Now=10420000;r.Reset(clock);r.Configure(3,200000,0,10020000);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==E_FAIL&&start==9920000);r.Verify(-1,0,0,400000,10020000,123,456);
 test_stage=5;r.Reset(clock);r.Configure(3,200000,0,10020000);r.QualityResult=S_OK;sample.Discontinuity=false;sample.Calls=0;start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK&&sample.Calls==1);r.Verify(4,0,0,400000,10420000,500000,400000);
 test_stage=6;r.Reset(clock);r.QualityResult=S_OK;sample.Discontinuity=true;sample.Calls=0;start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK&&sample.Calls==1);r.Verify(0,0,0,400000,10420000,500000,400000);
 test_stage=7;clock.Now=11920000;r.Reset(clock);r.Configure(3,200000,80001,11520000);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK);r.Verify(4,0,60000,400000,11920000,2000000,400000);
 test_stage=8;r.Reset(clock);r.Configure(3,200000,0,0);start=10000000;end=10400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_OK);r.Verify(4,0,0,400000,11920000,2000000,10000000);
 test_stage=9;clock.Now=1000000;r.Reset(clock);r.Configure(-1,100000,0,600000);start=20000000;end=20400000;
 result=r.ShouldDrawSampleNow(&sample,&start,&end);
 check(result==S_FALSE&&start==19520000);r.Verify(0,-18920000,4730000,400000,19520000,-400000,18920000);
 test_stage=10;clock.Now=79999;r.Reset(clock);start=79999;end=479999;
 check(r.ShouldDrawSampleNow(&sample,&start,&end)==S_OK&&start==79999&&end==479999);
 test_stage=11;clock.Now=0;r.Reset(clock);start=80000;end=480000;
 check(r.ShouldDrawSampleNow(&sample,&start,&end)==S_OK&&start==0&&end==400000);
 test_stage=12;clock.Now=1000000000;r.Reset(clock);r.Configure(3,200000,0,999600000);start=10000000;end=10400000;
 check(r.ShouldDrawSampleNow(&sample,&start,&end)==E_FAIL&&r.QualityLate==500000000);
 test_stage=13;clock.Now=-1000000000;r.Reset(clock);start=10000000;end=10400000;
 check(r.ShouldDrawSampleNow(&sample,&start,&end)==S_FALSE&&r.QualityLate==-500000000);
 test_stage=14;clock.Now=10000000;r.Reset(clock);r.Configure(3,200000,0,9600000);start=10000000;end=10800000;
 check(r.ShouldDrawSampleNow(&sample,&start,&end)==S_OK);
 r.Verify(4,0,0,800000,10000000,80000,400000);
 printf("00ca5850_TEST PASS\n");return 0;
}