#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CRITICAL_SECTION_Fable { int m_entered; int m_left; bool m_tryOnlyFails; };
struct CCriticalSectionLock {
    CRITICAL_SECTION_Fable* m_pSection; bool m_locked;
    CCriticalSectionLock(CRITICAL_SECTION_Fable* s, bool tryOnly) : m_pSection(s), m_locked(true)
    { if (tryOnly && s->m_tryOnlyFails) { m_locked = false; return; } ++s->m_entered; }
    ~CCriticalSectionLock() { if (m_locked) ++m_pSection->m_left; }
};
struct CHandleBasedMemoryPool { long m_locked; long m_freed; void Lock(long h) { m_locked = h; } void Free(long h) { m_freed = h; } };
struct CBankFileAsync { CRITICAL_SECTION_Fable m_section; };
struct CMovableResourceMemoryPool { CRITICAL_SECTION_Fable m_section; };
struct CBankFileAsyncData {
    void* m_pAllocation; void* m_pData; void* m_pReadBuffer; unsigned long m_bytesPending;
    CHandleBasedMemoryPool* m_pPool; long m_handle; CMovableResourceMemoryPool* m_pMovablePool; CBankFileAsync* m_pBank;
    bool m_relocatable, m_relocating, m_finished, m_dataLocked, m_completionPending; bool m_setupResult;
    bool SetupBuffer(bool) { return m_setupResult; }
    bool OnFileReadBegin(void** ppBuffer, unsigned long)
    {
        CCriticalSectionLock lock(&m_pBank->m_section, true);
        if (lock.m_locked) { if (SetupBuffer(false)) { *ppBuffer = m_pReadBuffer; return true; } }
        return false;
    }
    void OnFileReadCompletion(bool)
    {
        CCriticalSectionLock lock(&m_pBank->m_section, false);
        m_finished = true; m_bytesPending = 0;
        if (m_handle != -1) m_completionPending = true;
    }
    bool IsFinished()
    {
        CCriticalSectionLock lock(&m_pBank->m_section, false);
        if (m_finished) { m_completionPending = false; return true; }
        return false;
    }
    void* GetData()
    {
        CCriticalSectionLock lock(&m_pBank->m_section, false);
        if (m_pPool != 0 && m_handle != -1 && !m_dataLocked) { m_pPool->Lock(m_handle); m_dataLocked = true; }
        return m_pData;
    }
    bool CanRelocateData(long) const
    {
        CCriticalSectionLock lock(&m_pMovablePool->m_section, false);
        if (m_relocatable && !m_relocating) return true;
        return false;
    }
    void RelocateData(long, void* oldBase, void* newBase, long)
    {
        CCriticalSectionLock lock(&m_pMovablePool->m_section, false);
        if (m_pData != 0) m_pData = (char*)newBase + ((char*)m_pData - (char*)oldBase);
        if (m_pAllocation != 0) m_pAllocation = (char*)newBase + ((char*)m_pAllocation - (char*)oldBase);
    }
    void FreeBuffer()
    {
        if (m_pPool != 0 && m_handle != -1) { m_pPool->Free(m_handle); m_handle = -1; m_pReadBuffer = 0; m_pData = 0; }
    }
};
static CBankFileAsync g_bank; static CMovableResourceMemoryPool g_mpool; static CHandleBasedMemoryPool g_pool;
static CBankFileAsyncData Make()
{
    CBankFileAsyncData d; std::memset(&d, 0, sizeof d); std::memset(&g_bank, 0, sizeof g_bank); std::memset(&g_mpool, 0, sizeof g_mpool); std::memset(&g_pool, 0, sizeof g_pool);
    d.m_pBank = &g_bank; d.m_pMovablePool = &g_mpool; d.m_pPool = &g_pool; d.m_handle = 5; d.m_pReadBuffer = (void*)0x100; d.m_pData = (void*)0x200;
    return d;
}
static bool Balanced(const CRITICAL_SECTION_Fable& s, int n) { return s.m_entered == n && s.m_left == n; }

static bool Check()
{
    CBankFileAsyncData d = Make();
    if (d.GetData() != (void*)0x200 || g_pool.m_locked != 5 || !d.m_dataLocked || !Balanced(g_bank.m_section, 1)) return false;
    g_pool.m_locked = 0;
    if (d.GetData() != (void*)0x200 || g_pool.m_locked != 0) return false;        // already pinned
    CBankFileAsyncData e = Make(); e.m_handle = -1;
    return e.GetData() == (void*)0x200 && g_pool.m_locked == 0 && !e.m_dataLocked;
}
int main()
{
    if (!Check()) { std::printf("009d5580_TEST FAIL\n"); return 1; }
    std::printf("009d5580_TEST PASS\n");
    return 0;
}
