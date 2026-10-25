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
// CCmdQueue::CheckTime: no clock -> not due; stream times need the graph running and get the run offset added;
// due when the (presentation) time is at or before the clock's current time.
struct IReferenceClock { REFERENCE_TIME now; int calls; IReferenceClock() : now(0), calls(0) {} HRESULT GetTime(REFERENCE_TIME* p) { calls++; *p = now; return 0; } };
class CRefTime {
public:
    REFERENCE_TIME m_time;
    CRefTime() { m_time = 0; }
    CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    operator REFERENCE_TIME() const { return m_time; }
    CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
class CCmdQueue {
public:
    IReferenceClock* m_pClock; BOOL m_bRunning; CRefTime m_StreamTimeOffset;
    BOOL CheckTime(CRefTime time, BOOL bStream)
    {
        if (!m_pClock) {
            return 0;
        }
        if (bStream) {
            if (!m_bRunning) {
                return 0;
            }
            time += m_StreamTimeOffset;
        }
        CRefTime Now;
        m_pClock->GetTime((REFERENCE_TIME*)&Now);
        return (time <= Now);
    }
};
static void run()
{
    IReferenceClock clk; clk.now = 1000;
    CCmdQueue q; q.m_pClock = 0; q.m_bRunning = 0; q.m_StreamTimeOffset = 300;
    CHECK(q.CheckTime(CRefTime(10), 0) == 0);
    q.m_pClock = &clk;
    CHECK(q.CheckTime(CRefTime(1000), 0) == 1); CHECK(q.CheckTime(CRefTime(1001), 0) == 0);
    CHECK(q.CheckTime(CRefTime(10), 1) == 0); CHECK(clk.calls == 2);
    q.m_bRunning = 1;
    CHECK(q.CheckTime(CRefTime(700), 1) == 1); CHECK(q.CheckTime(CRefTime(701), 1) == 0);
    clk.now = LL(0x100000000);
    CHECK(q.CheckTime(CRefTime(LL(0xFFFFFFFF)), 0) == 1); CHECK(q.CheckTime(CRefTime(LL(0x100000001)), 0) == 0);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cae170_TEST FAIL\n"); return 1; }
    std::printf("00cae170_TEST PASS\n");
    return 0;
}