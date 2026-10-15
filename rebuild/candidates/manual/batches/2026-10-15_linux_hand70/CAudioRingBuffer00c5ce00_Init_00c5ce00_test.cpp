// Behaviour test for CAudioRingBuffer00c5ce00::Init @ 0x00C5CE00: the six parameters stored, the write cursor wrapped.
#include <cstdio>

struct CAudioRingBuffer00c5ce00 {
    unsigned long m_unknown00; unsigned char* m_pData; unsigned long m_capacity, m_frameBytes, m_channels;
    unsigned long m_playPosition, m_writePosition, m_queued;
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long playPosition, unsigned long queued)
    {
        m_pData = data; m_capacity = capacity; m_frameBytes = frameBytes; m_channels = channels;
        m_playPosition = playPosition; m_queued = queued;
        m_writePosition = (m_playPosition + m_queued) % m_capacity;
    }
};

int main()
{
    unsigned char buffer[16];
    CAudioRingBuffer00c5ce00 r; r.m_unknown00 = 77;
    r.Init(buffer, 0x80, 2, 1, 0, 0);
    if (r.m_pData != buffer || r.m_capacity != 0x80 || r.m_frameBytes != 2 || r.m_channels != 1 || r.m_writePosition != 0 || r.m_unknown00 != 77) {
        std::printf("00c5ce00_TEST FAIL\n"); return 1;
    }
    r.Init(buffer, 100, 4, 2, 70, 50);              // 120 wraps to 20
    if (r.m_playPosition != 70 || r.m_queued != 50 || r.m_writePosition != 20) { std::printf("00c5ce00_TEST FAIL\n"); return 1; }
    r.Init(buffer, 0xFFFFFFF0UL, 4, 2, 0xFFFFFFE0UL, 0x18);   // unsigned arithmetic: 0xFFFFFFF8 % 0xFFFFFFF0 = 8
    if (r.m_writePosition != 8) { std::printf("00c5ce00_TEST FAIL\n"); return 1; }
    std::printf("00c5ce00_TEST PASS\n");
    return 0;
}
