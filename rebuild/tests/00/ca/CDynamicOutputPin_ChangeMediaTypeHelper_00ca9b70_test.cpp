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
// CDynamicOutputPin::ChangeMediaTypeHelper: the peer accepts the new type, we adopt it, then (when an input pin is
// attached) the allocator is decommitted, resized to the input pin's requirements, recommitted and re-announced.
struct CMediaType { int id; };
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };
static char g_log[32]; static int g_n = 0;
static void note(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct IMemAllocator { HRESULT hrDecommit, hrCommit; IMemAllocator() : hrDecommit(0), hrCommit(0) {}
    HRESULT Decommit() { note('d'); return hrDecommit; } HRESULT Commit() { note('c'); return hrCommit; } };
struct IMemInputPin { long align; BOOL ro; HRESULT hrNotify; IMemInputPin() : align(0), ro(-1), hrNotify(0) {}
    HRESULT GetAllocatorRequirements(ALLOCATOR_PROPERTIES* p) { note('r'); p->cbAlign = align; p->cBuffers = 2; return 0; }
    HRESULT NotifyAllocator(IMemAllocator*, BOOL r) { note('n'); ro = r; return hrNotify; } };
struct IPin { HRESULT hrReceive; IPin() : hrReceive(0) {} HRESULT ReceiveConnection(void*, const CMediaType*) { note('R'); return hrReceive; } };
class CDynamicOutputPin {
public:
    IPin* m_Connected; IMemAllocator* m_pAllocator; IMemInputPin* m_pInputPin; BOOL m_bPinUsesReadOnlyAllocator;
    HRESULT hrSet, hrSize; long seenAlign;
    CDynamicOutputPin() : m_Connected(0), m_pAllocator(0), m_pInputPin(0), m_bPinUsesReadOnlyAllocator(1), hrSet(0), hrSize(0), seenAlign(-1) {}
    virtual HRESULT SetMediaType(const CMediaType*) { note('S'); return hrSet; }
    virtual HRESULT DecideBufferSize(IMemAllocator*, ALLOCATOR_PROPERTIES* p) { note('z'); seenAlign = p->cbAlign; return hrSize; }
    HRESULT ChangeMediaTypeHelper(const CMediaType* pmt)
    {
        HRESULT hr = m_Connected->ReceiveConnection(this, pmt);
        if (hr < 0) {
            return hr;
        }
        hr = SetMediaType(pmt);
        if (hr < 0) {
            return hr;
        }
        if (0 != m_pInputPin) {
            ALLOCATOR_PROPERTIES apInputPinRequirements;
            apInputPinRequirements.cbAlign = 0;
            apInputPinRequirements.cbBuffer = 0;
            apInputPinRequirements.cbPrefix = 0;
            apInputPinRequirements.cBuffers = 0;
            m_pInputPin->GetAllocatorRequirements(&apInputPinRequirements);
            if (0 == apInputPinRequirements.cbAlign) {
                apInputPinRequirements.cbAlign = 1;
            }
            hr = m_pAllocator->Decommit();
            if (hr < 0) {
                return hr;
            }
            hr = DecideBufferSize(m_pAllocator, &apInputPinRequirements);
            if (hr < 0) {
                return hr;
            }
            hr = m_pAllocator->Commit();
            if (hr < 0) {
                return hr;
            }
            hr = m_pInputPin->NotifyAllocator(m_pAllocator, m_bPinUsesReadOnlyAllocator);
            if (hr < 0) {
                return hr;
            }
        }
        return 0;
    }
};
static void run()
{
    CMediaType mt = { 1 }; IPin peer; IMemAllocator al; IMemInputPin in;
    { CDynamicOutputPin p; p.m_Connected = &peer; g_n = 0;
      CHECK(p.ChangeMediaTypeHelper(&mt) == 0); CHECK(std::strcmp(g_log, "RS") == 0); }
    { CDynamicOutputPin p; p.m_Connected = &peer; p.m_pAllocator = &al; p.m_pInputPin = &in; g_n = 0;
      CHECK(p.ChangeMediaTypeHelper(&mt) == 0); CHECK(std::strcmp(g_log, "RSrdzcn") == 0); CHECK(p.seenAlign == 1); CHECK(in.ro == 1); }
    { CDynamicOutputPin p; p.m_Connected = &peer; p.m_pAllocator = &al; p.m_pInputPin = &in; in.align = 8; al.hrCommit = HR(0x80040211); g_n = 0;
      CHECK(p.ChangeMediaTypeHelper(&mt) == HR(0x80040211)); CHECK(std::strcmp(g_log, "RSrdzc") == 0); CHECK(p.seenAlign == 8); }
    { CDynamicOutputPin p; peer.hrReceive = HR(0x8004022A); p.m_Connected = &peer; g_n = 0;
      CHECK(p.ChangeMediaTypeHelper(&mt) == HR(0x8004022A)); CHECK(std::strcmp(g_log, "R") == 0); peer.hrReceive = 0; }
    { CDynamicOutputPin p; p.m_Connected = &peer; p.hrSet = HR(0x80004005); p.m_pInputPin = &in; g_n = 0;
      CHECK(p.ChangeMediaTypeHelper(&mt) == HR(0x80004005)); CHECK(std::strcmp(g_log, "RS") == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9b70_TEST FAIL\n"); return 1; }
    std::printf("00ca9b70_TEST PASS\n");
    return 0;
}