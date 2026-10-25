// Behaviour test (standalone; compiled separately from the source). Plain C++98: the COM interfaces are modelled
// as ordinary virtual classes (no calling-convention keywords), the Win32 imports as local fakes.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef unsigned int DWORD;
typedef int LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
#ifdef _MSC_VER
#define LL(x) x##i64
#else
#define LL(x) x##LL
#endif
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID IID;
inline bool operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define HR(x) ((HRESULT)(x))
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
// CBasePin::AttemptConnection: check, accept the type, look connected, set the type, ask the peer, complete;
// any failure undoes the connection (BreakConnect + release of m_Connected), type refusals map to VFW_E_TYPE_NOT_ACCEPTED.
#define VFW_E_TYPE_NOT_ACCEPTED HR(0x8004022A)
#define E_FAIL HR(0x80004005)
#define E_INVALIDARG HR(0x80070057)
struct CMediaType { int id; };
class CBasePin;
struct IPin {
    int refs, received, disconnects; HRESULT hrReceive;
    IPin() : refs(0), received(0), disconnects(0), hrReceive(0) {}
    ULONG AddRef() { return ++refs; }
    ULONG Release() { return --refs; }
    HRESULT ReceiveConnection(IPin*, const CMediaType*) { received++; return hrReceive; }
    HRESULT Disconnect() { disconnects++; return 0; }
};
class CBasePin : public IPin {
public:
    IPin* m_Connected;
    HRESULT hrCheckConnect, hrCheckType, hrSetType, hrComplete;
    int breaks, completes;
    CBasePin() : m_Connected(0), hrCheckConnect(0), hrCheckType(0), hrSetType(0), hrComplete(0), breaks(0), completes(0) {}
    virtual HRESULT CheckConnect(IPin*) { return hrCheckConnect; }
    virtual HRESULT BreakConnect() { breaks++; return 0; }
    virtual HRESULT CheckMediaType(const CMediaType*) { return hrCheckType; }
    virtual HRESULT SetMediaType(const CMediaType*) { return hrSetType; }
    virtual HRESULT CompleteConnect(IPin*) { completes++; return hrComplete; }
    void DisplayTypeInfo(IPin*, const CMediaType*) {}
    HRESULT AttemptConnection(IPin* pReceivePin, const CMediaType* pmt)
    {
        HRESULT hr = CheckConnect(pReceivePin);
        if (hr < 0) {
            BreakConnect();
            return hr;
        }
        DisplayTypeInfo(pReceivePin, pmt);
        hr = CheckMediaType(pmt);
        if (hr == 0) {
            m_Connected = pReceivePin;
            m_Connected->AddRef();
            hr = SetMediaType(pmt);
            if (hr >= 0) {
                hr = pReceivePin->ReceiveConnection((IPin*)this, pmt);
                if (hr >= 0) {
                    hr = CompleteConnect(pReceivePin);
                    if (hr >= 0) {
                        return hr;
                    } else {
                        pReceivePin->Disconnect();
                    }
                }
            }
        } else {
            if ((hr >= 0) || (hr == E_FAIL) || (hr == E_INVALIDARG)) {
                hr = VFW_E_TYPE_NOT_ACCEPTED;
            }
        }
        BreakConnect();
        if (m_Connected) {
            m_Connected->Release();
            m_Connected = 0;
        }
        return hr;
    }
};
static void run()
{
    CMediaType mt = { 1 };
    { CBasePin p; IPin peer; p.hrCheckConnect = HR(0x80040208);
      CHECK(p.AttemptConnection(&peer, &mt) == HR(0x80040208)); CHECK(p.breaks == 1); CHECK(p.m_Connected == 0); }
    { CBasePin p; IPin peer; p.hrCheckType = 1;
      CHECK(p.AttemptConnection(&peer, &mt) == VFW_E_TYPE_NOT_ACCEPTED); CHECK(p.breaks == 1); CHECK(peer.refs == 0); }
    { CBasePin p; IPin peer; p.hrCheckType = E_INVALIDARG;
      CHECK(p.AttemptConnection(&peer, &mt) == VFW_E_TYPE_NOT_ACCEPTED); }
    { CBasePin p; IPin peer; p.hrCheckType = HR(0x80040200);
      CHECK(p.AttemptConnection(&peer, &mt) == HR(0x80040200)); }
    { CBasePin p; IPin peer;
      CHECK(p.AttemptConnection(&peer, &mt) == 0); CHECK(p.m_Connected == &peer); CHECK(peer.refs == 1);
      CHECK(peer.received == 1); CHECK(p.completes == 1); CHECK(p.breaks == 0); }
    { CBasePin p; IPin peer; peer.hrReceive = HR(0x80004005);
      CHECK(p.AttemptConnection(&peer, &mt) == HR(0x80004005)); CHECK(p.m_Connected == 0); CHECK(peer.refs == 0);
      CHECK(p.completes == 0); CHECK(p.breaks == 1); }
    { CBasePin p; IPin peer; p.hrComplete = HR(0x8007000E);
      CHECK(p.AttemptConnection(&peer, &mt) == HR(0x8007000E)); CHECK(peer.disconnects == 1); CHECK(peer.refs == 0); }
    { CBasePin p; IPin peer; p.hrSetType = HR(0x80004001);
      CHECK(p.AttemptConnection(&peer, &mt) == HR(0x80004001)); CHECK(peer.received == 0); CHECK(peer.refs == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca7dc0_TEST FAIL\n"); return 1; }
    std::printf("00ca7dc0_TEST PASS\n");
    return 0;
}