#include <cstdio>

typedef int HRESULT;
typedef unsigned int ULONG;

enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };

// Test copies of the COM interfaces (only the slots the function uses; bodies supplied by the fakes).
struct IPin {
    virtual ULONG Release() = 0;
    virtual HRESULT ConnectedTo(IPin** pPin) = 0;
    virtual HRESULT QueryDirection(PIN_DIRECTION* pPinDir) = 0;
};
struct IEnumPins {
    virtual ULONG Release() = 0;
    virtual HRESULT Next(ULONG cPins, IPin** ppPins, ULONG* pcFetched) = 0;
};
struct IBaseFilter {
    virtual HRESULT EnumPins(IEnumPins** ppEnum) = 0;
};
struct IGraphBuilder {
    virtual HRESULT Render(IPin* ppinOut) = 0;
};

#define S_OK                ((HRESULT)0)
#define SUCCEEDED(hr)       (((HRESULT)(hr)) >= 0)
#define FAILED(hr)          (((HRESULT)(hr)) < 0)
#define VFW_E_NOT_CONNECTED ((HRESULT)0x80040209)

HRESULT RenderOutputPins(IGraphBuilder* pGB, IBaseFilter* pFilter)
{
    HRESULT       hr = S_OK;
    IEnumPins*    pEnumPin = 0;
    IPin*         pConnectedPin = 0, *pPin = 0;
    PIN_DIRECTION PinDirection;
    ULONG         ulFetched;

    hr = pFilter->EnumPins(&pEnumPin);

    if (SUCCEEDED(hr)) {
        while (S_OK == (hr = pEnumPin->Next(1L, &pPin, &ulFetched))) {
            hr = pPin->ConnectedTo(&pConnectedPin);
            if (pConnectedPin) {
                pConnectedPin->Release();
                pConnectedPin = 0;
            }
            if (VFW_E_NOT_CONNECTED == hr) {
                hr = pPin->QueryDirection(&PinDirection);
                if ((S_OK == hr) && (PinDirection == PINDIR_OUTPUT))
                    hr = pGB->Render(pPin);
            }
            pPin->Release();
            if (FAILED(hr))
                break;
        }
    }

    pEnumPin->Release();
    return hr;
}

struct FakePin : IPin {
    PIN_DIRECTION Dir;
    IPin*         Peer;
    int           Releases;
    virtual ULONG Release() { return (ULONG)++Releases; }
    virtual HRESULT ConnectedTo(IPin** pPin)
    {
        *pPin = Peer;
        return Peer ? S_OK : VFW_E_NOT_CONNECTED;
    }
    virtual HRESULT QueryDirection(PIN_DIRECTION* pPinDir) { *pPinDir = Dir; return S_OK; }
};
struct FakeEnum : IEnumPins {
    IPin** Pins;
    int    Count;
    int    Pos;
    int    Releases;
    virtual ULONG Release() { return (ULONG)++Releases; }
    virtual HRESULT Next(ULONG, IPin** ppPins, ULONG* pcFetched)
    {
        if (Pos >= Count) { *pcFetched = 0; return 1; }   // S_FALSE
        *ppPins = Pins[Pos++];
        *pcFetched = 1;
        return S_OK;
    }
};
struct FakeFilter : IBaseFilter {
    FakeEnum* Enum;
    virtual HRESULT EnumPins(IEnumPins** ppEnum) { *ppEnum = Enum; return S_OK; }
};
struct FakeGraph : IGraphBuilder {
    IPin*   Rendered[8];
    int     Renders;
    HRESULT Result;
    virtual HRESULT Render(IPin* ppinOut) { Rendered[Renders++] = ppinOut; return Result; }
};

static void InitPin(FakePin& p, PIN_DIRECTION dir, IPin* peer) { p.Dir = dir; p.Peer = peer; p.Releases = 0; }

int main()
{
    FakePin out1, in1, outConnected, peer, out2;
    InitPin(out1, PINDIR_OUTPUT, 0);
    InitPin(in1, PINDIR_INPUT, 0);
    InitPin(peer, PINDIR_INPUT, 0);
    InitPin(outConnected, PINDIR_OUTPUT, &peer);
    InitPin(out2, PINDIR_OUTPUT, 0);
    IPin* pins[4] = { &out1, &in1, &outConnected, &out2 };
    FakeEnum e;
    e.Pins = pins; e.Count = 4; e.Pos = 0; e.Releases = 0;
    FakeFilter f;
    f.Enum = &e;
    FakeGraph g;
    g.Renders = 0; g.Result = S_OK;
    HRESULT hr = RenderOutputPins(&g, &f);
    // the loop ends on the enumerator's S_FALSE; only the two unconnected output pins are rendered
    if (hr != 1 || g.Renders != 2 || g.Rendered[0] != &out1 || g.Rendered[1] != &out2 || e.Releases != 1
        || out1.Releases != 1 || in1.Releases != 1 || outConnected.Releases != 1 || peer.Releases != 1 || out2.Releases != 1)
        { std::printf("00a3b000_TEST FAIL\n"); return 1; }
    // a failing Render stops the enumeration
    InitPin(out1, PINDIR_OUTPUT, 0);
    InitPin(out2, PINDIR_OUTPUT, 0);
    e.Pos = 0; e.Releases = 0;
    g.Renders = 0; g.Result = (HRESULT)0x80004005;
    hr = RenderOutputPins(&g, &f);
    if (hr != (HRESULT)0x80004005 || g.Renders != 1 || e.Pos != 1 || out1.Releases != 1 || e.Releases != 1)
        { std::printf("00a3b000_TEST FAIL\n"); return 1; }
    std::printf("00a3b000_TEST PASS\n");
    return 0;
}