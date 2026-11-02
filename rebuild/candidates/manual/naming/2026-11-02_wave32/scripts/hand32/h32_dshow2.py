# h32_dshow2.py: naming wave 32, family dshow (second row): 0x00CA61D0 is the DX9 SDK strmbase renbase.cpp CBaseRenderer::CBaseRenderer
#  (chunk 704 suggestion, proven here): it is the base call of the proven CBaseVideoRenderer ctor 0x00CA7360 (CBaseVideoRenderer : CBaseRenderer
#  in renbase.h), it stores 0x012BF498 / 0x012BF458 / 0x012BF444, the vtables the landed CBaseFilter::QueryInterface 0x00CA62D0 identifies as
#  CBaseRenderer's (IBaseFilter slots CBaseRenderer::Stop / Pause / Run / GetState / FindPin; primary slot 0 CBaseRenderer::
#  NonDelegatingQueryInterface 0x00CA4780), and its body is the SDK constructor's: CBaseFilter(pName, pUnk, &m_InterfaceLock, RenderClass)
#  0x00CAC830, the init list in renbase.h declaration order (m_ThreadSignal / m_evComplete TRUE, m_bRepaintStatus TRUE), the three CCritSec
#  ctors (InitializeCriticalSection inline) and Ready() = m_evComplete.Set() (SetEvent). The CVolume_00ca61d0 model's vtables
#  ??_7CVolume_00ca61d0@@6B...@@@ become ??_7CBaseRenderer@@6B{CUnknown,IBaseFilter,IAMovieSetup}@@@.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
HIER = open(os.path.join(W, 'runs', 'dshow_hier.txt')).read()
HIER = HIER.replace('''class CAMEvent {
protected:
    void* m_hEvent;                                         // HANDLE
public:
    ~CAMEvent();
};
class CCritSec {
    unsigned char m_CritSec[0x18];                          // CRITICAL_SECTION
public:
    ~CCritSec();
};''', '''class CAMEvent {                                            // wxutil.h (DX9: one-argument ctor)
protected:
    void* m_hEvent;                                         // HANDLE
public:
    CAMEvent(BOOL fManualReset = FALSE);                    // 0x00CB0190
    ~CAMEvent();                                            // 0x00CB01B0
    void Set() { SetEvent(m_hEvent); }                      // EXECUTE_ASSERT(SetEvent(m_hEvent)) in a release build
};
class CCritSec {                                            // wxutil.h
    unsigned char m_CritSec[0x18];                          // CRITICAL_SECTION
public:
    CCritSec() { InitializeCriticalSection(m_CritSec); }
    ~CCritSec() { DeleteCriticalSection(m_CritSec); }
};''')
HIER = HIER.replace('''public:
    ~CBaseFilter();
};''', '''public:
    CBaseFilter(const WCHAR* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid);   // 0x00CAC830
    ~CBaseFilter();
};''')
HIER = HIER.replace('''    CCritSec              m_ObjectCreationLock;             // +0xC4
public:''', '''    CCritSec              m_ObjectCreationLock;             // +0xC4
    void Ready() { m_evComplete.Set(); }
public:''')
assert 'CBaseFilter(const WCHAR*' in HIER and 'void Ready()' in HIER and 'CAMEvent(BOOL' in HIER
he.put('00ca61d0', 'src', '''// CBaseRenderer::CBaseRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) -- DirectShow strmbase renderer constructor: CBaseFilter on the interface lock, CBaseRenderer's three vtables, the init list, the three critical sections, then Ready()
// Leaf: ??0CBaseRenderer@@QAE@ABU_GUID@@PBGPAUIUnknown@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA61D0, 248 bytes, thiscall, `ret 10h`; built under /EHsc (states 0-2 for the three CAMEvent members).
// Identity (proven, chunk 704 suggestion): the DX9 SDK strmbase renbase.cpp constructor. (1) The proven CBaseVideoRenderer ctor 0x00CA7360
//   calls it as its base (CBaseVideoRenderer : CBaseRenderer). (2) It stores 0x012BF498 (+0x00) / 0x012BF458 (+0x0C) / 0x012BF444 (+0x10):
//   0x012BF458 is CBaseRenderer's IBaseFilter vtable (the landed CBaseFilter::QueryInterface 0x00CA62D0: slots CBaseRenderer::Stop /
//   Pause / Run / GetState / FindPin) and 0x012BF498 slot 0 is CBaseRenderer::NonDelegatingQueryInterface 0x00CA4780; the destructor
//   storing them is ~CBaseRenderer 0x00CA6370 (chunk 704). (3) The body is the SDK's: CBaseFilter(pName, pUnk, &m_InterfaceLock (+0x7C),
//   RenderClass), m_RenderEvent / m_ThreadSignal(TRUE) / m_evComplete(TRUE), the zeroed state, m_bRepaintStatus(TRUE) +0xB0, the three
//   CCritSec members (InitializeCriticalSection, the import cached in a register) and Ready() (SetEvent on m_evComplete +0x5C).
// Callees: CBaseFilter::CBaseFilter 0x00CAC830, CAMEvent::CAMEvent 0x00CB0190, InitializeCriticalSection / SetEvent (imports).
// Layout: the chunk 704 ~CBaseVideoRenderer model of CUnknown / CBaseFilter / CBaseRenderer (AM_NOVTABLE bases, novtable interfaces).
// History: landed as CVolume_00ca61d0 (classes named by address, Leaf ??0CHeroExperienceDef@); re-authored on the strmbase hierarchy in
//   naming wave 32 (family dshow).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);
#define TRUE 1
''' + HIER + '''
CBaseRenderer::CBaseRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr)
    : CBaseFilter(pName, pUnk, &m_InterfaceLock, RenderClass),
      m_evComplete(TRUE),
      m_bAbort(FALSE),
      m_pPosition(0),
      m_ThreadSignal(TRUE),
      m_bStreaming(FALSE),
      m_bEOS(FALSE),
      m_bEOSDelivered(FALSE),
      m_pMediaSample(0),
      m_dwAdvise(0),
      m_pQSink(0),
      m_pInputPin(0),
      m_bRepaintStatus(TRUE),
      m_SignalTime(0),
      m_bInReceive(FALSE),
      m_EndOfStreamTimer(0)
{
    Ready();
}
''')
# 0x00CB0190: the own row of the CAMEvent ctor this TU binds (was CEvent_00cb0190): wxutil.cpp CAMEvent::CAMEvent(BOOL fManualReset)
#  { m_hEvent = CreateEvent(NULL, fManualReset, FALSE, NULL); } (UNICODE: CreateEventW); ~CAMEvent 0x00CB01B0 is landed as ??1CAMEvent@@QAE@XZ.
he.put('00cb0190', 'src', '''// CAMEvent::CAMEvent(BOOL fManualReset) -- DirectShow strmbase wxutil.cpp: create an unnamed, initially unsignalled Win32 event (CreateEventW @ IAT 0x0143FDC8) and keep its handle
// Leaf: ??0CAMEvent@@QAE@H@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0190, 32 bytes, thiscall, `ret 4`.
// Identity: the DX9 SDK wxutil.cpp CAMEvent ctor (one BOOL argument in the DX9 SDK; UNICODE build: CreateEvent = CreateEventW); called
//   with 0 / 1 / 1 for m_RenderEvent / m_ThreadSignal / m_evComplete by CBaseRenderer::CBaseRenderer 0x00CA61D0; its destructor
//   0x00CB01B0 is landed as ??1CAMEvent@@QAE@XZ (0x00CAF580). The catalog label "CHeroExperienceDef::Init" was wrong.
// Shape: four pushes, `call [__imp]`, `mov [esi],eax`, `mov eax,esi`, `ret 4`.
// History: landed as CEvent_00cb0190 (named by address); respelled in naming wave 32 (family dshow).
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);
typedef int BOOL;                                  // windef.h
#define FALSE 0

class CAMEvent {
protected:
    void* m_hEvent;                                // +0x00 HANDLE
public:
    CAMEvent(BOOL fManualReset = FALSE);
};

CAMEvent::CAMEvent(BOOL fManualReset)
{
    m_hEvent = CreateEventW(0, fManualReset, FALSE, 0);
}
''')
# 0x00CA6370: ~CBaseRenderer (chunk 704 label; ~CBaseVideoRenderer 0x00CA7310 chains to it), was the CVolume_00ca6370 model: the DX9 SDK
#  renbase.cpp body StopStreaming() / ClearPendingSample() (virtual, called directly in a destructor), delete m_pPosition (the CUnknown
#  subobject of CRendererPosPassThru sits at +8: IMediaSeeking +0, IMediaPosition +4) and delete m_pInputPin (CUnknown at +0), then the
#  members in reverse and ~CBaseFilter 0x00CAAAC0. Its seven unwind funclets (0x012169F0 ..) include this source.
HIER3 = HIER.replace('''    virtual ~CBaseRenderer();                               // 0x00CA6370
};''', '''    virtual ~CBaseRenderer();                               // 0x00CA6370
    virtual HRESULT ClearPendingSample();                   // 0x00CA4BF0 (slot 28)
    virtual HRESULT StopStreaming();                        // 0x00CA4E30 (slot 32)
};''')
assert 'StopStreaming' in HIER3
he.put('00ca6370', 'src', '''// CBaseRenderer::~CBaseRenderer() -- DirectShow strmbase renderer destructor: CBaseRenderer's three vtables, StopStreaming, ClearPendingSample, delete the position pass-through and the input pin, then the members and ~CBaseFilter
// Leaf: ??1CBaseRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA6370, 216 bytes, thiscall; built under /EHsc (state 6 for the body, 2-0 for the CAMEvent members);
//   seven unwind funclets 0x012169F0 .. 0x01216A32 (landed rows that include this source).
// Identity (proven, chunk 704 label): the DX9 SDK strmbase renbase.cpp destructor. It stores CBaseRenderer's vtables 0x012BF498 / 458 /
//   444 (see CBaseRenderer::CBaseRenderer 0x00CA61D0), ~CBaseVideoRenderer 0x00CA7310 tail-jumps here, and the body is the SDK's:
//   StopStreaming() 0x00CA4E30 and ClearPendingSample() 0x00CA4BF0 (slots 32 / 28 of 0x012BF498, called directly as in any destructor),
//   delete m_pPosition (+0x50; the CUnknown subobject of CRendererPosPassThru is at +8, so the scalar deleting destructor is called
//   through [m_pPosition+8]) and delete m_pInputPin (+0x78, CUnknown at +0), then the CCritSec members (DeleteCriticalSection inline;
//   the funclets call its out-of-line copy 0x00CA4870), the three CAMEvent members (~CAMEvent 0x00CB01B0) and ~CBaseFilter 0x00CAAAC0.
// Layout: the chunk 704 ~CBaseVideoRenderer model of CUnknown / CBaseFilter / CBaseRenderer (AM_NOVTABLE bases, novtable interfaces); the
//   ctlutil.h CPosPassThru (IMediaSeeking, CMediaPosition = IMediaPosition + CUnknown) and amfilter.h CBasePin (CUnknown, IPin,
//   IQualityControl) prefixes for the two deletes.
// History: landed as CVolume_00ca6370 (classes named by address); re-authored on the strmbase hierarchy in naming wave 32 (family dshow).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);
#define TRUE 1
''' + HIER3 + '''
struct AM_NOVTABLE IMediaSeeking : public IUnknown { };     // strmif.h (slots not used here)
struct AM_NOVTABLE IDispatch : public IUnknown { };         // oaidl.h
struct AM_NOVTABLE IMediaPosition : public IDispatch { };   // control.h
class AM_NOVTABLE CMediaPosition : public IMediaPosition, public CUnknown { };       // ctlutil.h
class CPosPassThru : public IMediaSeeking, public CMediaPosition { };                // ctlutil.h: CUnknown at +8
class CRendererPosPassThru : public CPosPassThru { };                                // ctlutil.h
struct AM_NOVTABLE IPin : public IUnknown { };              // strmif.h
struct AM_NOVTABLE IMemInputPin : public IUnknown { };      // strmif.h
class AM_NOVTABLE CBasePin : public CUnknown, public IPin, public IQualityControl { };   // amfilter.h: CUnknown at +0
class AM_NOVTABLE CBaseInputPin : public CBasePin, public IMemInputPin { };           // amfilter.h
class CRendererInputPin : public CBaseInputPin { };                                  // renbase.h

CBaseRenderer::~CBaseRenderer()
{
    StopStreaming();
    ClearPendingSample();

    // Delete any IMediaPosition implementation
    if (m_pPosition) {
        delete m_pPosition;
        m_pPosition = 0;
    }

    // Delete any input pin created
    if (m_pInputPin) {
        delete m_pInputPin;
        m_pInputPin = 0;
    }
}
''')
