// Behaviour test for 0x00C5EE60 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct N { unsigned int m_blockCount, m_written; bool m_waited;
    bool IsChunkReady()
    {
        if (m_written >= m_blockCount) {
            if (!m_waited) { m_waited = true; return false; }
            m_waited = false; m_written = 0;
        }
        return true;
    } };
int main()
{
    N n; n.m_blockCount = 2; n.m_written = 1; n.m_waited = false;
    if (!n.IsChunkReady() || n.m_written != 1) { std::printf("00c5ee60_TEST FAIL\n"); return 1; }
    n.m_written = 2;
    if (n.IsChunkReady() || !n.m_waited || n.m_written != 2) { std::printf("00c5ee60_TEST FAIL\n"); return 1; }
    if (!n.IsChunkReady() || n.m_waited || n.m_written != 0) { std::printf("00c5ee60_TEST FAIL\n"); return 1; }
    std::printf("00c5ee60_TEST PASS\n");
    return 0;
}