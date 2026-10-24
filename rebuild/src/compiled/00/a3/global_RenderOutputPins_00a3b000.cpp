// RenderOutputPins(IGraphBuilder* pGB, IBaseFilter* pFilter) -- DirectShow SDK helper: enumerate the filter's pins and Render every unconnected output pin; stops at the first failure and returns its HRESULT
// Leaf: ?RenderOutputPins@@
// Retail: Fable.exe @ 0x00A3B000, 225 bytes (+ int3 padding), plain `ret`; __fastcall (pGB in ecx, pFilter in edx --
//   the CVideoSys TU is /Gr). No rel32 caller in TLC (kept, unreferenced).
// Callees: COM vtable calls only -- IBaseFilter::EnumPins (+0x28), IEnumPins::Next (+0x0C), IPin::ConnectedTo (+0x18),
//   IPin::QueryDirection (+0x24), IGraphBuilder::Render (+0x30), IUnknown::Release (+0x08).
// Label: the catalog name is right (the DirectX 9 SDK dshowutil.h RenderOutputPins), unscoped.
// Source: the SDK body verbatim: pEnumPin / pConnectedPin / pPin start NULL, VFW_E_NOT_CONNECTED = 0x80040209,
//   PINDIR_OUTPUT = 1; the final pEnumPin->Release() has no null test. Lane has no Windows SDK headers, so the COM
//   interfaces are written out with their real slot order.
struct _GUID;
typedef long HRESULT;
typedef unsigned long ULONG;

struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(const _GUID& riid, void** ppv) = 0;
    virtual ULONG __stdcall AddRef() = 0;
    virtual ULONG __stdcall Release() = 0;                  // +0x08
};

enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };

struct IPin;
struct IBaseFilter;
struct IEnumFilters;
struct IEnumMediaTypes;
struct IReferenceClock;
struct _AMMediaType;
struct _PinInfo;

struct IPin : IUnknown {
    virtual HRESULT __stdcall Connect(IPin* pReceivePin, const _AMMediaType* pmt) = 0;
    virtual HRESULT __stdcall ReceiveConnection(IPin* pConnector, const _AMMediaType* pmt) = 0;
    virtual HRESULT __stdcall Disconnect() = 0;
    virtual HRESULT __stdcall ConnectedTo(IPin** pPin) = 0;            // +0x18
    virtual HRESULT __stdcall ConnectionMediaType(_AMMediaType* pmt) = 0;
    virtual HRESULT __stdcall QueryPinInfo(_PinInfo* pInfo) = 0;
    virtual HRESULT __stdcall QueryDirection(PIN_DIRECTION* pPinDir) = 0;   // +0x24
};

struct IEnumPins : IUnknown {
    virtual HRESULT __stdcall Next(ULONG cPins, IPin** ppPins, ULONG* pcFetched) = 0;   // +0x0C
    virtual HRESULT __stdcall Skip(ULONG cPins) = 0;
    virtual HRESULT __stdcall Reset() = 0;
    virtual HRESULT __stdcall Clone(IEnumPins** ppEnum) = 0;
};

struct IPersist : IUnknown {
    virtual HRESULT __stdcall GetClassID(_GUID* pClassID) = 0;
};
struct IMediaFilter : IPersist {
    virtual HRESULT __stdcall Stop() = 0;
    virtual HRESULT __stdcall Pause() = 0;
    virtual HRESULT __stdcall Run(__int64 tStart) = 0;
    virtual HRESULT __stdcall GetState(unsigned long dwMilliSecsTimeout, int* State) = 0;
    virtual HRESULT __stdcall SetSyncSource(IReferenceClock* pClock) = 0;
    virtual HRESULT __stdcall GetSyncSource(IReferenceClock** pClock) = 0;
};
struct IBaseFilter : IMediaFilter {
    virtual HRESULT __stdcall EnumPins(IEnumPins** ppEnum) = 0;       // +0x28
};

struct IFilterGraph : IUnknown {
    virtual HRESULT __stdcall AddFilter(IBaseFilter* pFilter, const unsigned short* pName) = 0;
    virtual HRESULT __stdcall RemoveFilter(IBaseFilter* pFilter) = 0;
    virtual HRESULT __stdcall EnumFilters(IEnumFilters** ppEnum) = 0;
    virtual HRESULT __stdcall FindFilterByName(const unsigned short* pName, IBaseFilter** ppFilter) = 0;
    virtual HRESULT __stdcall ConnectDirect(IPin* ppinOut, IPin* ppinIn, const _AMMediaType* pmt) = 0;
    virtual HRESULT __stdcall Reconnect(IPin* ppin) = 0;
    virtual HRESULT __stdcall Disconnect(IPin* ppin) = 0;
    virtual HRESULT __stdcall SetDefaultSyncSource() = 0;
};
struct IGraphBuilder : IFilterGraph {
    virtual HRESULT __stdcall Connect(IPin* ppinOut, IPin* ppinIn) = 0;
    virtual HRESULT __stdcall Render(IPin* ppinOut) = 0;                // +0x30
};

#define S_OK                ((HRESULT)0L)
#define SUCCEEDED(hr)       (((HRESULT)(hr)) >= 0)
#define FAILED(hr)          (((HRESULT)(hr)) < 0)
#define VFW_E_NOT_CONNECTED ((HRESULT)0x80040209L)

HRESULT __fastcall RenderOutputPins(IGraphBuilder* pGB, IBaseFilter* pFilter)
{
    HRESULT       hr = S_OK;
    IEnumPins*    pEnumPin = 0;
    IPin*         pConnectedPin = 0, *pPin = 0;
    PIN_DIRECTION PinDirection;
    ULONG         ulFetched;

    // Enumerate all pins on the filter
    hr = pFilter->EnumPins(&pEnumPin);

    if (SUCCEEDED(hr)) {
        // Step through every pin, looking for the output pins
        while (S_OK == (hr = pEnumPin->Next(1L, &pPin, &ulFetched))) {
            // Is this pin connected?  We're not interested in connected pins.
            hr = pPin->ConnectedTo(&pConnectedPin);
            if (pConnectedPin) {
                pConnectedPin->Release();
                pConnectedPin = 0;
            }

            // If this pin is not connected, render it.
            if (VFW_E_NOT_CONNECTED == hr) {
                hr = pPin->QueryDirection(&PinDirection);
                if ((S_OK == hr) && (PinDirection == PINDIR_OUTPUT))
                    hr = pGB->Render(pPin);
            }
            pPin->Release();

            // If there was an error, stop enumerating
            if (FAILED(hr))
                break;
        }
    }

    // Release pin enumerator
    pEnumPin->Release();
    return hr;
}