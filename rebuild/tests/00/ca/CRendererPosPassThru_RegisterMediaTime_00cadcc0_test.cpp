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
// CRendererPosPassThru::RegisterMediaTime(LONGLONG, LONGLONG): under the position lock record the media times
// and mark them valid.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CRendererPosPassThru {
public:
    CCritSec m_PositionLock; LONGLONG m_StartMedia; LONGLONG m_EndMedia; BOOL m_bReset;
    HRESULT RegisterMediaTime(LONGLONG StartTime, LONGLONG EndTime)
    {
        CAutoLock cAutoLock(&m_PositionLock);
        m_StartMedia = StartTime;
        m_EndMedia = EndTime;
        m_bReset = 0;
        return 0;
    }
};
static void run()
{
    CRendererPosPassThru r; r.m_bReset = 1; r.m_StartMedia = 0; r.m_EndMedia = 0;
    LONGLONG a = LL(0x100000005), b = LL(0x200000007);
    CHECK(r.RegisterMediaTime(a, b) == 0);
    CHECK(r.m_StartMedia == a); CHECK(r.m_EndMedia == b); CHECK(r.m_bReset == 0);
    CHECK(r.m_PositionLock.locks == 1 && r.m_PositionLock.unlocks == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cadcc0_TEST FAIL\n"); return 1; }
    std::printf("00cadcc0_TEST PASS\n");
    return 0;
}