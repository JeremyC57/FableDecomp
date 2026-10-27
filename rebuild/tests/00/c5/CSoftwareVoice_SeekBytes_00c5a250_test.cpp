// Behaviour test for 0x00C5A250 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CAudioRingBuffer { unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    unsigned int committed;
    void Reset() { m_readPos = 0; m_writePos = 0; m_queued = m_capacity; }
    void CommitRead(unsigned int frames) { committed = frames; m_readPos = (m_readPos + frames) % m_capacity; m_queued -= frames; }
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; } };

struct V { CAudioRingBuffer m_ring; unsigned int m_1e4, m_step, m_phase;
    void SeekBytes(unsigned int bytes)
    {
        if (m_ring.m_pData) { unsigned int frameBytes = m_ring.m_frameBytes; m_ring.Reset(); m_ring.CommitRead(bytes / frameBytes); }
        m_phase = 0; m_1e4 = 0;
    } };
int main()
{
    static unsigned char mem[256];
    V v; std::memset(&v, 0, sizeof(v)); v.m_1e4 = 3; v.m_phase = 7; v.m_ring.m_capacity = 100;
    v.SeekBytes(40);
    if (v.m_ring.committed != 0 || v.m_phase != 0 || v.m_1e4 != 0) { std::printf("00c5a250_TEST FAIL\n"); return 1; }
    v.m_ring.m_pData = mem; v.m_ring.m_frameBytes = 4; v.m_ring.m_readPos = 50;
    v.SeekBytes(40);
    if (v.m_ring.committed != 10 || v.m_ring.m_readPos != 10 || v.m_ring.m_queued != 90) { std::printf("00c5a250_TEST FAIL\n"); return 1; }
    std::printf("00c5a250_TEST PASS\n");
    return 0;
}