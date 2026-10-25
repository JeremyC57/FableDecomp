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
// CCmdQueue::SetTimeAdvise: with a clock, reset the due event, find the earliest command (presentation list head,
// or the stream list head plus the run offset when running and earlier), and re-arm the clock advise when that time
// is positive and differs from the current advise (cancelling the old advise first).
struct IReferenceClock {
    int unadvises, advises; REFERENCE_TIME base, stream; DWORD lastCookie;
    IReferenceClock() : unadvises(0), advises(0), base(-1), stream(-1), lastCookie(0) {}
    HRESULT Unadvise(DWORD c) { unadvises++; lastCookie = c; return 0; }
    HRESULT AdviseTime(REFERENCE_TIME b, REFERENCE_TIME s, DWORD, DWORD* pc) { advises++; base = b; stream = s; *pc = 100 + advises; return 0; }
};
static int g_resets = 0;
class CRefTime {
public:
    REFERENCE_TIME m_time;
    CRefTime() { m_time = 0; }
    CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    operator REFERENCE_TIME() const { return m_time; }
    CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
const LONGLONG TimeZero = 0;
struct CAMEvent { HANDLE m_hEvent; void Reset() { g_resets++; } operator HANDLE() const { return m_hEvent; } };
struct CDeferredCommand { REFERENCE_TIME m_time; CRefTime GetTime() { return m_time; } };
struct CList {
    CDeferredCommand* items[4]; int n;
    CList() : n(0) {}
    int GetCount() const { return n; }
    CDeferredCommand** GetHeadPosition() { return n ? &items[0] : 0; }
    CDeferredCommand* Get(CDeferredCommand** pos) { return *pos; }
};
class CCmdQueue {
public:
    CList m_listPresentation; CList m_listStream; CAMEvent m_evDue; DWORD m_dwAdvise; CRefTime m_tCurrentAdvise;
    IReferenceClock* m_pClock; BOOL m_bRunning; CRefTime m_StreamTimeOffset;
    CCmdQueue() : m_dwAdvise(0), m_pClock(0), m_bRunning(0) { m_evDue.m_hEvent = 0; }
    void SetTimeAdvise(void)
    {
        if (!m_pClock) {
            return;
        }
        m_evDue.Reset();
        CRefTime current;
        if (m_listPresentation.GetCount() > 0) {
            CDeferredCommand** pos = m_listPresentation.GetHeadPosition();
            current = m_listPresentation.Get(pos)->GetTime();
        }
        if (m_bRunning) {
            CRefTime t;
            if (m_listStream.GetCount() > 0) {
                CDeferredCommand** pos = m_listStream.GetHeadPosition();
                t = m_listStream.Get(pos)->GetTime();
                t += m_StreamTimeOffset;
                if ((current == TimeZero) || (t < current)) {
                    current = t;
                }
            }
        }
        if ((current > TimeZero) && (current != m_tCurrentAdvise)) {
            if (m_dwAdvise) {
                m_pClock->Unadvise(m_dwAdvise);
                m_evDue.Reset();
            }
            m_pClock->AdviseTime((REFERENCE_TIME)current, TimeZero, (DWORD)0, &m_dwAdvise);
            m_tCurrentAdvise = current;
        }
    }
};
static void run()
{
    IReferenceClock clk; CDeferredCommand a, b; a.m_time = 500; b.m_time = 100;
    CCmdQueue q;
    q.SetTimeAdvise(); CHECK(g_resets == 0);                                   // no clock
    q.m_pClock = &clk;
    q.SetTimeAdvise(); CHECK(g_resets == 1); CHECK(clk.advises == 0);          // nothing queued
    q.m_listPresentation.items[0] = &a; q.m_listPresentation.n = 1;
    q.SetTimeAdvise(); CHECK(clk.advises == 1); CHECK(clk.base == 500 && clk.stream == 0); CHECK(q.m_dwAdvise == 101);
    CHECK(q.m_tCurrentAdvise.m_time == 500); CHECK(clk.unadvises == 0); CHECK(g_resets == 2);
    q.SetTimeAdvise(); CHECK(clk.advises == 1); CHECK(g_resets == 3);         // unchanged advise
    q.m_listStream.items[0] = &b; q.m_listStream.n = 1; q.m_StreamTimeOffset = 250;
    q.SetTimeAdvise(); CHECK(clk.advises == 1);                                // not running: stream ignored
    q.m_bRunning = 1;
    q.SetTimeAdvise(); CHECK(clk.advises == 2); CHECK(clk.base == 350); CHECK(clk.unadvises == 1); CHECK(clk.lastCookie == 101);
    CHECK(g_resets == 6); CHECK(q.m_dwAdvise == 102);
    q.m_StreamTimeOffset = 900;
    q.SetTimeAdvise(); CHECK(clk.advises == 3); CHECK(clk.base == 500);       // presentation earlier again
    q.m_listPresentation.n = 0; q.m_listStream.n = 0;
    q.SetTimeAdvise(); CHECK(clk.advises == 3);                                // time zero: no advise
}

int main()
{
    run();
    if (g_fail) { std::printf("00caf750_TEST FAIL\n"); return 1; }
    std::printf("00caf750_TEST PASS\n");
    return 0;
}