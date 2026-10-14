#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_deleted = 0; struct SBuffer { char* m_pBuffer; long m_count; ~SBuffer() { if (m_pBuffer) { delete[] m_pBuffer; ++g_deleted; m_pBuffer = 0; } m_count = 0; } };

int main()
{
    SBuffer* p = (SBuffer*)std::malloc(sizeof(SBuffer)); p->m_pBuffer = new char[4]; p->m_count = 3; p->~SBuffer();
    if (g_deleted != 1 || p->m_pBuffer || p->m_count) { std::printf("008413f0_TEST FAIL\n"); return 1; } p->~SBuffer(); if (g_deleted != 1) { std::printf("008413f0_TEST FAIL\n"); return 1; } std::free(p);
    std::printf("008413f0_TEST PASS\n");
    return 0;
}
