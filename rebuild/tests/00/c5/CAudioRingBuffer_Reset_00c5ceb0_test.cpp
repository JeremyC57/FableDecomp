// Behaviour test for 0x00C5CEB0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CAudioRingBuffer { unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    unsigned int committed;
    void Reset() { m_readPos = 0; m_writePos = 0; m_queued = m_capacity; }
    void CommitRead(unsigned int frames) { committed = frames; m_readPos = (m_readPos + frames) % m_capacity; m_queued -= frames; }
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; } };

int main()
{
    CAudioRingBuffer r; std::memset(&r, 0, sizeof(r)); r.m_capacity = 64; r.m_readPos = 5; r.m_writePos = 9; r.m_queued = 3;
    r.Reset();
    if (r.m_readPos != 0 || r.m_writePos != 0 || r.m_queued != 64 || r.m_capacity != 64) { std::printf("00c5ceb0_TEST FAIL\n"); return 1; }
    std::printf("00c5ceb0_TEST PASS\n");
    return 0;
}