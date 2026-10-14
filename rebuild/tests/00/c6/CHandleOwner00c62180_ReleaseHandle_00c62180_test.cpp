#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_released = 0, g_context = 0; struct CTable { virtual void Release(long handle) { g_released = handle; } }; static CTable g_table; static CTable* GetHandleTable(long context) { g_context = context; return &g_table; }
struct COwner { unsigned char _pad[0x4]; long m_handle; void ReleaseHandle(long context) { long handle = m_handle; if (handle >= 0) { GetHandleTable(context)->Release(handle); m_handle = -1; } } };

int main() {
    COwner o; std::memset(&o, 0, sizeof o); o.m_handle = 5; o.ReleaseHandle(9); if (g_released != 5 || g_context != 9 || o.m_handle != -1) { std::printf("00c62180_TEST FAIL\n"); return 1; } g_released = 0; o.ReleaseHandle(9); if (g_released != 0 || o.m_handle != -1) { std::printf("00c62180_TEST FAIL\n"); return 1; }
    std::printf("00c62180_TEST PASS\n"); return 0;
}