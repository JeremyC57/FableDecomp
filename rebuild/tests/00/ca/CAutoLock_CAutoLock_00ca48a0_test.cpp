// behaviour test for 0x00CA48A0 CAutoLock::CAutoLock: remember the critical section and enter it (left again by the destructor)
#include <cstdio>

struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
static CRITICAL_SECTION* g_lastEntered = 0;
static void TestEnterCriticalSection(CRITICAL_SECTION* section) { ++section->RecursionCount; g_lastEntered = section; }   // stand-in for the IAT call
static void TestLeaveCriticalSection(CRITICAL_SECTION* section) { --section->RecursionCount; }

class CCritSec {
    CRITICAL_SECTION m_CritSec;
public:
    CCritSec() { m_CritSec.RecursionCount = 0; }
    void Lock() { TestEnterCriticalSection(&m_CritSec); }
    void Unlock() { TestLeaveCriticalSection(&m_CritSec); }
    long Depth() const { return m_CritSec.RecursionCount; }
    CRITICAL_SECTION* Section() { return &m_CritSec; }
};

class CAutoLock {
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock);
    ~CAutoLock() { m_pLock->Unlock(); }
    CCritSec* Lock() const { return m_pLock; }
};

CAutoLock::CAutoLock(CCritSec* plock)
{
    m_pLock = plock;
    m_pLock->Lock();
}

int main()
{
    CCritSec section;
    {
        CAutoLock outer(&section);
        if (section.Depth() != 1 || outer.Lock() != &section || g_lastEntered != section.Section()) { std::printf("00ca48a0_TEST FAIL\n"); return 1; }
        {
            CAutoLock inner(&section);
            if (section.Depth() != 2) { std::printf("00ca48a0_TEST FAIL\n"); return 1; }
        }
        if (section.Depth() != 1) { std::printf("00ca48a0_TEST FAIL\n"); return 1; }
    }
    if (section.Depth() != 0) { std::printf("00ca48a0_TEST FAIL\n"); return 1; }
    std::printf("00ca48a0_TEST PASS\n");
    return 0;
}