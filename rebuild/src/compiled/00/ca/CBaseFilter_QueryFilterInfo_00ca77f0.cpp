// CBaseFilter::QueryFilterInfo(FILTER_INFO* info) -- the DirectShow base-class IBaseFilter method linked into Fable for CTextureRenderer (its IBaseFilter vtable 0x0129D04C, slot 12): E_POINTER for a null info, else copy the filter name (lstrcpynW, 128 WCHARs, or an empty name), hand out the filter graph with an AddRef and return NOERROR
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA77F0, length 81 bytes.
// Leaf: QueryFilterInfo
// Callees: kernel32!lstrcpynW (`call [__imp]` @ 0x0143FF08), IFilterGraph::AddRef (COM slot 1, `this` pushed).
// Shape: a COM method (`__stdcall`, `this` = the IBaseFilter sub-object as the first stack argument, `ret 8`):
// the CheckPointer early-out comes first (`test edi,edi ; jne` + `mov eax,80004003h ; pop edi ; ret 8`), the
// name at +0x30 and the graph at +0x34 relative to the IBaseFilter sub-object (+0x3c/+0x40 in CBaseFilter),
// the graph member read twice (the store through `info` may alias it), `xor eax,eax`. The manifest label
// "CSurface::CSurface" is propagated; the strmbase classes are not in the game PDB, so they are modelled from
// the DirectShow SDK layout (CUnknown, IBaseFilter at +0x0c, IAMovieSetup at +0x10, the filter state after).
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
    HRESULT_T __stdcall QueryFilterInfo(FILTER_INFO_Local* info);
};

HRESULT_T __stdcall CBaseFilter::QueryFilterInfo(FILTER_INFO_Local* info)
{
    if (info == 0)
        return (HRESULT_T)0x80004003L;              // E_POINTER (CheckPointer)
    if (m_pName)
        lstrcpynW(info->achName, m_pName, 128);
    else
        info->achName[0] = 0;
    info->pGraph = m_pGraph;
    if (m_pGraph)
        m_pGraph->AddRef();
    return 0;                                      // NOERROR
}