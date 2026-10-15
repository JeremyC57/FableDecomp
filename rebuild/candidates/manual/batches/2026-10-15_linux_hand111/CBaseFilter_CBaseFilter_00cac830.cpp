// CBaseFilter::CBaseFilter(const char* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid) -- DirectShow base-class filter constructor: CUnknown base, stopped state, no clock/graph/sink/name, the lock and CLSID stored, pin version 1
// Leaf: ??0CBaseFilter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC830, length 94 bytes.
// Callees: CUnknown::CUnknown(const char*, IUnknown*) @ 0x00CACC80 (InterlockedIncrement of the object count, m_pUnknown = pUnk ? pUnk : this, m_cRef = 0).
// The catalog label "CThingMap::CThingMap" is propagated; the body is the DirectShow base-classes
// CBaseFilter constructor (amfilter.cpp), called from the filter constructor at 0x00CA61D0, which
// then stores its own vtables. Layout = the SDK class: CUnknown {vptr, m_pUnknown, m_cRef}
// +0..+0xb, IBaseFilter vptr +0x0c, IAMovieSetup vptr +0x10, m_State +0x14, m_pClock +0x18,
// m_tStart (CRefTime, 8-aligned) +0x20, m_clsid +0x28, m_pLock +0x38, m_pName +0x3c, m_pGraph
// +0x40, m_pSink +0x44, m_PinVersion +0x48.
// Shape: AM_NOVTABLE (`__declspec(novtable)`) on the class and on the interfaces removes every
// vtable store; the members are initialised in declaration order from the SDK initialiser list
// (zero register eax), CRefTime's inline constructor zeroes the 64-bit time, the CLSID is a
// struct copy through `lea edx,[esi+28h]` and edi.
#define AM_NOVTABLE __declspec(novtable)
typedef unsigned short WCHAR;                       // wchar_t is not a builtin under the era defaults
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
struct IUnknown;
struct IReferenceClock;
struct IFilterGraph;
struct IMediaEventSink;
struct CCritSec;
enum FILTER_STATE { State_Stopped = 0, State_Paused = 1, State_Running = 2 };
struct AM_NOVTABLE INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
};
class AM_NOVTABLE CUnknown : public INonDelegatingUnknown {
public:
    CUnknown(const char* pName, IUnknown* pUnk);                    // 0x00CACC80
    IUnknown* m_pUnknown;                                            // +0x04
    long      m_cRef;                                                // +0x08
};
struct AM_NOVTABLE IBaseFilter {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
};
struct AM_NOVTABLE IAMovieSetup {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
};
class CRefTime {
public:
    __int64 m_time;
    CRefTime() { m_time = 0; }
};
class AM_NOVTABLE CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {
protected:
    FILTER_STATE     m_State;                                        // +0x14
    IReferenceClock* m_pClock;                                       // +0x18
    CRefTime         m_tStart;                                       // +0x20
    GUID             m_clsid;                                        // +0x28
    CCritSec*        m_pLock;                                        // +0x38
    WCHAR*           m_pName;                                        // +0x3c
    IFilterGraph*    m_pGraph;                                       // +0x40
    IMediaEventSink* m_pSink;                                        // +0x44
    long             m_PinVersion;                                   // +0x48
public:
    CBaseFilter(const char* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid);
};

CBaseFilter::CBaseFilter(const char* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid)
    : CUnknown(pName, pUnk),
      m_pLock(pLock),
      m_clsid(clsid),
      m_State(State_Stopped),
      m_pClock(0),
      m_pGraph(0),
      m_pSink(0),
      m_pName(0),
      m_PinVersion(1)
{
}
