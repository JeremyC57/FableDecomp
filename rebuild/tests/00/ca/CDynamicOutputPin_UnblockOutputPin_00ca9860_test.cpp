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
// CDynamicOutputPin::UnblockOutputPin: S_FALSE when not blocked; otherwise signal the unblock event, signal and close
// the caller's notification event, and reset the block state.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
static int g_ev[4]; static int g_closed[4];
static BOOL SetEvent(HANDLE h) { g_ev[*(int*)h]++; return 1; }
static BOOL CloseHandle(HANDLE h) { g_closed[*(int*)h]++; return 1; }
class CDynamicOutputPin {
public:
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    CCritSec m_BlockStateLock; HANDLE m_hUnblockOutputPinEvent; HANDLE m_hNotifyCallerPinBlockedEvent;
    BLOCK_STATE m_BlockState; DWORD m_dwBlockCallerThreadID;
    HRESULT UnblockOutputPin()
    {
        CAutoLock alBlockStateLock(&m_BlockStateLock);
        if (NOT_BLOCKED == m_BlockState) {
            return 1;
        }
        SetEvent(m_hUnblockOutputPinEvent);
        if (0 != m_hNotifyCallerPinBlockedEvent) {
            SetEvent(m_hNotifyCallerPinBlockedEvent);
            CloseHandle(m_hNotifyCallerPinBlockedEvent);
        }
        m_BlockState = NOT_BLOCKED;
        m_dwBlockCallerThreadID = 0;
        m_hNotifyCallerPinBlockedEvent = 0;
        return 0;
    }
};
static void run()
{
    static int h1 = 1, h2 = 2;
    CDynamicOutputPin p; p.m_hUnblockOutputPinEvent = &h1; p.m_hNotifyCallerPinBlockedEvent = 0;
    p.m_BlockState = CDynamicOutputPin::NOT_BLOCKED; p.m_dwBlockCallerThreadID = 9;
    CHECK(p.UnblockOutputPin() == 1); CHECK(g_ev[1] == 0); CHECK(p.m_dwBlockCallerThreadID == 9);
    p.m_BlockState = CDynamicOutputPin::BLOCKED; p.m_hNotifyCallerPinBlockedEvent = &h2;
    CHECK(p.UnblockOutputPin() == 0); CHECK(g_ev[1] == 1); CHECK(g_ev[2] == 1); CHECK(g_closed[2] == 1);
    CHECK(p.m_BlockState == CDynamicOutputPin::NOT_BLOCKED); CHECK(p.m_dwBlockCallerThreadID == 0); CHECK(p.m_hNotifyCallerPinBlockedEvent == 0);
    p.m_BlockState = CDynamicOutputPin::PENDING;
    CHECK(p.UnblockOutputPin() == 0); CHECK(g_ev[1] == 2); CHECK(g_ev[2] == 1);
    CHECK(p.m_BlockStateLock.locks == 3); CHECK(p.m_BlockStateLock.unlocks == 3);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9860_TEST FAIL\n"); return 1; }
    std::printf("00ca9860_TEST PASS\n");
    return 0;
}