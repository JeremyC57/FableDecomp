// Behaviour test for 00c5ce00: CAudioRingBuffer::Init -- fields stored, write cursor = (read + queued) % capacity
#include <cstdio>
class CPKObject { public: virtual ~CPKObject() { } };
class CAudioRingBuffer : public CPKObject {
public:
    unsigned char* m_pData; unsigned long m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels, unsigned long readPos, unsigned long queued)
    { m_pData = data; m_capacity = capacity; m_frameBytes = frameBytes; m_channels = channels; m_readPos = readPos; m_queued = queued; m_writePos = (m_readPos + m_queued) % m_capacity; }
};
int main()
{
    static unsigned char data[0x600]; CAudioRingBuffer r;
    r.Init(data, 0x80, 2, 6, 0x70, 0x20);
    int ok = r.m_pData == data && r.m_capacity == 0x80 && r.m_frameBytes == 2 && r.m_channels == 6 && r.m_readPos == 0x70
          && r.m_queued == 0x20 && r.m_writePos == 0x10;
    r.Init(data, 0x80, 2, 1, 0, 0); ok = ok && r.m_writePos == 0 && r.m_channels == 1;
    std::printf(ok ? "00c5ce00_TEST PASS\n" : "00c5ce00_TEST FAIL\n");
    return ok ? 0 : 1;
}
