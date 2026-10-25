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
// CDynamicOutputPin::CompleteConnect: the base completion (DecideAllocator on the input pin) and, when the filter is
// not stopped, an immediate Commit of the chosen allocator.
struct IMemAllocator { int commits; HRESULT hrCommit; IMemAllocator() : commits(0), hrCommit(0) {} HRESULT Commit() { commits++; return hrCommit; } };
struct IMemInputPin { int dummy; };
struct IPin { int dummy; };
static IMemAllocator g_alloc;
class CBaseOutputPin {
public:
    IMemAllocator* m_pAllocator; IMemInputPin* m_pInputPin; BOOL m_stopped; HRESULT hrDecide; IMemInputPin* seenPin;
    CBaseOutputPin() : m_pAllocator(0), m_pInputPin(0), m_stopped(1), hrDecide(0), seenPin(0) {}
    BOOL IsStopped() { return m_stopped; }
    virtual HRESULT DecideAllocator(IMemInputPin* pPin, IMemAllocator** pp) { seenPin = pPin; if (hrDecide >= 0) *pp = &g_alloc; return hrDecide; }
    HRESULT CompleteConnect(IPin*) { return DecideAllocator(m_pInputPin, &m_pAllocator); }
};
class CDynamicOutputPin : public CBaseOutputPin {
public:
    HRESULT CompleteConnect(IPin* pReceivePin)
    {
        HRESULT hr = CBaseOutputPin::CompleteConnect(pReceivePin);
        if (hr >= 0) {
            if (!IsStopped() && m_pAllocator) {
                hr = m_pAllocator->Commit();
            }
        }
        return hr;
    }
};
static void run()
{
    IPin peer; IMemInputPin in;
    { CDynamicOutputPin p; p.m_pInputPin = &in;
      CHECK(p.CompleteConnect(&peer) == 0); CHECK(p.m_pAllocator == &g_alloc); CHECK(p.seenPin == &in); CHECK(g_alloc.commits == 0); }
    { CDynamicOutputPin p; p.m_stopped = 0; g_alloc.hrCommit = HR(0x80040211);
      CHECK(p.CompleteConnect(&peer) == HR(0x80040211)); CHECK(g_alloc.commits == 1); }
    { CDynamicOutputPin p; p.m_stopped = 0; p.hrDecide = HR(0x80040201);
      CHECK(p.CompleteConnect(&peer) == HR(0x80040201)); CHECK(g_alloc.commits == 1); CHECK(p.m_pAllocator == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9c80_TEST FAIL\n"); return 1; }
    std::printf("00ca9c80_TEST PASS\n");
    return 0;
}