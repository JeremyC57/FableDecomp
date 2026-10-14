#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_deleted = 0; struct SBuffer { long m_0; char* m_pBuffer; long m_count; ~SBuffer() { delete[] m_pBuffer; ++g_deleted; m_pBuffer = 0; m_count = 0; } };

int main() {
    SBuffer* p = new SBuffer; p->m_pBuffer = new char[4]; p->m_count = 3; SBuffer* keep = p; delete p; if (g_deleted != 1) { std::printf("006fb430_TEST FAIL\n"); return 1; } (void)keep;
    std::printf("006fb430_TEST PASS\n"); return 0;
}
