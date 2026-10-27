// Behaviour test for 0x00C5E2B0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct SKsStreamBlock { unsigned int Size; unsigned char _h[0x24]; void* Data; unsigned int Options; unsigned char ov[0x14]; unsigned int pad; };
struct K { SKsStreamBlock* m_pBlocks; unsigned int m_writeIndex; void* LockChunk() { return m_pBlocks[m_writeIndex].Data; } };
int main()
{
    static int x, y; SKsStreamBlock blocks[2]; blocks[0].Data = &x; blocks[1].Data = &y;
    K k; k.m_pBlocks = blocks; k.m_writeIndex = 1;
    if (k.LockChunk() != &y) { std::printf("00c5e2b0_TEST FAIL\n"); return 1; }
    k.m_writeIndex = 0;
    if (k.LockChunk() != &x) { std::printf("00c5e2b0_TEST FAIL\n"); return 1; }
    std::printf("00c5e2b0_TEST PASS\n");
    return 0;
}