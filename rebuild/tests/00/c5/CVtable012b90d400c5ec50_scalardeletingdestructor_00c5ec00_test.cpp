// Behaviour test for CVtable_012b90d4_00c5ec50::`scalar deleting destructor' (0x00C5EC00): an owned handle is closed
// exactly once; the -1 sentinel is never closed.
#include <cstdio>

static int g_closed = 0; static void* g_lastClosed = 0;
static int CloseHandleStub(void* handle) { ++g_closed; g_lastClosed = handle; return 1; }   // kernel32 stand-in

struct CVtable_012b90d4_00c5ec50 {
    CVtable_012b90d4_00c5ec50() : m_hHandle((void*)-1) {}
    virtual ~CVtable_012b90d4_00c5ec50();
    void* m_hHandle;
};
CVtable_012b90d4_00c5ec50::~CVtable_012b90d4_00c5ec50()
{
    if (m_hHandle != (void*)-1) {
        CloseHandleStub(m_hHandle);
        m_hHandle = (void*)-1;
    }
}

int main()
{
    int dummy;
    CVtable_012b90d4_00c5ec50* owner = new CVtable_012b90d4_00c5ec50;
    owner->m_hHandle = &dummy;
    delete owner;
    if (g_closed != 1 || g_lastClosed != &dummy) { std::printf("00c5ec00_TEST FAIL\n"); return 1; }
    CVtable_012b90d4_00c5ec50* idle = new CVtable_012b90d4_00c5ec50;
    delete idle;
    if (g_closed != 1) { std::printf("00c5ec00_TEST FAIL\n"); return 1; }
    std::printf("00c5ec00_TEST PASS\n");
    return 0;
}