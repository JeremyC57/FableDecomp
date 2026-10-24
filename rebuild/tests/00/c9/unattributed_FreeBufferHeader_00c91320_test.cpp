// Behaviour test for 00c91320 (FreeBufferHeader): the data block at +8 is freed and all five header words are zeroed (a null block is still freed harmlessly).
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0;
static void* g_lastFreed = 0;
static void test_free(void* p) { ++g_frees; g_lastFreed = p; std::free(p); }
struct SBufferHeader14 {
    long  m_count;
    long  m_capacity;
    void* m_pData;
    long  m_c;
    long  m_10;
};
void FreeBufferHeader(SBufferHeader14* header)
{
    test_free(header->m_pData);
    std::memset(header, 0, sizeof(SBufferHeader14));
}
int main()
{
    SBufferHeader14 header;
    header.m_count = 3; header.m_capacity = 8; header.m_c = 5; header.m_10 = 9;
    void* block = std::malloc(32);
    header.m_pData = block;
    FreeBufferHeader(&header);
    if (g_frees != 1 || g_lastFreed != block) { std::printf("00c91320_TEST FAIL\n"); return 1; }
    if (header.m_count != 0 || header.m_capacity != 0 || header.m_pData != 0 || header.m_c != 0 || header.m_10 != 0) { std::printf("00c91320_TEST FAIL\n"); return 1; }
    FreeBufferHeader(&header);
    if (g_frees != 2 || g_lastFreed != 0) { std::printf("00c91320_TEST FAIL\n"); return 1; }
    std::printf("00c91320_TEST PASS\n");
    return 0;
}