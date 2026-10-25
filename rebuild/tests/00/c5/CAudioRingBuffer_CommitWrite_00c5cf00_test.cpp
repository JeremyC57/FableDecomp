#include <cstdio>
#include <cstring>
struct CAudioRingBuffer {
    unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void SetCapacity(unsigned int bytes, bool keepQueued)
    {
        unsigned int capacity = bytes / m_frameBytes;
        if (keepQueued) {
            m_queued += capacity - m_capacity;
            m_writePos = (m_readPos + m_queued) % capacity;
        }
        m_capacity = capacity;
    }
    void CommitWrite(unsigned int frames)
    {
        m_queued += frames;
        m_writePos += frames;
        if (m_writePos >= m_capacity)
            m_writePos -= m_capacity;
    }
    void CommitRead(unsigned int frames)
    {
        m_queued -= frames;
        m_readPos += frames;
        if (m_queued == 0) {
            m_readPos = 0;
            m_writePos = 0;
        } else if (m_readPos >= m_capacity)
            m_readPos -= m_capacity;
    }
};
static CAudioRingBuffer Make(unsigned int cap, unsigned int fb, unsigned int rd, unsigned int wr, unsigned int q)
{ CAudioRingBuffer r; std::memset(&r, 0, sizeof(r)); r.m_capacity = cap; r.m_frameBytes = fb; r.m_readPos = rd; r.m_writePos = wr; r.m_queued = q; return r; }
int main()
{
    CAudioRingBuffer r = Make(128, 2, 0, 100, 10);
    r.CommitWrite(20);
    int ok = r.m_queued == 30 && r.m_writePos == 120;
    r.CommitWrite(8);                                 // lands exactly on the capacity: wraps to 0
    ok = ok && r.m_queued == 38 && r.m_writePos == 0;
    r.CommitWrite(130 - 128 + 1);
    ok = ok && r.m_writePos == 3 && r.m_queued == 41;
    if (!ok) { std::printf("00c5cf00_TEST FAIL\n"); return 1; }
    std::printf("00c5cf00_TEST PASS\n");
    return 0;
}