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
// CDynamicOutputPin::Block (IPinFlowControl): validate the flags/event pair, reset the caller's event, then block
// synchronously (no event), asynchronously (event) or unblock (no flags); any success becomes S_OK.
#define E_INVALIDARG HR(0x80070057)
static BOOL g_resetOk = 1; static int g_resets = 0;
static BOOL ResetEvent(HANDLE) { g_resets++; return g_resetOk; }
static HRESULT AmGetLastErrorToHResult() { return HR(0x80070006); }
class CDynamicOutputPin {
public:
    char last; HANDLE lastEvent; HRESULT hrOp;
    CDynamicOutputPin() : last(0), lastEvent(0), hrOp(0) {}
    HRESULT SynchronousBlockOutputPin() { last = 's'; return hrOp; }
    HRESULT AsynchronousBlockOutputPin(HANDLE h) { last = 'a'; lastEvent = h; return hrOp; }
    HRESULT UnblockOutputPin() { last = 'u'; return hrOp; }
    HRESULT Block(DWORD dwBlockFlags, HANDLE hEvent)
    {
        const DWORD VALID_FLAGS = 0x1;
        if (dwBlockFlags & ~VALID_FLAGS) {
            return E_INVALIDARG;
        }
        if ((dwBlockFlags & 0x1) && (0 != hEvent)) {
            if (!ResetEvent(hEvent)) {
                return AmGetLastErrorToHResult();
            }
        }
        if (0 == dwBlockFlags) {
            if (0 != hEvent) {
                return E_INVALIDARG;
            }
        }
        HRESULT hr;
        if (dwBlockFlags & 0x1) {
            if (0 == hEvent) {
                hr = SynchronousBlockOutputPin();
            } else {
                hr = AsynchronousBlockOutputPin(hEvent);
            }
        } else {
            hr = UnblockOutputPin();
        }
        if (hr < 0) {
            return hr;
        }
        return 0;
    }
};
static void run()
{
    static int ev = 0; HANDLE h = &ev;
    { CDynamicOutputPin p; CHECK(p.Block(2, 0) == E_INVALIDARG); CHECK(p.last == 0); }
    { CDynamicOutputPin p; CHECK(p.Block(0, h) == E_INVALIDARG); CHECK(p.last == 0); CHECK(g_resets == 0); }
    { CDynamicOutputPin p; CHECK(p.Block(1, 0) == 0); CHECK(p.last == 's'); CHECK(g_resets == 0); }
    { CDynamicOutputPin p; p.hrOp = 1; CHECK(p.Block(1, h) == 0); CHECK(p.last == 'a'); CHECK(p.lastEvent == h); CHECK(g_resets == 1); }
    { CDynamicOutputPin p; p.hrOp = HR(0x8004020B); CHECK(p.Block(0, 0) == HR(0x8004020B)); CHECK(p.last == 'u'); }
    { CDynamicOutputPin p; g_resetOk = 0; CHECK(p.Block(1, h) == HR(0x80070006)); CHECK(p.last == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00cac920_TEST FAIL\n"); return 1; }
    std::printf("00cac920_TEST PASS\n");
    return 0;
}