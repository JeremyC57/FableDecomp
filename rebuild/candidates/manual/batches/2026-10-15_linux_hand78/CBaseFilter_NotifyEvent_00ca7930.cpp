// CBaseFilter::NotifyEvent(long eventCode, long eventParam1, long eventParam2) -- the DirectShow base-class helper linked into Fable for CTextureRenderer: forward the event to the graph's event sink (snapshot of m_pSink at +0x44), substituting the filter's IBaseFilter interface as the second parameter of EC_COMPLETE; E_NOTIMPL when no sink is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7930, length 67 bytes.
// Leaf: NotifyEvent
// Callees: IMediaEventSink::Notify (COM slot 3, `call [esi+0Ch]`, `this` pushed).
// Shape: `__thiscall`, `ret 0Ch`; the sink snapshot in eax, the EC_COMPLETE (1) test, and the Notify call
// tail-duplicated into both arms (`add ecx,0Ch` = the IBaseFilter sub-object as the substituted parameter),
// the E_NOTIMPL exit placed last. The manifest label "CSurface::GetRefCount" is propagated; the strmbase
// classes are modelled from the DirectShow SDK layout (not in the game PDB).
typedef unsigned short WCHAR_T;                    // wchar_t is not a builtin under the era defaults
typedef long HRESULT_T;

extern "C" __declspec(dllimport) WCHAR_T* __stdcall lstrcpynW(WCHAR_T* destination, const WCHAR_T* source, int length);   // 0x0143FF08

struct IUnknownLike {                              // IUnknown
    virtual HRESULT_T __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;  // slot 1 (+0x04)
    virtual unsigned long __stdcall Release() = 0; // slot 2 (+0x08)
};

struct IFilterGraphLike : IUnknownLike {};

struct IMediaEventSinkLike : IUnknownLike {
    virtual HRESULT_T __stdcall Notify(long eventCode, long param1, long param2) = 0;   // slot 3 (+0x0c)
};
struct IReferenceClockLike : IUnknownLike {};

struct FILTER_INFO_Local {                         // FILTER_INFO
    WCHAR_T           achName[128];                // +0x000
    IFilterGraphLike* pGraph;                      // +0x100
};

struct INonDelegatingUnknownLike {
    virtual HRESULT_T __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};

class CUnknown : public INonDelegatingUnknownLike {   // DirectShow CUnknown, 0x0c bytes
protected:
    IUnknownLike* m_pUnknown;                      // +0x04
    long          m_cRef;                          // +0x08
};

struct IBaseFilterLike : IUnknownLike {            // IPersist + IMediaFilter + IBaseFilter slots 3..14
    virtual HRESULT_T __stdcall GetClassID(void* clsid) = 0;                 // slot 3
    virtual HRESULT_T __stdcall Stop() = 0;
    virtual HRESULT_T __stdcall Pause() = 0;
    virtual HRESULT_T __stdcall Run(__int64 start) = 0;
    virtual HRESULT_T __stdcall GetState(unsigned long ms, long* state) = 0;
    virtual HRESULT_T __stdcall SetSyncSource(IReferenceClockLike* clock) = 0;
    virtual HRESULT_T __stdcall GetSyncSource(IReferenceClockLike** clock) = 0;
    virtual HRESULT_T __stdcall EnumPins(void** pins) = 0;                   // slot 10
    virtual HRESULT_T __stdcall FindPin(const WCHAR_T* id, void** pin) = 0;
    virtual HRESULT_T __stdcall QueryFilterInfo(FILTER_INFO_Local* info) = 0;   // slot 12
    virtual HRESULT_T __stdcall JoinFilterGraph(IFilterGraphLike* graph, const WCHAR_T* name) = 0;
    virtual HRESULT_T __stdcall QueryVendorInfo(WCHAR_T** vendorInfo) = 0;  // slot 14
};

struct IAMovieSetupLike : IUnknownLike {
    virtual HRESULT_T __stdcall Register() = 0;
    virtual HRESULT_T __stdcall Unregister() = 0;
};

struct GUID_Local { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

// DirectShow CBaseFilter (amfilter.h): CUnknown (+0), IBaseFilter (+0x0c), IAMovieSetup (+0x10), then the
// filter state. Relative to the IBaseFilter sub-object (the `this` a slot-12 call receives) the name sits at
// +0x30 and the graph at +0x34.
class CBaseFilter : public CUnknown, public IBaseFilterLike, public IAMovieSetupLike {
protected:
    long                 m_State;                  // +0x14
    IReferenceClockLike* m_pClock;                 // +0x18
    __int64              m_tStart;                 // +0x20 (CRefTime)
    GUID_Local           m_clsid;                  // +0x28
    void*                m_pLock;                  // +0x38 (CCritSec*)
    WCHAR_T*             m_pName;                  // +0x3c
    IFilterGraphLike*    m_pGraph;                 // +0x40
    IMediaEventSinkLike* m_pSink;                  // +0x44
    long                 m_PinVersion;             // +0x48
public:
    HRESULT_T NotifyEvent(long eventCode, long eventParam1, long eventParam2);
};

HRESULT_T CBaseFilter::NotifyEvent(long eventCode, long eventParam1, long eventParam2)
{
    IMediaEventSinkLike* pSink = m_pSink;          // snapshot, as in the SDK source
    if (pSink) {
        if (eventCode == 1)                        // EC_COMPLETE
            eventParam2 = (long)(IBaseFilterLike*)this;
        return pSink->Notify(eventCode, eventParam1, eventParam2);
    } else {
        return (HRESULT_T)0x80004001L;             // E_NOTIMPL
    }
}
