#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_removed = 0; static void* g_table = 0;
struct CTable { virtual void Remove(long handle) { g_removed = handle; g_table = this; } };
struct CContext { CTable m_table; };
struct COwner { unsigned char _pad[8]; long m_handle; float m_offset[3];
    CTable* GetHandleTable(CContext* c) { return &c->m_table; }
    void RemoveHandle(CContext* c) { long handle = m_handle; if (handle >= 0) { GetHandleTable(c)->Remove(handle); m_handle = -1; } } };

int main()
{
    CContext ctx; COwner o; o.m_handle = 5; o.RemoveHandle(&ctx);
    if (g_removed != 5 || g_table != &ctx.m_table || o.m_handle != -1) { std::printf("00c62500_TEST FAIL\n"); return 1; }
    g_removed = 0; o.RemoveHandle(&ctx); if (g_removed != 0 || o.m_handle != -1) { std::printf("00c62500_TEST FAIL\n"); return 1; }
    std::printf("00c62500_TEST PASS\n");
    return 0;
}
