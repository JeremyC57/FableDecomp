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
// CDynamicOutputPin::StopUsingOutputPin: drop one user; the last user out of a pending block completes the block.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CDynamicOutputPin {
public:
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    CCritSec m_BlockStateLock; BLOCK_STATE m_BlockState; DWORD m_dwNumOutstandingOutputPinUsers; int blocks;
    CDynamicOutputPin() : m_BlockState(NOT_BLOCKED), m_dwNumOutstandingOutputPinUsers(0), blocks(0) {}
    void BlockOutputPin() { blocks++; m_BlockState = BLOCKED; }
    void StopUsingOutputPin()
    {
        CAutoLock alBlockStateLock(&m_BlockStateLock);
        m_dwNumOutstandingOutputPinUsers--;
        if ((m_dwNumOutstandingOutputPinUsers == 0) && (NOT_BLOCKED != m_BlockState)) {
            BlockOutputPin();
        }
    }
};
static void run()
{
    CDynamicOutputPin p; p.m_dwNumOutstandingOutputPinUsers = 2;
    p.StopUsingOutputPin(); CHECK(p.m_dwNumOutstandingOutputPinUsers == 1); CHECK(p.blocks == 0);
    p.StopUsingOutputPin(); CHECK(p.m_dwNumOutstandingOutputPinUsers == 0); CHECK(p.blocks == 0);
    p.m_dwNumOutstandingOutputPinUsers = 2; p.m_BlockState = CDynamicOutputPin::PENDING;
    p.StopUsingOutputPin(); CHECK(p.blocks == 0);
    p.StopUsingOutputPin(); CHECK(p.blocks == 1); CHECK(p.m_BlockState == CDynamicOutputPin::BLOCKED);
    CHECK(p.m_BlockStateLock.locks == 4); CHECK(p.m_BlockStateLock.unlocks == 4);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9a00_TEST FAIL\n"); return 1; }
    std::printf("00ca9a00_TEST PASS\n");
    return 0;
}