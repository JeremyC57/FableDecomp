// Behaviour test for 00c5e3f0 CVtable_012b90d4_00c5ec50::Reset (copy of the source model, CloseHandle spy).
#include <cstdio>

static int   g_closeCount = 0;
static void* g_lastClosed = 0;
static int CloseHandleSpy(void* handle) { ++g_closeCount; g_lastClosed = handle; return 1; }

struct CHandleHolder {
    virtual void v00() {}
    void* m_hHandle;
    void Close()
    {
        if (m_hHandle != (void*)-1) {
            CloseHandleSpy(m_hHandle);
            m_hHandle = (void*)-1;
        }
    }
    void Reset(void* handle);
};

void CHandleHolder::Reset(void* handle)
{
    Close();
    m_hHandle = handle;
}

int main()
{
    CHandleHolder h;
    h.m_hHandle = (void*)-1;
    int ok = 1;
    h.Reset((void*)0x100);                       // nothing open: no close
    if (g_closeCount != 0 || h.m_hHandle != (void*)0x100) ok = 0;
    h.Reset((void*)0x200);                       // closes the old one
    if (g_closeCount != 1 || g_lastClosed != (void*)0x100 || h.m_hHandle != (void*)0x200) ok = 0;
    h.Reset((void*)-1);
    if (g_closeCount != 2 || g_lastClosed != (void*)0x200 || h.m_hHandle != (void*)-1) ok = 0;
    h.Reset((void*)0x300);
    if (g_closeCount != 2 || h.m_hHandle != (void*)0x300) ok = 0;
    if (!ok) { std::printf("00c5e3f0_TEST FAIL\n"); return 1; }
    std::printf("00c5e3f0_TEST PASS\n");
    return 0;
}