// Test: CVtable_012b4030::~CVtable_012b4030 -- destroying the part destroys exactly its +0xC worker queue, once.
#include <cstdio>
#include <cstddef>
static int g_dtors = 0; static const void* g_queue = 0;
struct PKCWorkerQueue { unsigned char _body[0x30]; ~PKCWorkerQueue() { ++g_dtors; g_queue = this; } };
struct CPart { void* m_vptr; void* m_link[2]; PKCWorkerQueue m_worker; void* m_pOwner; };
int main()
{
    bool ok = offsetof(CPart, m_worker) == 0xc && offsetof(CPart, m_pOwner) == 0x3c && sizeof(CPart) == 0x40;
    const void* where = 0;
    { CPart part; where = &part.m_worker; }
    ok = ok && g_dtors == 1 && g_queue == where;
    std::printf(ok ? "00c14710_TEST PASS\n" : "00c14710_TEST FAIL\n");
    return ok ? 0 : 1;
}