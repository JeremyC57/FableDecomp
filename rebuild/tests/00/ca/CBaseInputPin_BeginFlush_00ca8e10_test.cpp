// Behaviour test for 0x00ca8e10: CBaseInputPin::BeginFlush (amfilter.cpp) -- under the filter lock set m_bFlushing; S_OK;
// m_bRunTimeError untouched; the flag is set while the lock is held.
#include <cstdio>
typedef int HRESULT;
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
static bool g_heldAtStore = false;
class CBaseInputPin {
public:
    CCritSec* m_pLock; unsigned char m_bRunTimeError; unsigned char m_bFlushing;
    HRESULT BeginFlush()
    {
        CAutoLock lck(m_pLock);
        m_bFlushing = 1;
        g_heldAtStore = m_pLock->locks == 1 && m_pLock->unlocks == 0;
        return 0;
    }
};
int main()
{
    CCritSec cs; CBaseInputPin p; p.m_pLock = &cs; p.m_bFlushing = 0; p.m_bRunTimeError = 7;
    CHECK(p.BeginFlush() == 0);
    CHECK(p.m_bFlushing == 1); CHECK(p.m_bRunTimeError == 7); CHECK(g_heldAtStore);
    CHECK(cs.locks == 1); CHECK(cs.unlocks == 1);
    if (g_fail) { std::printf("00ca8e10_TEST FAIL\n"); return 1; }
    std::printf("00ca8e10_TEST PASS\n");
    return 0;
}
