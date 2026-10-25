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
// CBaseInputPin::EndFlush: under the filter lock clear the flushing and run-time-error flags.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CBaseInputPin {
public:
    CCritSec* m_pLock; unsigned char m_bRunTimeError; unsigned char m_bFlushing;
    HRESULT EndFlush()
    {
        CAutoLock lck(m_pLock);
        m_bFlushing = 0;
        m_bRunTimeError = 0;
        return 0;
    }
};
static void run()
{
    CCritSec cs; CBaseInputPin p; p.m_pLock = &cs; p.m_bFlushing = 1; p.m_bRunTimeError = 1;
    CHECK(p.EndFlush() == 0);
    CHECK(p.m_bFlushing == 0); CHECK(p.m_bRunTimeError == 0);
    CHECK(cs.locks == 1); CHECK(cs.unlocks == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca8e40_TEST FAIL\n"); return 1; }
    std::printf("00ca8e40_TEST PASS\n");
    return 0;
}