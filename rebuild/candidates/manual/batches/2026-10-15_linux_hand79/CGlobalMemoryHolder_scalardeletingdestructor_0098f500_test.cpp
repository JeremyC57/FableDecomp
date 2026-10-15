// Behaviour test for CGlobalMemoryHolder's scalar deleting destructor @ 0x0098F500: deleting the holder
// releases its global-memory handle (when present) and then the holder's own storage.
#include <cstdio>
#include <cstdlib>

static void* g_released = 0;
static int g_releaseCount = 0;
static void* GlobalFreeStub(void* h) { g_released = h; ++g_releaseCount; return 0; }
static int g_deleteCount = 0;

struct CGlobalMemoryHolder {
    unsigned char _pad00[0x20];
    void*         m_hMemory;
    ~CGlobalMemoryHolder() { if (m_hMemory) GlobalFreeStub(m_hMemory); }
    static void operator delete(void* p) { ++g_deleteCount; std::free(p); }
};

int main()
{
    int handle = 0;
    bool ok = true;
    CGlobalMemoryHolder* h = new CGlobalMemoryHolder;
    h->m_hMemory = &handle;
    delete h;
    if (g_releaseCount != 1 || g_released != &handle || g_deleteCount != 1) ok = false;

    CGlobalMemoryHolder* e = new CGlobalMemoryHolder;
    e->m_hMemory = 0;
    delete e;
    if (g_releaseCount != 1 || g_deleteCount != 2) ok = false;

    if (!ok) { std::printf("0098f500_TEST FAIL\n"); return 1; }
    std::printf("0098f500_TEST PASS\n");
    return 0;
}
