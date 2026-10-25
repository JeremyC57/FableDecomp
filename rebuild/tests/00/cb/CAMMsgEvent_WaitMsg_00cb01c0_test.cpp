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
// CAMMsgEvent::WaitMsg: wait for the event while dispatching sent messages; a finite timeout shrinks by the elapsed
// time on each wake-up; TRUE only when the event itself fired.
struct MSG { int dummy[7]; };
static DWORD g_now = 1000; static DWORD g_results[8]; static int g_nRes = 0, g_iRes = 0; static DWORD g_waits[8];
static int g_peeks = 0; static const HANDLE* g_handles = 0;
static DWORD timeGetTime() { return g_now; }
static DWORD MsgWaitForMultipleObjects(DWORD n, const HANDLE* h, BOOL all, DWORD ms, DWORD mask)
{
    g_handles = h; g_waits[g_iRes] = ms;
    if (n != 1 || all != 0 || mask != 0x40) return 0xFFFFFFFF;
    g_now += 30;
    return g_iRes < g_nRes ? g_results[g_iRes++] : 0x102;
}
static BOOL PeekMessageW(MSG*, void*, unsigned int, unsigned int, unsigned int remove) { g_peeks++; return remove == 0; }
class CAMEvent { protected: HANDLE m_hEvent; public: CAMEvent() : m_hEvent(0) {} };
class CAMMsgEvent : public CAMEvent {
public:
    const HANDLE* handle() const { return &m_hEvent; }
    BOOL WaitMsg(DWORD dwTimeout)
    {
        DWORD dwWait;
        DWORD dwStartTime = 0;
        DWORD dwWaitTime = dwTimeout;
        if (dwWaitTime != 0xFFFFFFFF) {
            dwStartTime = timeGetTime();
        }
        do {
            dwWait = MsgWaitForMultipleObjects(1, &m_hEvent, 0, dwWaitTime, 0x40);
            if (dwWait == 0 + 1) {
                MSG Message;
                PeekMessageW(&Message, 0, 0, 0, 0);
                if (dwWaitTime != 0xFFFFFFFF) {
                    DWORD dwElapsed = timeGetTime() - dwStartTime;
                    dwWaitTime = (dwElapsed >= dwTimeout) ? 0 : dwTimeout - dwElapsed;
                }
            }
        } while (dwWait == 0 + 1);
        return (dwWait == 0);
    }
};
static void run()
{
    CAMMsgEvent e;
    g_results[0] = 1; g_results[1] = 1; g_results[2] = 0; g_nRes = 3; g_iRes = 0;
    CHECK(e.WaitMsg(100) == 1); CHECK(g_peeks == 2); CHECK(g_handles == e.handle());
    CHECK(g_waits[0] == 100 && g_waits[1] == 70 && g_waits[2] == 40);
    g_results[0] = 1; g_results[1] = 1; g_results[2] = 1; g_nRes = 3; g_iRes = 0; g_peeks = 0;
    CHECK(e.WaitMsg(50) == 0); CHECK(g_peeks == 3); CHECK(g_waits[1] == 20 && g_waits[2] == 0);
    g_results[0] = 1; g_results[1] = 0; g_nRes = 2; g_iRes = 0; g_peeks = 0;
    CHECK(e.WaitMsg(0xFFFFFFFF) == 1); CHECK(g_waits[0] == 0xFFFFFFFF && g_waits[1] == 0xFFFFFFFF); CHECK(g_peeks == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cb01c0_TEST FAIL\n"); return 1; }
    std::printf("00cb01c0_TEST PASS\n");
    return 0;
}