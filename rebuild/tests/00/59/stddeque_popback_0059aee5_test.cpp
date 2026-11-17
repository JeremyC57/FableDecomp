#include <cstdio>
static int g_popBlocks;
struct CBlockStack { unsigned char _pad[0x10]; long* m_cursor; long* m_blockStart; void PopBlock() { ++g_popBlocks; } void PopValue(); };
void CBlockStack::PopValue() { if (m_cursor != m_blockStart) m_cursor = m_cursor - 1; else PopBlock(); }
int main()
{
    long block[4]; CBlockStack s; s.m_blockStart = block; s.m_cursor = block + 2;
    s.PopValue(); if (s.m_cursor != block + 1 || g_popBlocks != 0) { std::printf("0059aee5_TEST FAIL\n"); return 1; }
    s.PopValue(); s.PopValue(); if (s.m_cursor != block || g_popBlocks != 1) { std::printf("0059aee5_TEST FAIL\n"); return 1; }
    std::printf("0059aee5_TEST PASS\n");
    return 0;
}