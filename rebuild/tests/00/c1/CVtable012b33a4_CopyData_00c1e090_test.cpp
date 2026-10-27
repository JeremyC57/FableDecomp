#include <cstdio>

// Behaviour model of CVtable_012b33a4::CopyData (0x00C1E090): copy the data block, -1 when there is none.
#include <cstring>
struct Block {
    unsigned int m_size; const void* m_pData;
    int CopyData(void* buffer) { if (!m_pData) return -1; std::memcpy(buffer, m_pData, m_size); return 0; }
};

int main()
{

    const char src[7] = { 1, 2, 3, 4, 5, 6, 7 }; char dst[8] = { 0, 0, 0, 0, 0, 0, 0, 9 };
    Block b; b.m_size = 7; b.m_pData = 0;
    if (b.CopyData(dst) != -1 || dst[0] != 0) { std::printf("00c1e090_TEST FAIL\n"); return 1; }
    b.m_pData = src;
    if (b.CopyData(dst) != 0) { std::printf("00c1e090_TEST FAIL\n"); return 1; }
    if (dst[0] != 1 || dst[6] != 7 || dst[7] != 9) { std::printf("00c1e090_TEST FAIL\n"); return 1; }
    std::printf("00c1e090_TEST PASS\n");
    return 0;
}