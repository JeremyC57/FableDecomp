// Test for 0x00C4AA20 PKCBufferedReader::Tell: block index times block size plus the in-block offset.
#include <cstdio>
struct CReader { unsigned int m_blockSize, m_currentBlock, m_blockOffset;
    unsigned int Tell() { return m_currentBlock * m_blockSize + m_blockOffset; } };
int main()
{
    CReader r; r.m_blockSize = 0x1000; r.m_currentBlock = 3; r.m_blockOffset = 0x20;
    if (r.Tell() != 0x3020) { std::printf("00c4aa20_TEST FAIL\n"); return 1; }
    std::printf("00c4aa20_TEST PASS\n");
    return 0;
}